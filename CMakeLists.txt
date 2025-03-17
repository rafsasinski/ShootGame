cmake_minimum_required(VERSION 3.31)
project(ShootGame)

set(CMAKE_CXX_STANDARD 26)

# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

# Explicitly find OIS
find_path(OIS_INCLUDE_DIR NAMES OIS.h PATHS /usr/local/include/ois)
find_library(OIS_LIBRARY NAMES OIS PATHS /usr/local/lib)

if (NOT OIS_INCLUDE_DIR OR NOT OIS_LIBRARY)
    message(FATAL_ERROR "OIS not found!")
endif ()

message(STATUS "OIS include directory: ${OIS_INCLUDE_DIR}")
message(STATUS "OIS library: ${OIS_LIBRARY}")

include_directories(${OIS_INCLUDE_DIR})
# add the source files as usual
#add_executable(0_Bootstrap Bootstrap.cpp)
add_executable(ShootGame main.cpp BaseApplication.cpp MechWalkTest.cpp)

# this also sets the includes and pulls third party dependencies
target_link_libraries(ShootGame
        ${OIS_LIBRARY}
        OgreBites)