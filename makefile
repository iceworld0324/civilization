CXX = g++
CXXFLAG = -Og -Wall -std=c++11
OBJ = universe.o event.o simulator.o simulation.o
BIN = simulation

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAG)

$(BIN): $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAG)

clean:
	rm $(BIN) $(OBJ)
