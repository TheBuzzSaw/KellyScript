CXXFLAGS += -std=c++11
DEBUG_CXXFLAGS += -g -Wall -Werror
CPP_FILES := $(wildcard Source/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

ifeq ($(OS),Windows_NT)
	TARGET = kelly-script.exe
else
	TARGET = kelly-script.bin
	DEBUG_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
	DEBUG_LDFLAGS += -fsanitize=address
endif

all : debug

debug : CXXFLAGS += $(DEBUG_CXXFLAGS)
debug : LDFLAGS += $(DEBUG_LDFLAGS)
debug : $(TARGET)

release : CXXFLAGS += -O2
release : $(TARGET)

obj/%.o: Source/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET) : $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean :
	rm -f -v obj/*.o *.exe *.bin
