
# You are free to modify this file

cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 14)
project(asteroids)

if(APPLE)
    find_path(freetypePath freetype2 PATHS /usr/local/include NO_DEFAULT_PATH)
else()
    find_path(freetypePath freetype2)
endif()

find_package( X11 REQUIRED )
MESSAGE("X11 information:")
MESSAGE("  X11_INCLUDE_DIR: ${X11_INCLUDE_DIR}")
MESSAGE("  X11_LIBRARIES: ${X11_LIBRARIES}")
MESSAGE("  X11_LIBRARY_DIRS: ${X11_LIBRARY_DIRS}")
include_directories( ${X11_INCLUDE_DIR} )
include_directories(./lib/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")
file(GLOB SRC src/*.cpp)
file(GLOB HDR src/*.h)

add_executable(asteroids ${SRC} ${HDR})
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)
target_link_libraries(asteroids LINK_PUBLIC ${X11_LIBRARIES})

#add_executable(scratch scratch.cpp)
#target_link_libraries(scratch libfreetype)

