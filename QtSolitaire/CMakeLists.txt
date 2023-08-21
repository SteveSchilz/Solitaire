cmake_minimum_required(VERSION 3.5)

project(QtSolitaire VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS SvgWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS SvgWidgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.h    mainwindow.cpp    mainwindow.ui
        myscene.h
        card.h          card.cpp
        cardstack.h     cardstack.cpp
        constants.h
        enumiterator.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtSolitaire
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}        
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QtSolitaire APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QtSolitaire SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QtSolitaire
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QtSolitaire PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::SvgWidgets
)

set_target_properties(QtSolitaire PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS QtSolitaire
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtSolitaire)
endif()

# Resources (Has to be after add_executable, because resources are added to QtSolitaire exe)
set(solitaire_resource_files
    "images/King-Hearts.svg"
    "images/Queen-Hearts.svg"
    "images/Jack-Hearts.svg"
    "images/King-Diamonds.svg"
    "images/Queen-Diamonds.svg"
    "images/Jack-Diamonds.svg"
    "images/King-Spades.svg"
    "images/Queen-Spades.svg"
    "images/Jack-Spades.svg"
    "images/King-Clubs.svg"
    "images/Queen-Clubs.svg"
    "images/Jack-Clubs.svg"
)

qt_add_resources(QtSolitaire "Images"
    PREFIX
        "/"
    FILES
    ${solitaire_resource_files}
)
