# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(LAB1 VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(OPENAL_LIBRARY ${PROJECT_SOURCE_DIR}/../SFML/extlibs/libs-msvc/x64/openal32.lib)


# Add source files
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/code/*.cpp)

link_directories(${PROJECT_SOURCE_DIR}/../SFML/lib)

# Add the executable
add_executable(LAB1 ${SOURCES})

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
#target_link_libraries(LAB1 PUBLIC ${GRAPHICS_LIB} ${SYSTEM_LIB} ${WINDOW_LIB})
target_link_libraries(LAB1 PUBLIC ${SFML_GRAPHICS} ${SFML_WINDOW} ${SFML_SYSTEM})

set_target_properties(
	LAB1 PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${COMMON_OUTPUT_DIR}/bin"
)
# Copy DLLs using file(COPY ...)
file(COPY ${PROJECT_SOURCE_DIR}/fonts
     DESTINATION "${COMMON_OUTPUT_DIR}/bin")
file(COPY ${PROJECT_SOURCE_DIR}/graphics
     DESTINATION "${COMMON_OUTPUT_DIR}/bin")
