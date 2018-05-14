#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <random>
#include <vector>
#include "universe.h"

class CivilizationFactory {
 public:
  CivilizationFactory() : next_id_(0) {}

  Civilization Create(const Universe& universe);

  int next_id() const { return next_id_; }

 private:
  int next_id_;
  std::default_random_engine generator_;
};

class Event {
 public:
  Event(double timestamp) : timestamp_(timestamp) {}

  virtual std::vector<Event> Process(Universe* universe) {
    return std::vector<Event>();
  }

  double timestamp() const { return timestamp_; }

 protected:
  Universe* universe_;
  double timestamp_;
};

class CompareEvents {
 public:
  bool operator() (const Event& a, const Event& b) {
    return a.timestamp() < b.timestamp();
  }
};

class CivilizationBirth : public Event {
 public:
  CivilizationBirth(double timestamp, CivilizationFactory* factory)
    : Event(timestamp), factory_(factory) {}

  std::vector<Event> Process(Universe* universe) override;

  int next_id() const { return factory_->next_id(); }

 private:
  CivilizationFactory* factory_;
};

class CivilizationDeath : public Event {
 public:
  CivilizationDeath(double timestamp, int civilization_id)
    : Event(timestamp), civilization_id_(civilization_id) {}

  std::vector<Event> Process(Universe* universe) override;

  int civilization_id() const { return civilization_id_; }

 private:
  int civilization_id_;
};

std::ostream& operator<<(std::ostream& os, const Event& event);
std::ostream& operator<<(std::ostream& os, const CivilizationBirth& event);
std::ostream& operator<<(std::ostream& os, const CivilizationDeath& event);

#endif
