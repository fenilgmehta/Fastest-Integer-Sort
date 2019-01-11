cmake_minimum_required(VERSION 3.8)
project(project_Sorting)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE on)

add_subdirectory(src)
add_subdirectory(test)

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
