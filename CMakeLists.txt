cmake_minimum_required(VERSION 3.19.2)

project(airbrush LANGUAGES CXX C)

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

# Old versions of homebrew didn't put Qt in the usual place. This finds it.
# See: https://github.com/Homebrew/homebrew-core/issues/8392
if(APPLE AND ( NOT EXISTS /opt/homebrew ) AND EXISTS /usr/local/opt/qt)
    # Homebrew installs Qt in /usr/local/opt/qt.
    # Ensure it can be found by CMake since
    # it is not in the default /usr/local prefix.
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
endif()

# Once Qt 5.15 is everywhere, we can modify this to support Qt 5 and 6 by putting "5.15" after "Qt5"
# and then linking with "Qt::Widgets".
# From: https://www.qt.io/blog/versionless-cmake-targets-qt-5.15
# This form lets us use all kinds of Qt: https://doc-snapshots.qt.io/qt6-dev/cmake-qt5-and-qt6-compatibility.html
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

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
target_link_libraries(airbrush PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

## Let's use a fancier color picker
option(FANCY_COLOR_PICKER "Download and use a fancy color picker" OFF)
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
