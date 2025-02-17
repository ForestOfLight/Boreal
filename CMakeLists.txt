cmake_minimum_required(VERSION 3.15)

project(boreal CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        endstone
        GIT_REPOSITORY https://github.com/EndstoneMC/endstone.git
        GIT_TAG v0.5.7.1 # TODO: CHANGE ME to the actual version you are using
)
FetchContent_MakeAvailable(endstone)

link_directories("/usr/local/lib/")
link_directories("installed/lib/")
include_directories("installed/include/")
link_libraries(funchook distorm psapi)
endstone_add_plugin(${PROJECT_NAME} src/boreal.cpp installed/include/funchook.h installed/include/distorm.h installed/include/mnemonics.h)
