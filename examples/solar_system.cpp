/**
 * Solar system N-body simulation (scaled units).
 * Demonstrates gravitational force and RK4 integration.
 */
#include "simulation.h"
#include "forces.h"
#include "integrators.h"
#include <iostream>
#include <iomanip>

int main() {
    using namespace physim;

    //Masses in units where G = 1 (scaled)
    auto sim = Simulation(std::make_unique<EulerIntegrator>());
    sim.add_force(std::make_shared<GravitationalForce>());

    //Sun (fixed at origin)
    Particle sun(0, 1.989e30);
    sun.label = "Sun";
    sun.fixed = true;
    sim.add_particle(sun);

    //Earth (circular orbit: v = sqrt(G*M/r))
    constexpr double r_earth = 1.496e11;
    constexpr double v_earth = 2.978e4;
    Particle earth(1, 5.972e24, {r_earth, 0, 0}, {0, v_earth, 0});
    earth.label = "Earth";
    sim.add_particle(earth);

    //Mars
    constexpr double r_mars = 2.279e11;
    constexpr double v_mars = 2.413e4;
    Particle mars(2, 6.417e23, {r_mars, 0, 0}, {0, v_mars, 0});
    mars.label = "Mars";
    sim.add_particle(mars);

    //Run for 1 Earth year in steps of 1 hour
    constexpr double DT = 3600.0;
    constexpr double T  = 365.25 * 24 * 3600;

    size_t step = 0;
    sim.run(T, DT, [&](const auto& ps, double t) {
        if (step++ % 720 == 0) {  //print every 30 days
            std::cout << std::fixed << std::setprecision(0)
                      << "t=" << t/(3600*24) << "d  Earth: " << ps[1].position << "\n";
        }
    });

    auto s = sim.stats();
    std::cout << "Steps: " << s.step_count
              << "  KE: " << s.total_kinetic_energy << " J\n";
}
