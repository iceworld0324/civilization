CXX = g++
CXXFLAG = -Og -Wall -std=c++11
HDR = universe.h event.h simulator.h
OBJ = universe.o event.o simulator.o simulation.o
BIN = simulation

%.o: %.cpp $(HDR)
	$(CXX) -c $< -o $@ $(CXXFLAG)

$(BIN): $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAG)

clean:
	rm $(BIN) $(OBJ)
