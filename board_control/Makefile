CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lpthread
OBJS = board_control.o main_control.o

all: control

control: $(OBJS)
	$(CXX) -o control $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o control
