cmake_minimum_required(VERSION 3.28)
project(calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(calculator
    main.cpp
    calculator.cpp
)

target_link_libraries(
    calculator PRIVATE
    Qt6::Core
    Qt6::Widgets
)