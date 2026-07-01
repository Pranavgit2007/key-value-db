CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -Iinclude

all: server client

server: src/server.cpp src/Database.cpp
	$(CXX) $(CXXFLAGS) src/server.cpp src/Database.cpp -o server

client: src/client.cpp
	$(CXX) $(CXXFLAGS) src/client.cpp -o client

clean:
	rm -f server client *.kv

.PHONY: all clean