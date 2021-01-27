cmake_minimum_required(VERSION 3.5)

project(synthesis-task LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(synthesis
    main.cpp
    include/synthesizer.hpp
    include/wav.hpp
    include/number_traits.h
    include/audiomenu.h
    src/audiomenu.cpp
    ui/audiomenu.ui
)

target_link_libraries(synthesis PRIVATE Qt5::Widgets)
