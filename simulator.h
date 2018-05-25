#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "event.h"
#include "universe.h"
#include <memory>
#include <queue>
#include <random>
#include <string>

class Config {
public:
  Config(const std::string &config_filename);

  double end_time() const { return end_time_; }
  int num_stars() const { return num_stars_; }
  double radius_universe() const { return radius_universe_; }
  double lifespan_mean() const { return lifespan_mean_; }
  double lifespan_stddev() const { return lifespan_stddev_; }
  double civilization_birth_interval() const {
    return civilization_birth_interval_;
  }
  double science_advance_one_prob() const { return science_advance_one_prob_; }
  double science_advance_two_prob() const { return science_advance_two_prob_; }
  double science_advance_interval() const { return science_advance_interval_; }

private:
  double end_time_;
  int num_stars_;
  double radius_universe_;
  double lifespan_mean_;
  double lifespan_stddev_;
  double civilization_birth_interval_;
  double science_advance_one_prob_;
  double science_advance_two_prob_;
  double science_advance_interval_;
};

class Simulator {
public:
  Simulator(const std::string &config_filename);

  void Run();

private:
  Config config_;
  std::default_random_engine generator_;
  StarFactory star_factory_;
  CivilizationFactory civilization_factory_;
  CivilizationBirthHandler civilization_birth_handler_;
  CivilizationDeathHandler civilization_death_handler_;
  ScienceAdvanceHandler science_advance_handler_;
  Universe universe_;
  std::priority_queue<std::unique_ptr<Event>,
                      std::vector<std::unique_ptr<Event>>, CompareEvents>
      events_;
};

#endif
