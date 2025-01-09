cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(gravigun CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_compile_options(-w)

add_executable(gravigun
    "src/main.cpp"
    "src/Background.cpp"
    "src/BaseShip.cpp"
    "src/Enemies/BaseEnemyShip.cpp"
    "src/Enemies/SmallEnemyShip.cpp"
    "src/Enemies/Asteroid.cpp"
    "src/Game.cpp"
    "src/GameEventsSystem.cpp"
    "src/GameplayManager.cpp"
    "src/GameObject.cpp"
    "src/GravityZone.cpp"
    "src/Helpers.cpp"
    "src/HUD/HUDBase.cpp"
    "src/HUD/HUDGameState.cpp"
    "src/HUD/HUDContainer.cpp"
    "src/HUD/HUDPlayerState.cpp"
    "src/HUD/HUDPlayerXP.cpp"
    "src/ObjectsManager.cpp"
    "src/Physics.cpp"
    "src/PlayerController.cpp"
    "src/PlayerShip.cpp"
    "src/PlayerStats.cpp"
    "src/Popups/DeathPopup.cpp"
    "src/Popups/LevelUpPopup.cpp"
    "src/Popups/MainMenuPopup.cpp"
    "src/Popups/PausePopup.cpp"
    "src/Popups/Popup.cpp"
    "src/Popups/Popups.cpp"
    "src/Skills.cpp"
    "src/Weapons/BaseWeapon.cpp"
    "src/Weapons/LaserWeapon.cpp"
    "src/Weapons/RocketWeapon.cpp"
    "src/Weapons/Projectile.cpp"
    "src/Weapons/Gravigun.cpp"

    "include/resource_dir.h"
    "src/external/reasings.h"
    "src/AppContext.h"
    "src/Background.h"
    "src/BaseShip.h"
    "src/Enemies/BaseEnemyShip.h"
    "src/Enemies/SmallEnemyShip.h"
    "src/Enemies/Asteroid.h"
    "src/Game.h"
    "src/GameObject.h"
    "src/GameplayManager.h"
    "src/GameEventsSystem.h"
    "src/GravityZone.hpp"
    "src/Helpers.h"
    "src/ObjectsManager.h"
    "src/Physics.hpp"
    "src/PlayerController.hpp"
    "src/PlayerShip.h"
    "src/PlayerStats.h"
    "src/Popups/DeathPopup.hpp"
    "src/Popups/LevelUpPopup.h"
    "src/Popups/MainMenuPopup.h"
    "src/Popups/PausePopup.h"
    "src/Popups/Popup.h"
    "src/Popups/Popups.h"
    "src/Skills.h"
    "src/Vitality.h"
    "src/HUD/HUDBase.hpp"
    "src/HUD/HUDGameState.hpp"
    "src/HUD/HUDContainer.hpp"
    "src/HUD/HUDPlayerState.hpp"
    "src/HUD/HUDPlayerXP.hpp"
    "src/Weapons/BaseWeapon.h"
    "src/Weapons/LaserWeapon.h"
    "src/Weapons/RocketWeapon.h"
    "src/Weapons/Projectile.h"
    "src/Weapons/Gravigun.hpp"
)

option(BOX2D_SAMPLES "Build the Box2D samples" OFF)
add_subdirectory(src/external/box2d)

add_subdirectory(src/external/raylib)

target_link_libraries(gravigun PRIVATE box2d raylib)
target_include_directories(gravigun PRIVATE include src)
if (${PLATFORM} STREQUAL "Web")
    set(EMSCRIPTEN_LINK_FLAGS
        "--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/resources/web/index.html"
        "-s ALLOW_MEMORY_GROWTH=1"
        "-s ENVIRONMENT=web"
        "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/resources@resources")

    if (${DISABLE_WASM})
        list(APPEND EMSCRIPTEN_LINK_FLAGS
            "-s WASM=0"
            "-s BINARYEN=0")
    endif()
    list(JOIN EMSCRIPTEN_LINK_FLAGS " " EMSCRIPTEN_LINK_ARGS)
    message(ERROR "${EMSCRIPTEN_LINK_ARGS}")
    set_target_properties(gravigun PROPERTIES
        LINK_FLAGS "${EMSCRIPTEN_LINK_ARGS}"
        SUFFIX ".html")
endif()

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT gravigun)
    set_property(TARGET gravigun PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()
