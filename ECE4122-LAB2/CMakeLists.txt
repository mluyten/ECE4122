# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Lab2 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/*.cpp)

find_package(OpenMP REQUIRED)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Add the executable
add_executable(Lab2 ${SOURCES})

include_directories(${PROJECT_SOURCE_DIR}/../SFML/include)

find_library(SFML_GRAPHICS 
	NAME sfml-graphics sfml-window sfml-system
	HINTS "${PROJECT_SOURCE_DIR}/../SFML/lib")
find_library(SFML_WINDOW 
	NAME sfml-window
	HINTS "${PROJECT_SOURCE_DIR}/../SFML/lib")
find_library(SFML_SYSTEM 
	NAME sfml-system
	HINTS "${PROJECT_SOURCE_DIR}/../SFML/lib")

message(STATUS "SFML:  [${SFML_GRAPHICS} ${SFML_WINDOW} ${SFML_SYSTEM}]")

# Link the executable to the libraries in the lib directory
target_link_libraries(Lab2 PUBLIC ${SFML_GRAPHICS} ${SFML_WINDOW} ${SFML_SYSTEM})

if(OpenMP_CXX_FOUND)
	target_link_libraries(Lab2 PUBLIC OpenMP::OpenMP_CXX)
endif()

set_target_properties(
	Lab2 PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)
