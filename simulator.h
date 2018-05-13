#include <queue>
#include "event.h"
#include "universe.h"

class Simulator {
 public:
  Simulator() {}
  void Run();

 private:
  Universe universe_;
  std::priority_queue<Event> events_;
};
