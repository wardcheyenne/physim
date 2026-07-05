#include "integrators.h"
#include <functional>
#include <stdexcept>

namespace physim {

/**
This is a function to advance particles by one timestep using explicit Euler integration.
Input(s):
- particles: the particles to integrate, updated in place
- dt: the timestep size
Output(s):
- None: mutates each non-fixed particle's velocity and position
**/
void EulerIntegrator::step(std::vector<Particle>& particles, double dt) const {
    for (auto& p : particles) {
        if (p.fixed) continue;
        Vector3 accel = p.force * (1.0 / p.mass);
        p.velocity += accel * dt;
        p.position += p.velocity * dt;
    }
}

/**
This is a function to advance particles by one timestep using fourth-order Runge-Kutta integration.
Input(s):
- particles: the particles to integrate, updated in place
- dt: the timestep size
Output(s):
- None: mutates each non-fixed particle's position and velocity; throws std::invalid_argument if dt exceeds the stability limit
**/
void RK4Integrator::step(std::vector<Particle>& particles, double dt) const {
    const size_t n = particles.size();
    if (n == 0) return;

    //Clamp dt to avoid catastrophic divergence on large timesteps
    constexpr double MAX_DT = 1.0;
    if (dt > MAX_DT) {
        throw std::invalid_argument("RK4: dt too large, would cause numerical instability");
    }

    std::vector<Vector3> pos0(n), vel0(n);
    for (size_t i = 0; i < n; ++i) {
        pos0[i] = particles[i].position;
        vel0[i] = particles[i].velocity;
    }

    auto k = [&](std::vector<Particle>& ps) {
        std::vector<std::pair<Vector3,Vector3>> deriv(n);
        for (size_t i = 0; i < n; ++i) {
            if (ps[i].fixed) { deriv[i] = {{},{}}; continue; }
            Vector3 a = force_fn_(ps[i], ps) * (1.0 / ps[i].mass);
            deriv[i]  = {ps[i].velocity, a};
        }
        return deriv;
    };

    auto k1 = k(particles);
    std::vector<Particle> tmp = particles;

    for (size_t i = 0; i < n; ++i) {
        tmp[i].position = pos0[i] + k1[i].first  * (dt/2);
        tmp[i].velocity = vel0[i] + k1[i].second * (dt/2);
    }
    auto k2 = k(tmp);

    for (size_t i = 0; i < n; ++i) {
        tmp[i].position = pos0[i] + k2[i].first  * (dt/2);
        tmp[i].velocity = vel0[i] + k2[i].second * (dt/2);
    }
    auto k3 = k(tmp);

    for (size_t i = 0; i < n; ++i) {
        tmp[i].position = pos0[i] + k3[i].first  * dt;
        tmp[i].velocity = vel0[i] + k3[i].second * dt;
    }
    auto k4 = k(tmp);

    for (size_t i = 0; i < n; ++i) {
        if (particles[i].fixed) continue;
        particles[i].position = pos0[i] + (k1[i].first  + 2*k2[i].first  + 2*k3[i].first  + k4[i].first)  * (dt/6);
        particles[i].velocity = vel0[i] + (k1[i].second + 2*k2[i].second + 2*k3[i].second + k4[i].second) * (dt/6);
    }
}

} //namespace physim
