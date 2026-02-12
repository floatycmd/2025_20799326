cmake_minimum_required(VERSION 3.15)
project(Worksheet5 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(maths SHARED adder.cpp)
set_target_properties(maths PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_include_directories(maths PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(calc calc.cpp)
target_include_directories(calc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(calc PRIVATE maths)

include(GNUInstallDirs)
install(TARGETS calc maths
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set(CPACK_PACKAGE_NAME "Worksheet5_Installer")
set(CPACK_PACKAGE_VENDOR "floatycmd")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

if(WIN32)
  set(CPACK_GENERATOR "NSIS")
endif()

include(CPack)
