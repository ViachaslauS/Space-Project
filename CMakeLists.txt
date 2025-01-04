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
    "src/HUD/HUDBase.cpp"
    "src/HUD/HUDGameState.cpp"
    "src/HUD/HUDContainer.cpp"
    "src/Weapons/BaseWeapon.cpp"
    "src/Weapons/LaserWeapon.cpp"
    "src/Weapons/RocketWeapon.cpp"
    "src/Enemies/BaseEnemyShip.cpp"
    "src/Enemies/SmallEnemyShip.cpp"
    "src/Enemies/Asteroid.cpp"
    "src/ObjectsManager.cpp"
    "src/BaseShip.cpp"

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
    "src/Weapons/BaseWeapon.h"
    "src/Weapons/LaserWeapon.h"
    "src/Weapons/RocketWeapon.h"
    "src/Enemies/BaseEnemyShip.h"
    "src/Enemies/SmallEnemyShip.h"
    "src/Enemies/Asteroid.h"
    "src/ObjectsManager.h"
    "src/Weapons/Projectile.h"
    "src/BaseShip.h"

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
