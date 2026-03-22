CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -O2

TARGET=pru_arm_test
SRCS=main.cpp pructrl.cpp
OBJS=$(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)