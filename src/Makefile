.PHONY: all

CXXFLAGS+=-I../include
LIBS=-lproxygenhttpserver -lproxygenlib -lfolly -lwangle -lgflags -lglog -llog4cpp -ljsoncpp

OBJS=under_durest.o

DEPS=$(OBJS:.o=.d)

-include $(OBJS:.o=.d)

TARGET=under_durest

all: CXXFLAGS+=-O0 -g
all: $(TARGET)

debug: CXXFLAGS+=-ggdb3 -O0
debug: $(TARGET)

clean:
	- rm $(OBJS) $(DEPS) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cpp
	g++ -MM $(CXXFLAGS) $< > $@
