# Changelog

## [Unreleased]

## [0.1.0] - 2026-07-04

### Added
- `Vector3` - 3D math library (dot, cross, norm, normalized)
- `Particle` - mass, charge, radius, position, velocity, force, fixed flag
- Force calculators: `GravitationalForce`, `ElectrostaticForce`, `SpringForce`
- Integrators: `EulerIntegrator`, `RK4Integrator`
- `Simulation` - step loop, force accumulation, collision handling, energy diagnostic
- `SphereCollisionDetector` and `ElasticCollisionResolver`
- `Octree` spatial partitioning (broadphase)
- OpenMP parallel force accumulation with thread-local buffers
- Python bindings via pybind11
- Google Test unit tests for `Vector3` and `Simulation`
- GitHub Actions CI with CMake + ctest
- Examples: `solar_system`, `charged_particles`

### Fixed
- `Vector3::normalized()` zero-vector guard
- Force accumulation: reset forces before pairwise loop
- RK4: clamp large timesteps to prevent divergence
- OpenMP race condition: thread-local force buffers
- Collision resolver: use event normal, not recomputed direction
- Octree: pass depth to child nodes to enforce `MAX_DEPTH`
- pybind11: return copies of particle list to prevent dangling references
- Compiler warnings: default constructors, `noexcept`
