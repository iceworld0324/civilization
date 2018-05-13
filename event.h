#include "universe.h"

class CivilizationFactory {
 public:
  CivilizationFactory(Universe* universe) : next_id_(0), universe_(universe) {}
  Civilization Create();

 private:
  int next_id_;
  Universe* universe_;
};

class Event {
 public:
  Event(Universe* universe, double timestamp)
    : universe_(universe), timestamp_(timestamp) {}
  virtual void Process() {}

 protected:
  Universe* universe_;
  double timestamp_;
};

class CivilizationBirth : public Event {
 public:
  CivilizationBirth(Universe* universe, double timestamp, CivilizationFactory* factory)
    : Event(universe), factory_(factory) {}
  void Process() override;

 private:
  CivilizationFactory* factory_;
};

class CivilizationDeath : public Event {
 public:
  CivilizationDeath(Universe* universe, double timestamp, int civilization_id)
    : Event(universe), civilization_id_(civilization_id) {}
  void Process() override;

 private:
  int civilization_id_;
};
