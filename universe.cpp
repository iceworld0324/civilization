#include "universe.h"

Star StarFactory::Create() {
  double x = distribution_(*generator_);
  double y = distribution_(*generator_);
  double z = distribution_(*generator_);
  return Star(next_id_++, Location(x, y, z));
}

Civilization CivilizationFactory::Create(const Universe &universe) {
  const auto &stars = universe.stars();
  std::uniform_int_distribution<int> star_id_distribution(
      stars.begin()->first, std::prev(stars.end())->first);
  std::map<int, Star>::const_iterator star_it;
  do {
    int star_id = star_id_distribution(*generator_);
    star_it = stars.find(star_id);
  } while (star_it == stars.end() || star_it->second.resident() != -1);

  double lifespan = lifespan_distribution_(*generator_);
  return Civilization(next_id_++, star_it->first, lifespan);
}
