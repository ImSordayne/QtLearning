cmake_minimum_required(VERSION 3.28)
project(Letter)

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

set(SOURCES
    main.cpp
    window.cpp
)

if(WIN32)
    set(SOURCES ${SOURCES} Info.rc)
endif()

add_executable(Letter
    ${SOURCES}
)

target_link_libraries(
    Letter PRIVATE
    Qt6::Core
    Qt6::Widgets
)


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(WIN32)
        set_target_properties(Letter PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    endif()
endif()
# 隐藏窗口