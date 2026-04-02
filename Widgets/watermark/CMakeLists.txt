cmake_minimum_required(VERSION 3.17)
project(watermark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(3RD_PARTY_DIR ${PROJECT_SOURCE_DIR}/3rd_party)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${3RD_PARTY_DIR}/mini/include)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

add_executable(watermark 
    src/main.cpp
    src/watermark.cpp
    src/mainWindow.cpp
    src/sysTray.cpp
    rc/rcs.qrc
)

target_link_libraries(watermark
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)