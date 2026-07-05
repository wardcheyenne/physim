#pragma once
#include "particle.h"
#include <vector>
#include <functional>

namespace physim {

class Integrator {
public:
    virtual ~Integrator() = default;
    /**
    This is a function to advance a set of particles forward in time by one timestep.
    Input(s):
    - particles: the particles to integrate, updated in place
    - dt: the timestep size
    Output(s):
    - None: mutates each particle's position and velocity
    **/
    virtual void step(std::vector<Particle>& particles, double dt) const = 0;
};

class EulerIntegrator : public Integrator {
public:
    /**
    This is a function to advance particles by one timestep using explicit Euler integration.
    Input(s):
    - particles: the particles to integrate, updated in place
    - dt: the timestep size
    Output(s):
    - None: mutates each particle's position and velocity
    **/
    void step(std::vector<Particle>& particles, double dt) const override;
};

class RK4Integrator : public Integrator {
public:
    using ForceFunc = std::function<Vector3(const Particle&, const std::vector<Particle>&)>;
    /**
    This is a function to construct an RK4 integrator with a force function used to evaluate accelerations at intermediate stages.
    Input(s):
    - ff: the force function, mapping a particle and the full particle list to a force vector
    Output(s):
    - None: stores the force function for use during integration
    **/
    explicit RK4Integrator(ForceFunc ff) : force_fn_(std::move(ff)) {}
    /**
    This is a function to advance particles by one timestep using fourth-order Runge-Kutta integration.
    Input(s):
    - particles: the particles to integrate, updated in place
    - dt: the timestep size
    Output(s):
    - None: mutates each particle's position and velocity
    **/
    void step(std::vector<Particle>& particles, double dt) const override;
private:
    ForceFunc force_fn_;
};

} //namespace physim
