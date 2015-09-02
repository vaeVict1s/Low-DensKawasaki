# check on CMake version
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# set project properties
SET (PROJECT_NAME Low-DensKawasaki)
PROJECT (${PROJECT_NAME})

# add compilation flags
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# set sources files and define executable
SET (SOURCES myoptLexer.c myoptParser.c myoptSymtab.c mersenne.cpp DensManager.cpp Grid.cpp Main.cpp)
ADD_EXECUTABLE (${PROJECT_NAME} ${SOURCES})

# set output directory for executable
SET_TARGET_PROPERTIES ( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/..")
