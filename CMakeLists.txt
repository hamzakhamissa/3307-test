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
    icrops.h icrops.cpp
    corn.h corn.cpp
    itiles.h itiles.cpp
    field.h field.cpp
    itool.h itool.cpp
    water.h water.cpp
    wateringcan.h wateringcan.cpp
    yard.h yard.cpp
    wheat.h wheat.cpp
    tomato.h tomato.cpp
    cow.h cow.cpp
    ianimal.h ianimal.cpp
    chicken.h chicken.cpp
    farminghoe.h farminghoe.cpp
    character.h character.cpp
    player.h player.cpp
    itemtypeenum.h
    inventory.h inventory.cpp
    iinteract.h iinteract.cpp
    market.h market.cpp
    menu.h menu.cpp
    game.h game.cpp
    resources.qrc
    mainwindow.h mainwindow.cpp
    tileboard.h tileboard.cpp
)

# Link Qt libraries
target_link_libraries(assignment2
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)
