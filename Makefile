CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGETS = server client
OBJ_SERVER = server.o
OBJ_CLIENT = client.o

all: $(TARGETS)

server: $(OBJ_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^

client: $(OBJ_CLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean
