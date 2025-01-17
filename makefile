# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := vengine

BUILD_DIR := build
SRC_DIRS := src
INC_DIRS := include
SHADERS_DIR := shaders

GLSLC := /usr/local/bin/glslc

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(wildcard $(SRC_DIRS)/*.cpp)

# Find shaders files 
VERTS := $(wildcard $(SHADERS_DIR)/*.vert)
FRAGS := $(wildcard $(SHADERS_DIR)/*.frag)

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# Prepends in SHADERS_DIR and appends .spv to every shader file
VSPVS := $(VERTS:%= %.spv)
FSPVS := $(FRAGS:%= %.spv)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Include all headers with -I
INC_FLAGS := -I$(INC_DIRS)

# Include librairies
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -std=c++17 -MMD -MP  -ggdb
CXXFLAGS := -Wall


# COMPILE SHADERS 
$(BUILD_DIR)/$(TARGET_EXEC): $(VSPVS) $(FSPVS)

%.spv : %
	$(GLSLC) $< -o $@

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)