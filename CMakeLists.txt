cmake_minimum_required(VERSION 3.31)
project(ShootGame)

set(CMAKE_CXX_STANDARD 20)

# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

# add the source files as usual
#add_executable(0_Bootstrap Bootstrap.cpp)
add_executable(ShootGame main.cpp)

# this also sets the includes and pulls third party dependencies
target_link_libraries(ShootGame OgreBites)