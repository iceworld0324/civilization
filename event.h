#ifndef EVENT_H_
#define EVENT_H_

#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
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

  virtual std::vector<std::unique_ptr<Event>> Process(
      Universe* universe) = 0;

  virtual std::string Print();

  double timestamp() const { return timestamp_; }

 protected:
  Universe* universe_;
  double timestamp_;
};

class CompareEvents {
 public:
  bool operator() (const std::unique_ptr<Event>& a,
      const std::unique_ptr<Event>& b) {
    return a->timestamp() > b->timestamp();
  }
};

class CivilizationBirth : public Event {
 public:
  CivilizationBirth(double timestamp, CivilizationFactory* factory)
    : Event(timestamp), factory_(factory) {}

  std::vector<std::unique_ptr<Event>> Process(Universe* universe);

  std::string Print();

 private:
  CivilizationFactory* factory_;
};

class CivilizationDeath : public Event {
 public:
  CivilizationDeath(double timestamp, int civilization_id)
    : Event(timestamp), civilization_id_(civilization_id) {}

  std::vector<std::unique_ptr<Event>> Process(Universe* universe);

  std::string Print();

 private:
  int civilization_id_;
};

#endif
