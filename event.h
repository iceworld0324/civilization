#ifndef EVENT_H_
#define EVENT_H_

#include "universe.h"
#include <memory>
#include <random>
#include <string>
#include <vector>

class EventHandler;
class CivilizationBirthHandler;
class CivilizationDeathHandler;

class Event {
public:
  Event(double timestamp, EventHandler *handler)
      : timestamp_(timestamp), handler_(handler) {}

  virtual std::string Print() const;

  double timestamp() const { return timestamp_; }
  EventHandler *handler() const { return handler_; }

protected:
  double timestamp_;
  EventHandler *handler_;
};

class CompareEvents {
public:
  bool operator()(const std::unique_ptr<Event> &a,
                  const std::unique_ptr<Event> &b) {
    return a->timestamp() > b->timestamp();
  }
};

class CivilizationBirth : public Event {
public:
  CivilizationBirth(double timestamp, EventHandler *handler)
      : Event(timestamp, handler) {}

  std::string Print() const;
};

class CivilizationDeath : public Event {
public:
  CivilizationDeath(double timestamp, EventHandler *handler,
                    int civilization_id)
      : Event(timestamp, handler), civilization_id_(civilization_id) {}

  std::string Print() const;

  int civilization_id() const { return civilization_id_; }

private:
  int civilization_id_;
};

class EventHandler {
public:
  virtual std::vector<std::unique_ptr<Event>>
  Handle(const std::unique_ptr<Event> &event, Universe *universe) = 0;
};

class CivilizationBirthHandler : public EventHandler {
public:
  CivilizationBirthHandler(double birth_interval,
                           std::default_random_engine *generator,
                           CivilizationFactory *factory,
                           CivilizationDeathHandler *death_handler)
      : distribution_(1.0 / birth_interval), generator_(generator),
        factory_(factory), death_handler_(death_handler) {}

  std::vector<std::unique_ptr<Event>>
  Handle(const std::unique_ptr<Event> &event, Universe *universe);

  CivilizationFactory *factory() const { return factory_; }

private:
  std::exponential_distribution<double> distribution_;
  std::default_random_engine *generator_;
  CivilizationFactory *factory_;
  CivilizationDeathHandler *death_handler_;
};

class CivilizationDeathHandler : public EventHandler {
public:
  std::vector<std::unique_ptr<Event>>
  Handle(const std::unique_ptr<Event> &event, Universe *universe);
};

#endif
