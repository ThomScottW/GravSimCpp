cmake_minimum_required(VERSION 3.10)

# Specify C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set where executables get stored.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(COMPILE_FLAGS "-Wall -pedantic-errors -Werror -g -D_REENTRANT -I/usr/include/SDL2")



# Create executable for main source files.
# Set the project name.
project(a.out.src)
# Get a list of file names.
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Now we need to get a list of the source filenames besides one that contains the main function.
# The reason we exclude the main function is because the compiler will complain about multiple
# definitions of the main function when we try to build gtest, which contains its own main.
set(SRC_FILES_WITHOUT_MAIN ${SRC_FILES})
list(REMOVE_ITEM SRC_FILES_WITHOUT_MAIN ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Add the executable.
add_executable(${PROJECT_NAME} ${SRC_FILES})
# Add compile flags.
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# This will let us use the SDL2 library.
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_ttf)



# Create executable for experimentation folder.
# Set project name for exp.
project(a.out.exp)
# Get a list of file names.
file(GLOB EXP_SRC_FILES ${CMAKE_SOURCE_DIR}/exp/*.cpp)

# Add the executable.
add_executable(${PROJECT_NAME} ${EXP_SRC_FILES} ${SRC_FILES_WITHOUT_MAIN})
# Add compile flags.
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_ttf)



# Create executable for gtests.
# Set project name for gtest.
project(a.out.gtest)
# Get list of file names.
file(GLOB GTEST_SRC_FILES ${CMAKE_SOURCE_DIR}/gtest/*.cpp)

# Add the executable.
add_executable(${PROJECT_NAME} ${GTEST_SRC_FILES} ${SRC_FILES_WITHOUT_MAIN})
# Add compile flags.
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS})
# Include project header files.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
# Since gtest requires libraries to function properly, we need to include them.
target_link_libraries(${PROJECT_NAME} gtest gtest_main pthread SDL2 SDL2_ttf)
