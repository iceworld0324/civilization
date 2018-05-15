#include "simulator.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <utility>

constexpr char kSplitToken[] = ": ";

Config::Config(const std::string& config_filename) {
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
    }
  }
}

void Simulator::Run() {
  while (time_ < config_.end_time()) {
    const std::unique_ptr<Event>& event = events_.top();
    time_ = event->timestamp();
    std::cout << event->Print() << std::endl;
    std::vector<std::unique_ptr<Event>> following_events = event->Process(&universe_);
    events_.pop();
    for (std::unique_ptr<Event>& following_event : following_events) {
      events_.push(std::move(following_event));
    }
  }
}
