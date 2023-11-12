CXX = g++
CXXFLAGS = -std=c++11 -g
OPTFLAGS = -O3 -pthread
TARGET = Floorplan
SRCDIR = src
OBJS = main.o sa.o module.o

all: release

debug: CXXFLAGS += -Wall -DDEBUG_FLAG=1 -g
debug: TARGET := $(TARGET)_debug
debug: $(TARGET)

release: CXXFLAGS += -DDEBUG_FLAG=0 $(OPTFLAGS)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp $(SRCDIR)/sa.h
	$(CXX) $(CXXFLAGS) -c main.cpp

sa.o: $(SRCDIR)/sa.cpp $(SRCDIR)/sa.h
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/sa.cpp

module.o: $(SRCDIR)/module.cpp $(SRCDIR)/module.h
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/module.cpp

run-debug:
	./$(TARGET)_debug input.in output_debug.out

run-release:
	./$(TARGET) input.in output_release.out

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET)_debug output_debug.out output_release.out
