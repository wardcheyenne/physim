#pragma once
#include "particle.h"
#include "forces.h"
#include "integrators.h"
#include "collision.h"
#include <vector>
#include <memory>
#include <functional>
#include <string>

namespace physim {

struct SimulationStats {
    double total_kinetic_energy;
    double elapsed_time;
    size_t step_count;
    size_t particle_count;
    size_t collision_count;
};

class Simulation {
public:
    using StepCallback = std::function<void(const std::vector<Particle>&, double)>;

    /**
    This is a function to construct a Simulation using a given integration scheme.
    Input(s):
    - integrator: the Integrator implementation used to advance particles each step
    Output(s):
    - None: initializes the simulation with the given integrator and empty particle/force lists
    **/
    explicit Simulation(std::unique_ptr<Integrator> integrator);

    /**
    This is a function to add a particle to the simulation.
    Input(s):
    - p: the particle to add
    Output(s):
    - None: appends p to the simulation's particle list
    **/
    void add_particle(Particle p);
    /**
    This is a function to register a force calculator that will be applied every step.
    Input(s):
    - f: the shared ForceCalculator to add
    Output(s):
    - None: appends f to the simulation's list of active forces
    **/
    void add_force(std::shared_ptr<ForceCalculator> f);
    /**
    This is a function to set the collision detector used to find collisions each step.
    Input(s):
    - cd: the shared CollisionDetector to use
    Output(s):
    - None: stores cd as the simulation's active collision detector
    **/
    void set_collision_detector(std::shared_ptr<CollisionDetector> cd);
    /**
    This is a function to set the collision resolver used to respond to detected collisions.
    Input(s):
    - cr: the shared CollisionResolver to use
    Output(s):
    - None: stores cr as the simulation's active collision resolver
    **/
    void set_collision_resolver(std::shared_ptr<CollisionResolver> cr);

    /**
    This is a function to run the simulation for a total duration, advancing by fixed timesteps and optionally invoking a callback after each step.
    Input(s):
    - total_time: the total simulation time to run
    - dt: the fixed timestep size
    - on_step: optional callback invoked with the current particles and elapsed time after each step
    Output(s):
    - None: advances the simulation's internal state until total_time has elapsed
    **/
    void run(double total_time, double dt, StepCallback on_step = nullptr);
    /**
    This is a function to advance the simulation by a single timestep: accumulate forces, integrate, and handle collisions.
    Input(s):
    - dt: the timestep size
    Output(s):
    - None: mutates particle positions/velocities and advances the simulation's time and step count
    **/
    void step(double dt);
    /**
    This is a function to export the current simulation state to a JSON file.
    Input(s):
    - path: filesystem path to write the JSON output to
    Output(s):
    - None: writes the simulation's particle data to the given file
    **/
    void export_json(const std::string& path) const;

    /**
    This is a function to access the simulation's current particles.
    Input(s):
    - None
    Output(s):
    - const std::vector<Particle>&: reference to the internal particle list
    **/
    const std::vector<Particle>& particles() const { return particles_; }
    /**
    This is a function to compute summary statistics for the current simulation state.
    Input(s):
    - None
    Output(s):
    - SimulationStats: total kinetic energy, elapsed time, step count, particle count, and collision count
    **/
    SimulationStats stats() const;
    /**
    This is a function to compute the total kinetic energy of all particles in the simulation.
    Input(s):
    - None
    Output(s):
    - double: the sum of each particle's kinetic energy
    **/
    double total_kinetic_energy() const;
    /**
    This is a function to report the current simulation time.
    Input(s):
    - None
    Output(s):
    - double: the elapsed simulation time
    **/
    double time() const { return time_; }
    /**
    This is a function to reset the simulation to its initial empty state.
    Input(s):
    - None
    Output(s):
    - None: clears particles and resets time, step count, and collision count to zero
    **/
    void reset();

private:
    std::vector<Particle>                         particles_;
    std::vector<std::shared_ptr<ForceCalculator>> forces_;
    std::unique_ptr<Integrator>                   integrator_;
    std::shared_ptr<CollisionDetector>            collider_;
    std::shared_ptr<CollisionResolver>            resolver_;
    double time_            = 0.0;
    size_t steps_           = 0;
    size_t collision_count_ = 0;

    /**
    This is a function to reset every particle's force to zero, then accumulate all registered forces across all particle pairs.
    Input(s):
    - None
    Output(s):
    - None: mutates each particle's force member
    **/
    void accumulate_forces();
    /**
    This is a function to detect and resolve collisions among the current particles using the configured detector and resolver.
    Input(s):
    - None
    Output(s):
    - None: mutates colliding particles' velocities/positions and increments the collision count
    **/
    void handle_collisions();
};

} //namespace physim
