EFNDK ?= third_party/efndk_cpp
OUTPUT_FILE ?= 言序.efn
CXX ?= g++

CXXFLAGS ?= -s -O2 -w -fpermissive -fPIC -c -D_LINUX -DNDEBUG -D_UNICODE -DUNICODE
EF_CXXFLAGS ?= $(CXXFLAGS)
LDFLAGS ?= -s -O2 -shared
INCLUDES = -I. -I$(EFNDK)/include
LDLIBS = -ldl

EF_SOURCES = \
	$(EFNDK)/source/ptrlist.cpp \
	$(EFNDK)/source/hashkv.cpp \
	$(EFNDK)/source/classes.cpp \
	$(EFNDK)/source/os.cpp \
	$(EFNDK)/source/utils.cpp \
	$(EFNDK)/source/functions.cpp

SOURCES = lib_yanxu.cpp cls_engine.cpp yanxu_loader.cpp $(EF_SOURCES)
OBJECTS = $(notdir $(SOURCES:.cpp=.o))

all: clean $(OUTPUT_FILE)

$(OUTPUT_FILE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

ptrlist.o: $(EFNDK)/source/ptrlist.cpp
	$(CXX) $(EF_CXXFLAGS) $(INCLUDES) $< -o $@

hashkv.o: $(EFNDK)/source/hashkv.cpp
	$(CXX) $(EF_CXXFLAGS) $(INCLUDES) $< -o $@

classes.o: $(EFNDK)/source/classes.cpp
	$(CXX) $(EF_CXXFLAGS) $(INCLUDES) $< -o $@

os.o: $(EFNDK)/source/os.cpp
	$(CXX) $(EF_CXXFLAGS) $(INCLUDES) $< -o $@

utils.o: $(EFNDK)/source/utils.cpp
	$(CXX) $(EF_CXXFLAGS) $(INCLUDES) $< -o $@

functions.o: $(EFNDK)/source/functions.cpp
	$(CXX) $(EF_CXXFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f *.o "$(OUTPUT_FILE)"

.PHONY: all clean
