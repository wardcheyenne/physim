#include "simulation.h"
#ifdef PHYSIM_OPENMP
#include <omp.h>
#endif
#include <vector>
#include <fstream>
#include <iomanip>

namespace physim {

/**
This is a function to construct a Simulation using a given integration scheme.
Input(s):
- integrator: the Integrator implementation used to advance particles each step
Output(s):
- None: initializes the simulation's integrator, taking ownership of it
**/
Simulation::Simulation(std::unique_ptr<Integrator> integrator)
    : integrator_(std::move(integrator)) {}

/**
This is a function to add a particle to the simulation.
Input(s):
- p: the particle to add
Output(s):
- None: appends p to the simulation's particle list
**/
void Simulation::add_particle(Particle p)                       { particles_.push_back(std::move(p)); }
/**
This is a function to register a force calculator that will be applied every step.
Input(s):
- f: the shared ForceCalculator to add
Output(s):
- None: appends f to the simulation's list of active forces
**/
void Simulation::add_force(std::shared_ptr<ForceCalculator> f) { forces_.push_back(std::move(f)); }
/**
This is a function to set the collision detector used to find collisions each step.
Input(s):
- cd: the shared CollisionDetector to use
Output(s):
- None: stores cd as the simulation's active collision detector
**/
void Simulation::set_collision_detector(std::shared_ptr<CollisionDetector> cd) { collider_ = std::move(cd); }
/**
This is a function to set the collision resolver used to respond to detected collisions.
Input(s):
- cr: the shared CollisionResolver to use
Output(s):
- None: stores cr as the simulation's active collision resolver
**/
void Simulation::set_collision_resolver(std::shared_ptr<CollisionResolver> cr) { resolver_ = std::move(cr); }

/**
This is a function to reset every particle's force to zero, then accumulate all registered forces across all particle pairs, using OpenMP parallelism when enabled.
Input(s):
- None
Output(s):
- None: mutates each particle's force member
**/
void Simulation::accumulate_forces() {
    const size_t n = particles_.size();
    for (auto& p : particles_) p.reset_force();
#ifdef PHYSIM_OPENMP
    int nth = omp_get_max_threads();
    std::vector<std::vector<Vector3>> lf(nth, std::vector<Vector3>(n));
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        #pragma omp for schedule(dynamic, 4)
        for (size_t i = 0; i < n; ++i) {
            if (particles_[i].fixed) continue;
            for (size_t j = i + 1; j < n; ++j) {
                for (auto& fc : forces_) {
                    Particle a = particles_[i], b = particles_[j];
                    fc->accumulate(a, b);
                    lf[tid][i] += a.force - particles_[i].force;
                    lf[tid][j] += b.force - particles_[j].force;
                }
            }
        }
    }
    for (int t = 0; t < nth; ++t)
        for (size_t i = 0; i < n; ++i)
            particles_[i].force += lf[t][i];
#else
    for (size_t i = 0; i < n; ++i) {
        if (particles_[i].fixed) continue;
        for (size_t j = i + 1; j < n; ++j)
            for (auto& fc : forces_) fc->accumulate(particles_[i], particles_[j]);
    }
#endif
}

/**
This is a function to detect and resolve collisions among the current particles using the configured detector and resolver.
Input(s):
- None
Output(s):
- None: mutates colliding particles' velocities/positions and increments the collision count; does nothing if no detector/resolver is set
**/
void Simulation::handle_collisions() {
    if (!collider_ || !resolver_) return;
    auto events = collider_->detect(particles_, time_);
    collision_count_ += events.size();
    for (auto& ev : events) resolver_->resolve(particles_[ev.i], particles_[ev.j], ev);
}

/**
This is a function to advance the simulation by a single timestep: accumulate forces, integrate, and handle collisions.
Input(s):
- dt: the timestep size
Output(s):
- None: mutates particle positions/velocities and advances the simulation's time and step count
**/
void Simulation::step(double dt) {
    accumulate_forces();
    integrator_->step(particles_, dt);
    handle_collisions();
    time_  += dt;
    steps_ += 1;
}

/**
This is a function to run the simulation for a total duration, advancing by fixed timesteps and optionally invoking a callback after each step.
Input(s):
- total_time: the total simulation time to run
- dt: the fixed timestep size
- on_step: optional callback invoked with the current particles and elapsed time after each step
Output(s):
- None: advances the simulation's internal state until total_time has elapsed
**/
void Simulation::run(double total_time, double dt, StepCallback on_step) {
    double t = 0.0;
    while (t < total_time - 1e-12) {
        step(dt); t += dt;
        if (on_step) on_step(particles_, time_);
    }
}

/**
This is a function to export the current simulation state to a JSON file.
Input(s):
- path: filesystem path to write the JSON output to
Output(s):
- None: writes each particle's id, mass, position, velocity, and kinetic energy as JSON to the given file
**/
void Simulation::export_json(const std::string& path) const {
    std::ofstream f(path);
    f << std::fixed << std::setprecision(6);
    f << "{\"time\":" << time_ << ",\"particles\":[";
    for (size_t i = 0; i < particles_.size(); ++i) {
        const auto& p = particles_[i];
        if (i) f << ",";
        f << "{\"id\":" << p.id << ",\"mass\":" << p.mass
          << ",\"pos\":[" << p.position.x << "," << p.position.y << "," << p.position.z << "]"
          << ",\"vel\":[" << p.velocity.x << "," << p.velocity.y << "," << p.velocity.z << "]"
          << ",\"ke\":"   << p.kinetic_energy() << "}";
    }
    f << "]}";
}

/**
This is a function to compute the total kinetic energy of all particles in the simulation.
Input(s):
- None
Output(s):
- double: the sum of each particle's kinetic energy
**/
double Simulation::total_kinetic_energy() const {
    double ke = 0;
    for (const auto& p : particles_) ke += p.kinetic_energy();
    return ke;
}

/**
This is a function to compute summary statistics for the current simulation state.
Input(s):
- None
Output(s):
- SimulationStats: total kinetic energy, elapsed time, step count, particle count, and collision count
**/
SimulationStats Simulation::stats() const {
    return { total_kinetic_energy(), time_, steps_, particles_.size(), collision_count_ };
}

/**
This is a function to reset the simulation to its initial empty state.
Input(s):
- None
Output(s):
- None: zeroes every particle's force and velocity, and resets time, step count, and collision count to zero
**/
void Simulation::reset() {
    for (auto& p : particles_) { p.force = p.velocity = Vector3::zero(); }
    time_ = 0.0; steps_ = 0; collision_count_ = 0;
}

} //namespace physim
