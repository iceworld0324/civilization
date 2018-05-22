#include "event.h"

#include <iomanip>
#include <sstream>

std::string Event::Print() const {
  std::stringstream stream;
  stream << "Year: " << std::fixed << std::setprecision(1) << timestamp_
         << "\tEvent: ";
  return stream.str();
}

std::string CivilizationBirth::Print() const {
  std::stringstream stream;
  stream << Event::Print() << "CivilizationBirth, id: "
         << dynamic_cast<CivilizationBirthHandler *>(handler_)
                ->factory()
                ->next_id();
  return stream.str();
}

std::string CivilizationDeath::Print() const {
  std::stringstream stream;
  stream << Event::Print() << "CivilizationDeath, id: " << civilization_id_;
  return stream.str();
}

std::vector<std::unique_ptr<Event>>
CivilizationBirthHandler::Handle(const std::unique_ptr<Event> &event,
                                 Universe *universe) {
  Civilization civilization = factory_->Create(*universe);
  universe->mutable_civilizations()->insert({civilization.id(), civilization});
  universe->mutable_stars()
      ->at(civilization.residence())
      .set_resident(civilization.id());

  std::vector<std::unique_ptr<Event>> following_events;
  following_events.emplace_back(
      new CivilizationDeath(event->timestamp() + civilization.lifespan(),
                            death_handler_, civilization.id()));
  following_events.emplace_back(new CivilizationBirth(
      event->timestamp() + distribution_(*generator_), this));
  return following_events;
}

std::vector<std::unique_ptr<Event>>
CivilizationDeathHandler::Handle(const std::unique_ptr<Event> &event,
                                 Universe *universe) {
  int civilization_id =
      dynamic_cast<CivilizationDeath *>(event.get())->civilization_id();
  int star_id = universe->civilizations().at(civilization_id).residence();
  universe->mutable_stars()->at(star_id).set_resident(-1);
  universe->mutable_civilizations()->erase(civilization_id);
  return {};
}
