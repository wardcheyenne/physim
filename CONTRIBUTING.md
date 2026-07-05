# Contributing to physim

## Build requirements

- CMake >= 3.20
- C++20 compiler (GCC 12+, Clang 15+, MSVC 2022+)
- Optional: OpenMP (for parallel force calculation)
- Optional: pybind11 (for Python bindings)

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DPHYSIM_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build
```

## Adding a force calculator

Subclass `ForceCalculator` and implement `accumulate(Particle& a, Particle& b)`.
Use Newton's 3rd law: apply equal and opposite forces. Check `fixed` before modifying velocity.

```cpp
class MyForce : public ForceCalculator {
public:
    void accumulate(Particle& a, Particle& b) const override {
        // compute force f on a due to b
        if (!a.fixed) a.force += f;
        if (!b.fixed) b.force -= f;
    }
};
```

## Adding an integrator

Subclass `Integrator` and implement `step(particles, dt)`.
Do not call `accumulate_forces()` inside the integrator - the Simulation does that.

## Python bindings

```bash
cmake -B build -DPHYSIM_PYTHON=ON
cmake --build build
python -c "import physim; print(physim.__doc__)"
```

## Running the examples

```bash
./build/examples/solar_system
./build/examples/charged_particles
```
