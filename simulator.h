#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <memory>
#include <queue>
#include <string>
#include "event.h"
#include "universe.h"

class Config {
 public:
  Config(const std::string& config_filename);

  double end_time() const { return end_time_; }
  int num_stars() const { return num_stars_; }
  double radius_universe() const { return radius_universe_; }

 private:
  double end_time_;
  int num_stars_;
  double radius_universe_;
};

class Simulator {
 public:
  Simulator(const std::string& config_filename)
    : time_(0.0),
      config_(config_filename),
      universe_(config_.num_stars(), config_.radius_universe()),
      factory_() {
    events_.emplace(new CivilizationBirth(0.0, &factory_));
  }

  void Run();

 private:
  double time_;
  Config config_;
  Universe universe_;
  CivilizationFactory factory_;
  std::priority_queue<std::unique_ptr<Event>,
      std::vector<std::unique_ptr<Event>>, CompareEvents> events_;
};

#endif
