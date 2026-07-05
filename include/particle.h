#pragma once
#include "vector3.h"
#include <string>
#include <cstdint>

namespace physim {

struct Particle {
    std::uint64_t id     = 0;
    double        mass   = 1.0;
    double        charge = 0.0;
    double        radius = 1.0;
    Vector3       position{};
    Vector3       velocity{};
    Vector3       force{};
    bool          fixed  = false;
    std::string   label;

    Particle() = default;

    /**
    This is a function to construct a Particle with an id, mass, and optional position, velocity, charge, and radius.
    Input(s):
    - id_: unique identifier for the particle
    - mass_: the particle's mass
    - pos: initial position vector (defaults to the zero vector)
    - vel: initial velocity vector (defaults to the zero vector)
    - charge_: the particle's electric charge (default 0.0)
    - radius_: the particle's radius (default 1.0)
    Output(s):
    - None: initializes the particle's fields, with force set to zero and fixed set to false
    **/
    Particle(std::uint64_t id_, double mass_,
             Vector3 pos = Vector3::zero(), Vector3 vel = Vector3::zero(),
             double charge_ = 0.0, double radius_ = 1.0)
        : id(id_), mass(mass_), charge(charge_), radius(radius_),
          position(pos), velocity(vel), force(Vector3::zero()), fixed(false) {}

    /**
    This is a function to reset the accumulated force on the particle to zero.
    Input(s):
    - None
    Output(s):
    - None: sets the particle's force member to the zero vector
    **/
    void reset_force()     noexcept { force = Vector3::zero(); }
    /**
    This is a function to compute the particle's kinetic energy.
    Input(s):
    - None
    Output(s):
    - double: 0.5 * mass * (velocity squared magnitude)
    **/
    double kinetic_energy() const   { return 0.5 * mass * velocity.norm2(); }
    /**
    This is a function to compute the particle's linear momentum.
    Input(s):
    - None
    Output(s):
    - Vector3: the velocity vector scaled by mass
    **/
    Vector3 momentum()      const   { return velocity * mass; }
};

} //namespace physim
