#include "event.h"

#include <iomanip>
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

std::string Event::Print() {
  std::stringstream stream;
  stream << "Year: " << std::fixed << std::setprecision(1) << timestamp_
         << "\tEvent: ";
  return stream.str();
}

std::vector<std::unique_ptr<Event>> CivilizationBirth::Process(
    Universe* universe) {
  Civilization civilization = factory_->Create(*universe);
  universe->mutable_civilizations()
      ->insert({civilization.id(), civilization});
  universe->mutable_stars()
      ->at(civilization.residence()).set_resident(civilization.id());
  std::vector<std::unique_ptr<Event>> following_events;
  following_events.emplace_back(new CivilizationDeath(
      timestamp_ + 5.0, civilization.id()));
  following_events.emplace_back(new CivilizationBirth(
      timestamp_ + 10.0, factory_));
  return following_events;
}

std::string CivilizationBirth::Print() {
  std::stringstream stream;
  stream << Event::Print() << "CivilizationBirth, id: " << factory_->next_id();
  return stream.str();
}

std::vector<std::unique_ptr<Event>> CivilizationDeath::Process(Universe* universe) {
  int star_id = universe->civilizations().at(civilization_id_).residence();
  universe->mutable_stars()->at(star_id).set_resident(-1);
  universe->mutable_civilizations()->erase(civilization_id_);
  return {};
}

std::string CivilizationDeath::Print() {
  std::stringstream stream;
  stream << Event::Print() << "CivilizationDeath, id: " << civilization_id_;
  return stream.str();
}
