cmake_minimum_required(VERSION 3.8)
project(project_Sorting C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB sourceFILES *.cpp *.c *.hpp)
FOREACH (sourceFile ${sourceFILES})
    #get_filename_component(sourceFile_Path ${sourceFile} PATH)
    get_filename_component(sourceFile_Name ${sourceFile} NAME)

    MESSAGE(STATUS "Process file, name: ${sourceFile_Name}")
    #MESSAGE(STATUS "Process file, path: ${sourceFile}")

    add_executable("${sourceFile_Name}" ${sourceFile})
endforeach (sourceFile)

#add_executable(raw_data_generator_integer.cpp raw_data_generator_integer.cpp)
#add_executable(project_Fastest-Integer-Sort.cpp project_Fastest-Integer-Sort.cpp)
#add_executable(project_SortingBest_integer.cpp project_SortingBest_integer.cpp)
#add_executable(project_SortingBest_object.cpp project_SortingBest_object.cpp)
