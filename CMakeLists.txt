cmake_minimum_required(VERSION 3.15)

project(boreal CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        endstone
        GIT_REPOSITORY https://github.com/EndstoneMC/endstone.git
        GIT_TAG v0.6.1
)
FetchContent_MakeAvailable(endstone)

IF (WIN32)
    link_directories("installed/lib/")
    include_directories("installed/include/")
    link_libraries(funchook distorm psapi)
    endstone_add_plugin(${PROJECT_NAME} src/boreal.cpp installed/include/funchook.h installed/include/distorm.h installed/include/mnemonics.h)
    target_include_directories(${PROJECT_NAME} PRIVATE include)
ELSE()
    link_directories("/usr/local/lib/")
    include_directories("installed/include/")
    include_directories("include/lib/")
    link_libraries(funchook)

    FILE(GLOB SRCLIB src/lib/*.cpp)
    endstone_add_plugin(${PROJECT_NAME} src/boreal.cpp ${SRCLIB})
ENDIF()
