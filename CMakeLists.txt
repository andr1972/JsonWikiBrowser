cmake_minimum_required(VERSION 3.16)
project(JsonWikiBrowser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(JsonWikiBrowser
        main.cpp
        MainWindow.cpp MainWindow.h
        FileOpenDialog.cpp FileOpenDialog.h
)

target_link_libraries(JsonWikiBrowser PRIVATE Qt6::Widgets)
