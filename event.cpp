#include "event.h"

#include <map>

Civilization CivilizationFactory::Create(const Universe& universe) {
  const std::map<int, Star>& stars = universe.stars();
  std::uniform_int_distribution<int> distribution(
      stars.begin()->first, std::prev(stars.end())->first);
  int star_id;
  std::map<int, Star>::const_iterator star_it;
  do {
    star_id = distribution(generator_);
    star_it = stars.find(star_id);
  } while (star_it == stars.end() || star_it->second.resident() != -1);
  return Civilization(next_id_++, star_id);
}

std::vector<Event> CivilizationBirth::Process(Universe* universe) {
  Civilization civilization = factory_->Create(*universe);
  universe->mutable_civilizations()
      ->insert({civilization.id(), civilization});
  universe->mutable_stars()
      ->at(civilization.residence()).set_resident(civilization.id());
  CivilizationDeath this_death(timestamp_ + 5.0, civilization.id());
  CivilizationBirth next_birth(timestamp_ + 10.0, factory_);
  return {static_cast<Event>(this_death), static_cast<Event>(next_birth)};
}

std::vector<Event> CivilizationDeath::Process(Universe* universe) {
  int star_id = universe->civilizations().at(civilization_id_).residence();
  universe->mutable_stars()->at(star_id).set_resident(-1);
  universe->mutable_civilizations() ->erase(civilization_id_);
  return {};
}

std::ostream& operator<<(std::ostream& os, const Event& event) {
  os << "Year: " << event.timestamp() << "\nEvent: ";
  return os;
}

std::ostream& operator<<(std::ostream& os, const CivilizationBirth& event) {
  os << static_cast<const Event&>(event) << "CivilizationBirth, id: "
     << event.next_id();
  return os;
}

std::ostream& operator<<(std::ostream& os, const CivilizationDeath& event) {
  os << static_cast<const Event&>(event) << "CivilizationDeath, id: "
     << event.civilization_id();
  return os;
}
