CPPFILES = particle.cpp
OBJS = $(CPPFILES:.cpp=.o)
OUTFILES = $(wildcard *.dat)

TARGET = particleGenerator

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -g -o $@ $(OBJS)

.cpp.o:
	g++ -g -c $<

clean:
	-rm -f $(OBJS) $(TARGET) $(OUTFILES) 

