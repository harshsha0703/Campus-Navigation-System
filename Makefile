CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

app: main.cpp application.cpp dist.cpp graph.h application.h dist.h json.hpp
	$(CXX) $(CXXFLAGS) main.cpp application.cpp dist.cpp -o app

clean:
	rm -f app
