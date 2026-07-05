# physim

A C++ physics simulation engine for N-body and particle systems, with Python bindings via pybind11.

## Features

- **Force calculators** - gravitational (Newton), electrostatic (Coulomb), spring (Hooke)
- **Integrators** - forward Euler and Runge-Kutta 4
- **Collision detection** - sphere-sphere broadphase with elastic/inelastic resolution
- **Octree spatial partitioning** - O(n log n) collision broadphase for large systems
- **OpenMP parallelism** - thread-local force buffers for race-free parallel accumulation
- **Python bindings** - pybind11 wrapper exposing Simulation, Particle, Vector3
- **Energy diagnostics** - kinetic energy tracking per step

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DPHYSIM_BUILD_TESTS=ON -DPHYSIM_BUILD_EXAMPLES=ON
cmake --build build --parallel
ctest --test-dir build
```

## Quick start (C++)

```cpp
#include "simulation.h"
#include "forces.h"
#include "integrators.h"
using namespace physim;

auto sim = Simulation(std::make_unique<EulerIntegrator>());
sim.add_force(std::make_shared<GravitationalForce>());

Particle sun(0, 1.989e30); sun.fixed = true;
Particle earth(1, 5.972e24, {1.496e11, 0, 0}, {0, 2.978e4, 0});
sim.add_particle(sun);
sim.add_particle(earth);

sim.run(365.25 * 86400, 3600.0);
```

## Quick start (Python)

```python
import physim

sim = physim.Simulation(integrator="euler")
sim.add_gravitational_force()

sun   = physim.Particle(0, 1.989e30)
sun.fixed = True
earth = physim.Particle(1, 5.972e24,
                        physim.Vector3(1.496e11, 0, 0),
                        physim.Vector3(0, 2.978e4, 0))
sim.add_particle(sun)
sim.add_particle(earth)
sim.run(365.25 * 86400, 3600.0)
print(f"Earth position: {sim.particles()[1].position}")
```

## Running tests

```bash
ctest --test-dir build --output-on-failure
```

## Examples

- `examples/solar_system.cpp` - Earth and Mars orbiting the Sun (1 year)
- `examples/charged_particles.cpp` - 20 charged ions with Coulomb interaction

## Onboarding

See [CONTRIBUTING.md](CONTRIBUTING.md) for build setup and a guide to adding
custom force calculators, integrators, and Python bindings.
