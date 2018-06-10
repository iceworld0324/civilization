#ifndef EVENT_H_
#define EVENT_H_

#include "universe.h"
#include <initializer_list>
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

class ScienceAdvance : public Event {
public:
  ScienceAdvance(double timestamp, EventHandler *handler, int civilization_id)
      : Event(timestamp, handler), civilization_id_(civilization_id) {}

  std::string Print() const;

  int civilization_id() const { return civilization_id_; }

private:
  int civilization_id_;
};

class SelfBroadcast : public Event {
public:
  SelfBroadcast(double timestamp, EventHandler *handler, int civilization_id)
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
      : birth_distribution_(1.0 / birth_interval), generator_(generator),
        factory_(factory), death_handler_(death_handler) {}

  std::vector<std::unique_ptr<Event>>
  Handle(const std::unique_ptr<Event> &event, Universe *universe);

  CivilizationFactory *factory() const { return factory_; }

private:
  std::exponential_distribution<double> birth_distribution_;
  std::default_random_engine *generator_;
  CivilizationFactory *factory_;
  CivilizationDeathHandler *death_handler_;
};

class CivilizationDeathHandler : public EventHandler {
public:
  std::vector<std::unique_ptr<Event>>
  Handle(const std::unique_ptr<Event> &event, Universe *universe);
};

class ScienceAdvanceHandler : public EventHandler {
public:
  ScienceAdvanceHandler(double event_interval,
                        std::initializer_list<double> weights,
                        std::default_random_engine *generator)
      : event_rate_(1.0 / event_interval), level_distribution_(weights),
        generator_(generator) {}

  std::vector<std::unique_ptr<Event>>
  Handle(const std::unique_ptr<Event> &event, Universe *universe);

private:
  double event_rate_;
  std::discrete_distribution<int> level_distribution_;
  std::default_random_engine *generator_;
};

class SelfBroadcastHandler : public EventHandler {
public:
  SelfBroadcastHandler(double event_interval,
                       std::default_random_engine *generator)
      : event_rate_(1.0 / event_interval),
        send_threshold_distribution_(0.0, 1.0), generator_(generator) {}

  std::vector<std::unique_ptr<Event>>
  Handle(const std::unique_ptr<Event> &event, Universe *universe);

private:
  double event_rate_;
  std::uniform_real_distribution<double> send_threshold_distribution_;
  std::default_random_engine *generator_;
};

#endif
