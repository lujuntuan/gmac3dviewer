#*********************************************************************************
#  *Copyright(C): Juntuan.Lu, 2020-2030, All rights reserved.
#  *Author:  Juntuan.Lu
#  *Version: 1.0
#  *Date:  2021/10/23
#  *Email: 931852884@qq.com
#  *Description:
#  *Others:
#  *Function List:
#  *History:
#**********************************************************************************

cmake_minimum_required(VERSION 3.10)
project(gmac3dviewer LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
find_package(Threads REQUIRED)
file(GLOB_RECURSE PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.qrc
    ${CMAKE_SOURCE_DIR}/src/*.ui
    )
if(WIN32)
    list(APPEND PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/src/ico/main.rc)
endif()
add_executable(
    ${PROJECT_NAME}
    ${PROJECT_SOURCES}
    )
if(WIN32)
    set_property(
        TARGET
        ${PROJECT_NAME}
        PROPERTY
        WIN32_EXECUTABLE
        true
    )
endif()
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    )
file(GLOB_RECURSE QML_SOURCES
    ${CMAKE_SOURCE_DIR}/src/qml/*.qml
    )
##### qt
find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Quick Widgets)
##### osg
if(MSVC)
    set(OSG_SDK_PATH "${CMAKE_SOURCE_DIR}/osgsdk/win64-msvc" CACHE PATH "OSG SDK Path")
    target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${OSG_SDK_PATH}/include
        )
    target_link_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${OSG_SDK_PATH}/lib
        ${OSG_SDK_PATH}/bin
        )
else()
    find_package(
        OpenSceneGraph REQUIRED
        osg
        osgAnimation
        osgManipulator
        osgViewer
        osgGA
        osgDB
        osgParticle
        osgUtil
        osgText
        OpenThreads
    )
endif()
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    osg
    osgAnimation
    osgManipulator
    osgViewer
    osgGA
    osgDB
    osgParticle
    osgUtil
    osgText
    OpenThreads
    )
##### install
include(GNUInstallDirs)
install(
    TARGETS
    ${PROJECT_NAME}
    RUNTIME
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
    )
install(
    DIRECTORY
    ${CMAKE_SOURCE_DIR}/sources
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
    )
add_custom_command(
    TARGET
    ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/sources/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sources
)
add_custom_command(
    TARGET
    ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qml
    COMMAND ${CMAKE_COMMAND} -E copy ${QML_SOURCES} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qml
    )
