# working directory: **this should be changed to find it automatically**
MPM_DIR = /home/cbgeo/research/MPMcode

# include the common information
include $(MPM_DIR)/definitions.mk

# compiler/linker
CXX = $(C_PP)
LD = $(C_PP)

# compilation/linking flags
CXXFLAGS = $(C_FLAGS) -std=c++11
LDFLAGS = $(L_FLAGS)

# add directories which include header files
INCLUDES = -I$(SRC) -I$(MATERIAL) -I$(EIGEN)
INCLUDES += -I$(MISC) -I$(MESH) -I$(PARTICLE) -I$(MATRIX) -I$(PROJECTION) 

# boost libraries
LIBS = -L$(BOOST) 
LIBS += -lboost_system -lboost_filesystem 

# eigen libraries
#LIBS += -L$(EIGEN)

# find source files and object files
CPPFILES = $(wildcard *.cpp)
OBJS = $(CPPFILES:.cpp=.o)

# final executable file
TARGET = incompressibleMPM


##############################################################
all: $(TARGET)

# link the target/object 
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)  

# rules for making object files
.cpp.o:
	$(CXX) $(INCLUDES) -fpermissive $(CXXFLAGS) -c $< 

#.PHONY clean
clean:
	-rm -f $(OBJS) $(TARGET)
##############################################################