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
  Star(int id, const Location& location) : id_(id), location_(location), civilization_id_(-1) {}

 private:
  int id_;
  Location location_;
  int civilization_id_;
};

class Civilization {
 public:
  Civilization(int id, int star_id) : id_(id), star_id_(star_id) {}

 private:
  int id_;
  int star_id_;
};

class Universe {
 public:
  Universe() {}

 private:
  std::map<int, Star> stars_;
  std::map<int, Civilization> civilizations_;
};
