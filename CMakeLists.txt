cmake_minimum_required(VERSION 3.5)

project(SleepMoniotrV2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(curvegroup)
find_package(Qt5 COMPONENTS Widgets serialport REQUIRED)

include_directories(ControlWnd)
include_directories(OutputWnd)

file(GLOB SRC ./src/*)
file(GLOB controlwnd ./ControlWnd/*)
file(GLOB outputwnd ./OutputWnd/*)
file(GLOB RES ./res/*)
file(GLOB ICON ./icon/*)
file(COPY ${RES} DESTINATION res)
file(COPY ${CURVE_RES} DESTINATION res)
file(COPY ${ICON} DESTINATION icon)
add_executable(SleepMoniotrV2 main.cpp ${SRC} ${controlwnd} ${outputwnd})

target_link_libraries(SleepMoniotrV2  Qt5::Widgets Qt5::SerialPort curvegroup)
