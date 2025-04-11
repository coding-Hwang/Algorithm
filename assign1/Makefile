CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = experiment
SRCS = experiment.cc convention_algorithm.cc contemporary_algorithm.cc
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET) *.o experiment_results.csv
