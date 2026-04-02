cmake_minimum_required(VERSION 3.16)
project(Base64ConverUI VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

if(WIN32)
    set(build_option WIN32)
endif()
add_executable(Base64ConverUI ${build_option}
    main.cpp
    Base64ConverUI.cpp
    Base64Conver.cpp
)

target_link_libraries(Base64ConverUI
    Qt6::Core
    Qt6::Widgets
)