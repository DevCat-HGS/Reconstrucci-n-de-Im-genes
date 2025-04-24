cmake_minimum_required(VERSION 3.1.0)

project(image_reconstruction)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    image_utils.cpp
    image_utils.h
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
) 