#include <random>
#include <vector>
#include "universe.h"

class CivilizationFactory {
 public:
  CivilizationFactory(Universe* universe) : next_id_(0), universe_(universe) {}
  Civilization Create();

 private:
  int next_id_;
  Universe* universe_;
  std::default_random_engine generator_;
};

class Event {
 public:
  Event(double timestamp) : timestamp_(timestamp) {}

  virtual std::vector<Event> Process(Universe* universe) {}

  double timestamp() { return timestamp_; }

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

 private:
  CivilizationFactory* factory_;
};

class CivilizationDeath : public Event {
 public:
  CivilizationDeath(double timestamp, int civilization_id)
    : Event(timestamp), civilization_id_(civilization_id) {}
  std::vector<Event> Process(Universe* universe) override;

 private:
  int civilization_id_;
};
