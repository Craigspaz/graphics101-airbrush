cmake_minimum_required(VERSION 3.1.0)

project(airbrush)

# We want C++11
set (CMAKE_CXX_STANDARD 11)

# Turn on some flags for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt generated files don't see headers here unless we turn this on.
# UPDATE: I have CMake 3.18, but I still need this.
# if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
# endif()

# Homebrew doesn't put Qt in the usual place. This finds it.
# See: https://github.com/Homebrew/homebrew-core/issues/8392
if(APPLE AND EXISTS /usr/local/opt/qt5)
	# Homebrew installs Qt5 (up to at least 5.9.1) in
	# /usr/local/qt5, ensure it can be found by CMake since
	# it is not in the default /usr/local prefix.
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(airbrush
    mainwindow.ui
    mainwindow.cpp
    main.cpp
    image.cpp
    canvaswidget.cpp
    airbrush.cpp
)

target_link_libraries(airbrush Qt5::Widgets)
