cmake_minimum_required(VERSION 3.8)
project(project_Sorting)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE on)

#file(GLOB sourceFILES *.cpp *.c)
#FOREACH (sourceFile ${sourceFILES})
#    #get_filename_component(sourceFile_Path ${sourceFile} PATH)
#    get_filename_component(sourceFile_Name ${sourceFile} NAME)
#
#    MESSAGE(STATUS "Process file, name: ${sourceFile_Name}")
#    #MESSAGE(STATUS "Process file, path: ${sourceFile}")
#
#    add_executable("${sourceFile_Name}" ${sourceFile})
#endforeach (sourceFile)

add_executable(basic_sorts.cpp basic_sorts.cpp)
add_executable(integer_sort.hpp integer_sort.hpp)
add_executable(integer_sort.cpp integer_sort.cpp)
add_executable(integer_sort_objects_small.cpp integer_sort_objects_small.cpp)
add_executable(ir_sort_competitions.cpp ir_sort_competitions.cpp)
