#include <vector>
#include <iostream>
#include <cstdint>
#include <array>

// The number of motion simulation steps.
constexpr uint32_t STEPS = 10000;
// The number of paticles to simulate.
constexpr uint32_t PARTICLES = 1000;

struct Particle {
  float x;
  float y;
  float velocity;
};

// Initialize the particles with random coordinates and velocities.
std::vector<Particle> initParticles();

// Medium-quality random number generator.
// https://www.javamex.com/tutorials/random_numbers/xorshift.shtml
struct XorShift32 {
  uint32_t val;
  XorShift32 (uint32_t seed) : val(seed) {}
public:
  uint32_t gen() {
    val ^= (val << 13);
    val ^= (val >> 17);
    val ^= (val << 5);
    return val;
  }
};

constexpr double PI_D = 3.141592653589793238463;
constexpr float PI_F = 3.14159265358979f;

// Approximate sine and cosine functions
// https://stackoverflow.com/questions/18662261/fastest-implementation-of-sine-cosine-and-square-root-in-c-doesnt-need-to-b
static float sine(float x) {
    const float B = 4 / PI_F;
    const float C = -4/( PI_F * PI_F);
    return B * x + C * x * std::abs(x);
}
static float cosine(float x) {
    return sine(x + (PI_F / 2));
}

// A constant to convert from degrees to radians.
// It maps the random number from [0;UINT32_MAX) to [0;2*pi).
// We do calculations in double precision then convert to float.
constexpr float DEGREE_TO_RADIAN = (2 * PI_D) / UINT32_MAX;

// Simulate the motion of the particles.
// For every particle, we generate a random angle and move the particle
// in the corresponding direction.
template <class RNG>
void randomParticleMotion(std::vector<Particle> &particles, uint32_t seed) {
  RNG rng1(seed);
  RNG rng2(seed);
  RNG rng3(seed);
  RNG rng4(seed);
  for (int i = 0; i < STEPS; i++) {
    for (size_t j = 0; j < particles.size(); j += 4) {
      Particle& p0 = particles[j];
      uint32_t angle = rng1.gen();
      float angle_rad = angle * DEGREE_TO_RADIAN;
      p0.x += cosine(angle_rad) * p0.velocity;
      p0.y += sine(angle_rad) * p0.velocity;

      Particle& p1 = particles[j + 1];
      angle = rng2.gen();
      angle_rad = angle * DEGREE_TO_RADIAN;
      p1.x += cosine(angle_rad) * p1.velocity;
      p1.y += sine(angle_rad) * p1.velocity;      

      Particle& p2 = particles[j + 2];
      angle = rng3.gen();
      angle_rad = angle * DEGREE_TO_RADIAN;
      p2.x += cosine(angle_rad) * p2.velocity;
      p2.y += sine(angle_rad) * p2.velocity;      
      
      Particle& p3 = particles[j + 3];
      angle = rng4.gen();
      angle_rad = angle * DEGREE_TO_RADIAN;
      p3.x += cosine(angle_rad) * p3.velocity;
      p3.y += sine(angle_rad) * p3.velocity;
    }
  }
}