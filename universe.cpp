#include "universe.h"

#include <random>

void Universe::CreateStars(int num_stars, double radius_universe) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(
      -radius_universe, radius_universe);
  for (int id = 0; id < num_stars; id++) {
    double x = distribution(generator);
    double y = distribution(generator);
    double z = distribution(generator);
    stars_.insert({id, Star(id, Location(x, y, z))});
  }
}
