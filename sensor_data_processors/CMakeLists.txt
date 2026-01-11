cmake_minimum_required(VERSION 3.23)

project(ROBOTSERVER)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../folly-deps/installed")

find_package(folly REQUIRED)
find_package(gflags REQUIRED)
find_package(Curses REQUIRED)
find_package(SDL2 REQUIRED)

add_library(sensor_data_processors)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../googletest ${CMAKE_BINARY_DIR}/googletest)

target_include_directories(sensor_data_processors
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sensor_data_processors
  PRIVATE
    GTest::gmock
    Folly::folly
    SDL2::SDL2
    ${CURSES_LIBRARIES}
)

target_sources(sensor_data_processors
  PRIVATE
   StreamProcessor.cpp
   SensorDataProcessorMock.cpp
   SensorDataDispatcher.cpp
   SensorDataProviderInterface.cpp
   UserInputProvider.cpp
   SensorDataManager.cpp
   UserInputProcessor.cpp

  PUBLIC
    FILE_SET myHeaders
    TYPE HEADERS
    BASE_DIRS
      .
    FILES
      SensorDataProcessorInterface.h
      StreamProcessor.h
      SensorDataProcessorMock.h
      SensorDataDispatcher.h
      SensorDataProviderInterface.h
      UserInputProvider.h
      SensorDataManager.h
      UserInputProcessor.h
)