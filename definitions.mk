# written by Shyamini Kularathna

WORK_DIR = /home/cbgeo/research/mpm_code

# compiler/linker
C_PP = g++

# compilation/linking flags
C_FLAGS = -g -O2
L_FLAGS = -g

# library paths and names
LIB_DIR = $(WORK_DIR)/libs
BOOST = $(LIB_DIR)/boost_1_61_0
EIGEN = $(LIB_DIR)/eigen-eigen-dc6cfdf9bcec

# include directory for mpm header files
INCLUDE_DIR = $(WORK_DIR)/includes 
MATERIAL = $(WORK_DIR)/includes/material
SRC = $(WORK_DIR)/includes/src
MISC = $(SRC)/misc
MESH = $(SRC)/mesh
PARTICLE = $(SRC)/particle
MATRIX = $(SRC)/matrix 