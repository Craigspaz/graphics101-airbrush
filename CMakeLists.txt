cmake_minimum_required(VERSION 3.14)

project(airbrush)

# Create the directory for saving artwork and remember its location.
set(ARTWORK "${PROJECT_SOURCE_DIR}/artwork")
file(MAKE_DIRECTORY "${ARTWORK}")

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

set(SRCS
    src/airbrush.cpp
    src/airbrush.h
    src/canvaswidget.cpp
    src/canvaswidget.h
    src/image.cpp
    src/image.h
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/stb_image.h
    src/stb_image_resize.h
    src/stb_image_write.h
)
add_executable(airbrush ${SRCS})
## Add "src" as an include directory for Qt's auto-generated source files.
target_include_directories(airbrush PUBLIC "src")
target_link_libraries(airbrush Qt5::Widgets)

## Let's use a fancier color picker
option(FANCY_COLOR_PICKER "Download and use a fancy color picker" ON)
if(FANCY_COLOR_PICKER)
    include(cmake/CPM.cmake)
    CPMFindPackage(
        NAME QtColorWidgets
        GITLAB_REPOSITORY mattbas/Qt-Color-Widgets
        GIT_TAG 85adc313
        VERSION 2.2
        )
    add_compile_definitions(FANCY_COLOR_PICKER=1)
    target_link_libraries(airbrush QtColorWidgets)
else()
    add_compile_definitions(FANCY_COLOR_PICKER=0)
endif()

include("CMakeLists-zip.txt" OPTIONAL)
