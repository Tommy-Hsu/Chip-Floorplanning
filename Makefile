CXX = g++
CXXFLAGS = -std=c++11 -g
OPTFLAGS = -O3 -pthread
TARGET = Floorplan
OBJS = main.o

all: $(TARGET) release

debug: CXXFLAGS += -Wall -DDEBUG_FLAG=1 -g
debug: $(TARGET) time

release: CXXFLAGS += -DDEBUG_FLAG=0 $(OPTFLAGS)
release: $(TARGET) time

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

time:
	time ./$(TARGET) input.in output_release.out

clean:
	rm -f $(OBJS) $(TARGET) output_debug.out output_release.out
