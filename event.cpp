#include "event.h"

#include <map>

Civilization CivilizationFactory::Create() {
  const std::map<int, Star>& stars = universe_->stars();
  std::uniform_int_distribution<int> distribution(
      stars->begin()->first, std::prev(stars->end())->first);
  int star_id;
  do {
    star_id = distribution(generator_);
    auto star_it = stars->find(star_id);
  } while (star_it == stars->end() || star_it->second.resident() != -1);
  return Civilization(next_id_++, star_id);
}

std::vector<Event> CivilizationBirth::Process(Universe* universe) {
  Civilization civilization = factory_->Create();
  universe->mutable_civilizations()
      ->insert({civilization.id(), civilization});
  universe->mutable_stars()
      ->at(civilization.residence()).set_resident(civilization.id());
  CivilizationDeath this_death(timestamp_ + 5.0, civilization.id());
  CivilizationBirth next_birth(timestamp_ + 10.0, factory_);
  return {this_death, next_birth};
}

std::vector<Event> CivilizationDeath::Process(Universe* universe) {
  int star_id = universe->civilizations().at(civilization_id_).residence();
  universe->mutable_stars()->at(star_id).set_resident(-1);
  universe->mutable_civilizations() ->erase(civilization_id_);
  return {};
}
