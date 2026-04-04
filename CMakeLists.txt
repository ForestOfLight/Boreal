cmake_minimum_required(VERSION 3.15)

project(boreal CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        endstone
        GIT_REPOSITORY https://github.com/EndstoneMC/endstone.git
        GIT_TAG v0.11.3
)
FetchContent_MakeAvailable(endstone)

FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG        fe8d7d78c4823e8a66a050bf86f5c6318cf76ce7
)

FetchContent_MakeAvailable(EnTT)

IF (WIN32)
    link_directories("installed/lib/")
    include_directories("installed/include/")
    link_libraries(funchook distorm psapi EnTT)
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
