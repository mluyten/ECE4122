# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(LAB5 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

find_library(SFML_GRAPHICS 
	NAME sfml-graphics sfml-window sfml-system
	HINTS "${PROJECT_SOURCE_DIR}/../SFML/lib")
find_library(SFML_WINDOW 
	NAME sfml-window
	HINTS "${PROJECT_SOURCE_DIR}/../SFML/lib")
find_library(SFML_SYSTEM 
	NAME sfml-system
	HINTS "${PROJECT_SOURCE_DIR}/../SFML/lib")
find_library(SFML_NETWORK
	NAME sfml-network
	HINTS "${PROJECT_SOURCE_DIR}/../SFML/lib")

message(STATUS "SFML:  [${SFML_GRAPHICS} ${SFML_WINDOW} ${SFML_SYSTEM} ${SFML_NETWORK}]")

# Add source files
file(GLOB CLIENT_SOURCES ${PROJECT_SOURCE_DIR}/code/client/*.cpp)

# Add the executable
add_executable(LAB5_CLIENT ${CLIENT_SOURCES})

# Link the executable to the libraries in the lib directory
target_link_libraries(LAB5_CLIENT PUBLIC ${SFML_WINDOW} ${SFML_SYSTEM} ${SFML_NETWORK})

# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/server/*.cpp)

# Add the executable
add_executable(LAB5_SERVER ${SOURCES})

# Link the executable to the libraries in the lib directory
target_link_libraries(LAB5_SERVER PUBLIC ${SFML_GRAPHICS} ${SFML_WINDOW} ${SFML_SYSTEM} ${SFML_NETWORK})

file(COPY ${PROJECT_SOURCE_DIR}/graphics
     DESTINATION "${COMMON_OUTPUT_DIR}/bin")
