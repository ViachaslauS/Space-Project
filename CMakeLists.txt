cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(gravigun CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_compile_options(-w)

add_executable(gravigun
    "src/main.cpp"
    "src/Game.cpp"
    "src/GameObject.cpp"
    "src/GravityZone.cpp"
    "src/Physics.cpp"
    "src/PlayerShip.cpp"
    "src/PlayerStats.cpp"
    "src/Popups/LevelUpPopup.cpp"
    "src/Popups/MainMenuPopup.cpp"
    "src/Popups/PausePopup.cpp"
    "src/Popups/Popup.cpp"
    "src/Popups/Popups.cpp"
    "src/Background.cpp"
    "src/GameplayManager.cpp"
    "src/GameEventsSystem.cpp"

    "include/resource_dir.h"
    "src/AppContext.h"
    "src/Game.h"
    "src/GameObject.h"
    "src/GravityZone.hpp"
    "src/Physics.hpp"
    "src/PlayerShip.h"
    "src/PlayerStats.h"
    "src/Popups/LevelUpPopup.h"
    "src/Popups/MainMenuPopup.h"
    "src/Popups/PausePopup.h"
    "src/Popups/Popup.h"
    "src/Popups/Popups.h"
    "src/Vitality.h"
    "src/Background.h"
    "src/Helpers.h"
    "src/external/reasings.h"
    "src/GameplayManager.h"
    "src/GameEventsSystem.h"
)

option(BOX2D_SAMPLES "Build the Box2D samples" OFF)
add_subdirectory(src/external/box2d)

add_subdirectory(src/external/raylib)

target_link_libraries(gravigun PRIVATE box2d raylib)
target_include_directories(gravigun PRIVATE include src)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT gravigun)
    set_property(TARGET gravigun PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()
