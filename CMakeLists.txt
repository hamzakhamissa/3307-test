cmake_minimum_required(VERSION 3.16)
project(Assignment2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 or Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

# Enable automatic MOC (Meta-Object Compiler) and RCC (Resource Compiler)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(assignment2
    main.cpp
    resources.qrc
    core/icrops.h core/icrops.cpp
    core/corn.h core/corn.cpp
    core/itiles.h core/itiles.cpp
    core/field.h core/field.cpp
    core/itool.h core/itool.cpp
    core/water.h core/water.cpp
    core/wateringcan.h core/wateringcan.cpp
    core/yard.h core/yard.cpp
    core/wheat.h core/wheat.cpp
    core/tomato.h core/tomato.cpp
    core/cow.h core/cow.cpp
    core/ianimal.h core/ianimal.cpp
    core/chicken.h core/chicken.cpp
    core/farminghoe.h core/farminghoe.cpp
    core/character.h core/character.cpp
    core/player.h core/player.cpp
    core/itemtypeenum.h
    core/inventory.h core/inventory.cpp
    core/iinteract.h core/iinteract.cpp
    core/market.h core/market.cpp
    core/menu.h core/menu.cpp
    core/tileboard.h core/tileboard.cpp
    ui/game.h ui/game.cpp
)

target_include_directories(assignment2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# Link Qt libraries
target_link_libraries(assignment2
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)
