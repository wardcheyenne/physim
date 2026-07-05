/**
 * Charged particle simulation - Coulomb interaction between ions.
 */
#include "simulation.h"
#include "forces.h"
#include "integrators.h"
#include "collision.h"
#include <iostream>
#include <iomanip>
#include <random>

int main() {
    using namespace physim;

    auto sim = Simulation(std::make_unique<EulerIntegrator>());
    sim.add_force(std::make_shared<ElectrostaticForce>());
    sim.set_collision_detector(std::make_shared<SphereCollisionDetector>());
    sim.set_collision_resolver(std::make_shared<ElasticCollisionResolver>(0.9));

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> pos_dist(-1e-8, 1e-8);
    std::uniform_real_distribution<double> charge_dist(-1.6e-19, 1.6e-19);

    for (int i = 0; i < 20; ++i) {
        Particle p(i, 1.67e-27,
                   {pos_dist(rng), pos_dist(rng), pos_dist(rng)},
                   Vector3::zero(),
                   charge_dist(rng),
                   1e-10);
        sim.add_particle(p);
    }

    constexpr double DT = 1e-16;
    constexpr double T  = 1e-13;
    size_t step = 0;

    sim.run(T, DT, [&](const auto& ps, double t) {
        if (step++ % 1000 == 0) {
            double ke = 0;
            for (const auto& p : ps) ke += p.kinetic_energy();
            std::cout << "t=" << std::scientific << t << "  KE=" << ke << " J\n";
        }
    });

    auto s = sim.stats();
    std::cout << "Collisions: " << s.collision_count << "\n";
}
