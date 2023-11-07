CXX = g++
CXXFLAGS = -std=c++11 -g
OPTFLAGS = -O3 -pthread
TARGET = Floorplan
SRCDIR = src
OBJS = main.o sa.o

all: release

debug: CXXFLAGS += -Wall -DDEBUG_FLAG=1 -g
debug: $(TARGET)

release: CXXFLAGS += -DDEBUG_FLAG=0 $(OPTFLAGS)
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp $(SRCDIR)/sa.h
	$(CXX) $(CXXFLAGS) -c main.cpp

sa.o: $(SRCDIR)/sa.cpp $(SRCDIR)/sa.h
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/sa.cpp

run:
	./$(TARGET) input.in output_release.out

clean:
	rm -f $(OBJS) $(TARGET) output_debug.out output_release.out
