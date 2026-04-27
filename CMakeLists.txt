cmake_minimum_required(VERSION 3.16)

project(worksheet6 VERSION 1.0 LANGUAGES CXX)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install path" FORCE)
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

find_package(VTK REQUIRED COMPONENTS
    CommonCore
    CommonDataModel
    FiltersCore
    IOGeometry
    IOXML
    RenderingCore
    RenderingOpenGL2
    InteractionStyle
    GUISupportQt
)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.ui

    src/ModelPart.cpp
    src/ModelPartList.cpp

    src/optiondialog.cpp
    src/optiondialog.ui

    include/mainwindow.h
    include/ModelPart.h
    include/ModelPartList.h
    include/optiondialog.h

    resources/icon.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(worksheet6
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(worksheet6
        ${PROJECT_SOURCES}
    )
endif()

target_include_directories(worksheet6
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(worksheet6
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        ${VTK_LIBRARIES}
)

vtk_module_autoinit(
    TARGETS worksheet6
    MODULES ${VTK_LIBRARIES}
)

set_target_properties(worksheet6 PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS worksheet6
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(worksheet6)
endif()

include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "worksheet6")
set(CPACK_PACKAGE_VENDOR "EEEE2076 Group Project")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt VTK CAD Viewer")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "worksheet6")

set(CPACK_NSIS_DISPLAY_NAME "worksheet6")
set(CPACK_NSIS_PACKAGE_NAME "worksheet6")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

set(CPACK_NSIS_MENU_LINKS
    "bin/worksheet6.exe" "worksheet6"
    "Uninstall.exe" "Uninstall worksheet6"
)

set(CPACK_CREATE_DESKTOP_LINKS "worksheet6")

include(CPack)