cmake_minimum_required(VERSION 3.5)
project(1.0)

SET(EXECUTABLE_OUTPUT_PATH $ENV{HOME}/work/github/OpenGlStudy/bin)
SET(LIBRARY_OUTPUT_PATH $ENV{HOME}/work/github/OpenGlStudy/lib)

macro(source_group_by_dir source_files)
    set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(sgbd_file ${${source_files}})
        string(REGEX REPLACE ${sgbd_cur_dir}//(.*/) //1 sgbd_fpath ${sgbd_file})
        string(REGEX REPLACE "/(.*/)/.*" //1 sgbd_group_name ${sgbd_fpath})
        string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        string(REPLACE "/" "//" sgbd_group_name ${sgbd_group_name})
        if(sgbd_nogroup)
            set(sgbd_group_name "//")
        endif(sgbd_nogroup)
        source_group(${sgbd_group_name} FILES ${sgbd_file})
    endforeach(sgbd_file)
endmacro(source_group_by_dir)

add_definitions("-g")

FILE(GLOB_RECURSE all_files
        *.cpp
        *.cc
        *.c
        )

link_directories("$ENV{HOME}/work/github/OpenGlStudy/lib/")

source_group(all_files)
include_directories($ENV{HOME}/work/github/OpenGlStudy/include/)
include_directories(/usr/local/include/gl3w/)
include_directories($ENV{HOME}/work/github/OpenGlStudy/3rdparty/include/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
add_executable(test1.0 ${all_files})

target_link_libraries(test1.0 GL GLU glut glfw3 dl pthread)








