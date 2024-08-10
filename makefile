CXX = clang++
CXXFLAGS = -std=c++17
LDFLAGS = -framework AudioToolbox

SRCS = main.cc matrix.cc maths.cc matrix.cc range.cc utilities.cc
OBJS = $(SRCS:.cc=.o)
TARGET = audio_test
TARGET_DEBUG = audio_test_d

.PHONY: all clean debug release

all: release

debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

release: CXXFLAGS += -O2
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# rule to make
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)