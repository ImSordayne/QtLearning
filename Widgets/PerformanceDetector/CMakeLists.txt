cmake_minimum_required(VERSION 3.28)
project(PerformanceDetector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}src
)

add_executable(PerformanceDetector
    main.cpp
    mainwindow.cpp
)

target_link_libraries(
    PerformanceDetector PRIVATE
    Qt6::Core
    Qt6::Widgets
)