#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <map>

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
  Star(int id, const Location& location)
    : id_(id), location_(location), resident_(-1) {}

  int resident() const { return resident_; }
  void set_resident(int resident) { resident_ = resident; }

 private:
  int id_;
  Location location_;
  int resident_;
};

class Civilization {
 public:
  Civilization(int id, int residence) : id_(id), residence_(residence) {}

  int id() const { return id_; }
  int residence() const { return residence_; }

 private:
  int id_;
  int residence_;
};

class Universe {
 public:
  Universe(int num_stars, double radius_universe) {
    CreateStars(num_stars, radius_universe);
  }

  const std::map<int, Star>& stars() const { return stars_; }
  std::map<int, Star>* mutable_stars() { return &stars_; }

  const std::map<int, Civilization>& civilizations() const { return civilizations_; }
  std::map<int, Civilization>* mutable_civilizations() { return &civilizations_; }

 private:
  void CreateStars(int num_stars, double radius_universe);

  std::map<int, Star> stars_;
  std::map<int, Civilization> civilizations_;
};

#endif
