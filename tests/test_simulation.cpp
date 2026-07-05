#include <gtest/gtest.h>
#include "simulation.h"
#include "forces.h"
#include "integrators.h"
using namespace physim;

class SimTest : public ::testing::Test {
protected:
    void SetUp() override {
        sim = std::make_unique<Simulation>(std::make_unique<EulerIntegrator>());
        sim->add_force(std::make_shared<GravitationalForce>());
    }
    std::unique_ptr<Simulation> sim;
};

TEST_F(SimTest, ParticleCountAfterAdd) {
    sim->add_particle(Particle(0, 1.0));
    sim->add_particle(Particle(1, 1.0, {1,0,0}));
    EXPECT_EQ(sim->particles().size(), 2u);
}

TEST_F(SimTest, EnergyIsPositive) {
    sim->add_particle(Particle(0, 1.0, {0,0,0}, {1,0,0}));
    EXPECT_GT(sim->total_kinetic_energy(), 0.0);
}

TEST_F(SimTest, TimeAdvancesAfterStep) {
    sim->add_particle(Particle(0, 1.0));
    sim->add_particle(Particle(1, 1.0, {1,0,0}));
    double dt = 0.01;
    sim->step(dt);
    EXPECT_NEAR(sim->time(), dt, 1e-12);
}

TEST_F(SimTest, FixedParticleDoesNotMove) {
    Particle fixed(0, 1e30, {0,0,0});
    fixed.fixed = true;
    sim->add_particle(fixed);
    sim->add_particle(Particle(1, 1.0, {1e6,0,0}, {0,1e3,0}));
    sim->run(1.0, 0.01);
    EXPECT_DOUBLE_EQ(sim->particles()[0].position.x, 0.0);
}
