#include "event.h"

#include <iomanip>
#include <sstream>

std::string Event::Print() const {
  std::stringstream stream;
  stream << "[Year] " << std::fixed << std::setprecision(1) << timestamp_
         << "\t[Event] ";
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

std::string ScienceAdvance::Print() const {
  std::stringstream stream;
  stream << Event::Print() << "ScienceAdvance, id: " << civilization_id_;
  return stream.str();
}

std::string SelfBroadcast::Print() const {
  std::stringstream stream;
  stream << Event::Print() << "SelfBroadcast, id: " << civilization_id_;
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
      event->timestamp() + birth_distribution_(*generator_), this));
  return following_events;
}

std::vector<std::unique_ptr<Event>>
CivilizationDeathHandler::Handle(const std::unique_ptr<Event> &event,
                                 Universe *universe) {
  int civilization_id =
      dynamic_cast<CivilizationDeath *>(event.get())->civilization_id();
  const auto &civilizations = universe->civilizations();
  auto civilization_it = civilizations.find(civilization_id);
  if (civilization_it != civilizations.end()) {
    int star_id = civilization_it->second.residence();
    universe->mutable_stars()->at(star_id).set_resident(-1);
    universe->mutable_civilizations()->erase(civilization_id);
  }
  return {};
}

std::vector<std::unique_ptr<Event>>
ScienceAdvanceHandler::Handle(const std::unique_ptr<Event> &event,
                              Universe *universe) {
  auto *civilizations = universe->mutable_civilizations();
  int civilization_id =
      dynamic_cast<ScienceAdvance *>(event.get())->civilization_id();
  auto civilization_it =
      civilizations->find(civilization_id);
  if (civilization_it != civilizations->end()) {
    int level = level_distribution_(*generator_);
    civilization_it->second.advance_science(level);
  }

  int next_id = universe->RandomCivilization();
  double interval = 1.0 / event_rate_;
  if (!civilizations->empty()) {
    std::exponential_distribution<double> interval_distribution(
        event_rate_ * civilizations->size());
    interval = interval_distribution(*generator_);
  }
  std::vector<std::unique_ptr<Event>> following_events;
  following_events.emplace_back(new ScienceAdvance(
      event->timestamp() + interval, this, next_id));
  return following_events;
}

std::vector<std::unique_ptr<Event>>
SelfBroadcastHandler::Handle(const std::unique_ptr<Event> &event,
                              Universe *universe) {
  std::vector<std::unique_ptr<Event>> following_events;
  const auto &civilizations = universe->civilizations();
  int civilization_id =
      dynamic_cast<SelfBroadcast *>(event.get())->civilization_id();
  auto civilization_it =
      civilizations.find(civilization_id);
  if (civilization_it != civilizations.end()) {
    double send_threshold = send_threshold_distribution_(*generator_);
    if (send_threshold < civilization_it->second.extroversion()) {
      // TODO: Create a Broadcast and all the ReceiveBroadcast events.
    }
  }

  int next_id = universe->RandomCivilization();
  double interval = 1.0 / event_rate_;
  if (!civilizations.empty()) {
    std::exponential_distribution<double> interval_distribution(
        event_rate_ * civilizations.size());
    interval = interval_distribution(*generator_);
  }
  following_events.emplace_back(new SelfBroadcast(
      event->timestamp() + interval, this, next_id));
  return following_events;
}
