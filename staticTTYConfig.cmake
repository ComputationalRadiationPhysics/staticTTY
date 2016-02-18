# - Config file for the staticTTY package
# It defines the following variables
#  staticTTY_INCLUDE_DIRS - include directories for FooBar
#  staticTTY_LIBRARIES    - libraries to link against

###############################################################################
# staticTTY
###############################################################################
cmake_minimum_required(VERSION 3.0.1)
project("staticTTY")

set(staticTTY_INCLUDE_DIRS ${staticTTY_INCLUDE_DIRS} "${staticTTY_DIR}/include")

###############################################################################
# COMPILER FLAGS
###############################################################################
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

###############################################################################
# DEPENDENCIES
###############################################################################

################################################################################
# Find PThreads
################################################################################
find_package(Threads MODULE)
set(staticTTY_LIBRARIES ${staticTTY_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
