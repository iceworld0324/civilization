#include "simulator.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <utility>

constexpr char kSplitToken[] = ": ";

Config::Config(const std::string &config_filename) {
  std::ifstream config_file(config_filename);
  std::string line;
  while (std::getline(config_file, line)) {
    size_t pos = line.find(kSplitToken);
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + strlen(kSplitToken));
    if (key == "end_time") {
      end_time_ = std::stod(value);
    } else if (key == "num_stars") {
      num_stars_ = std::stoi(value);
    } else if (key == "radius_universe") {
      radius_universe_ = std::stod(value);
    } else if (key == "lifespan_mean") {
      lifespan_mean_ = std::stod(value);
    } else if (key == "lifespan_stddev") {
      lifespan_stddev_ = std::stod(value);
    } else if (key == "civilization_birth_interval") {
      civilization_birth_interval_ = std::stod(value);
    } else if (key == "science_advance_one_prob") {
      science_advance_one_prob_ = std::stod(value);
    } else if (key == "science_advance_two_prob") {
      science_advance_two_prob_ = std::stod(value);
    } else if (key == "science_advance_interval") {
      science_advance_interval_ = std::stod(value);
    }
  }
}

Simulator::Simulator(const std::string &config_filename)
    : config_(config_filename),
      star_factory_(config_.radius_universe(), &generator_),
      civilization_factory_(config_.lifespan_mean(), config_.lifespan_stddev(),
                            &generator_),
      civilization_birth_handler_(config_.civilization_birth_interval(),
                                  &generator_, &civilization_factory_,
                                  &civilization_death_handler_),
      science_advance_handler_(config_.science_advance_interval(),
                               {0, config_.science_advance_one_prob(),
                                config_.science_advance_two_prob()},
                               &generator_) {
  for (int i = 0; i < config_.num_stars(); i++) {
    Star star = star_factory_.Create();
    universe_.mutable_stars()->insert({star.id(), star});
  }
  events_.emplace(new CivilizationBirth(0.0, &civilization_birth_handler_));
  events_.emplace(new ScienceAdvance(0.0, &science_advance_handler_, -1));
}

void Simulator::Run() {
  while (!events_.empty() && events_.top()->timestamp() < config_.end_time()) {
    const std::unique_ptr<Event> &event = events_.top();
    std::cout << event->Print() << std::endl;
    std::vector<std::unique_ptr<Event>> following_events =
        event->handler()->Handle(event, &universe_);
    events_.pop();
    for (std::unique_ptr<Event> &following_event : following_events) {
      events_.push(std::move(following_event));
    }
  }
}
