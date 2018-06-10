#include "universe.h"

#include <cstdlib>
#include <iostream>

int Universe::RandomCivilization() const {
  int id = -1;
  if (!civilizations_.empty()) {
    std::uniform_int_distribution<int> id_distribution(
        civilizations_.begin()->first, std::prev(civilizations_.end())->first);
    do {
      id = id_distribution(*generator_);
    } while (civilizations_.find(id) == civilizations_.end());
  }
  return id;
}

Star StarFactory::Create() {
  double x = distribution_(*generator_);
  double y = distribution_(*generator_);
  double z = distribution_(*generator_);
  return Star(next_id_++, Location(x, y, z));
}

Civilization CivilizationFactory::Create(const Universe &universe) {
  const auto &stars = universe.stars();
  if (stars.empty()) {
    std::cout << "[Error] Universe has no star." << std::endl;
    exit(EXIT_FAILURE);
  }
  double density = (double)universe.civilizations().size() / stars.size(); 
  if (density > 0.95) {
    std::cout << "[Error] Civilization density is too high: " << density << std::endl;
    exit(EXIT_FAILURE);
  }

  std::uniform_int_distribution<int> star_id_distribution(
      stars.begin()->first, std::prev(stars.end())->first);
  std::map<int, Star>::const_iterator star_it;
  do {
    int star_id = star_id_distribution(*generator_);
    star_it = stars.find(star_id);
  } while (star_it == stars.end() || star_it->second.resident() != -1);

  double lifespan = lifespan_distribution_(*generator_);
  double extroversion = extroversion_distribution_(*generator_);
  return Civilization(next_id_++, star_it->first, lifespan, extroversion);
}
