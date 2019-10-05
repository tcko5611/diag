cmake_minimum_required(VERSION 3.2)
set(CMAKE_C_COMPILER /depot/qsc/QSCO/bin/gcc)
set(CMAKE_CXX_COMPILER /depot/qsc/QSCO/bin/g++)

project(diag)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")
set(Qt5_DIR "/global/libs/qt_qscn_2017.09/5.6.2/linux64_debug_shlib/install/lib/cmake/Qt5" CACHE PATH "Initial cache" FORCE)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
add_executable(diag main.cpp mainwindow.h mainwindow.cpp mainwindow.ui
  JsonUtils.h JsonUtils.cpp QAfData.h QAfData.cpp DiagSetting.h DiagSetting.cpp
  DiagSettingDialog.ui DiagSettingDialog.h DiagSettingDialog.cpp)

target_include_directories(diag PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})
qt5_use_modules(diag Core Widgets)