#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <map>
#include <random>

class Location {
public:
  Location(double x, double y, double z) : x_(x), y_(y), z_(z) {}

private:
  double x_;
  double y_;
  double z_;
};

class Star {
public:
  Star(int id, const Location &location)
      : id_(id), location_(location), resident_(-1) {}

  int id() const { return id_; }
  int resident() const { return resident_; }
  void set_resident(int resident) { resident_ = resident; }

private:
  int id_;
  Location location_;
  int resident_;
};

class Civilization {
public:
  Civilization(int id, int residence, double lifespan)
      : id_(id), residence_(residence), lifespan_(lifespan), science_level_(0) {
  }

  int id() const { return id_; }
  int residence() const { return residence_; }
  double lifespan() const { return lifespan_; }
  void advance_science(int level) { science_level_ += level; }

private:
  int id_;
  int residence_;
  double lifespan_;
  int science_level_;
};

class Universe {
public:
  const std::map<int, Star> &stars() const { return stars_; }
  std::map<int, Star> *mutable_stars() { return &stars_; }

  const std::map<int, Civilization> &civilizations() const {
    return civilizations_;
  }
  std::map<int, Civilization> *mutable_civilizations() {
    return &civilizations_;
  }

private:
  std::map<int, Star> stars_;
  std::map<int, Civilization> civilizations_;
};

class StarFactory {
public:
  StarFactory(double radius_universe, std::default_random_engine *generator)
      : distribution_(-radius_universe, radius_universe), generator_(generator),
        next_id_(0) {}

  Star Create();

private:
  std::uniform_real_distribution<double> distribution_;
  std::default_random_engine *generator_;
  int next_id_;
};

class CivilizationFactory {
public:
  CivilizationFactory(double lifespan_mean, double lifespan_stddev,
                      std::default_random_engine *generator)
      : lifespan_distribution_(lifespan_mean, lifespan_stddev),
        generator_(generator), next_id_(0) {}

  Civilization Create(const Universe &universe);

  int next_id() const { return next_id_; }

private:
  std::normal_distribution<double> lifespan_distribution_;
  std::default_random_engine *generator_;
  int next_id_;
};

#endif
