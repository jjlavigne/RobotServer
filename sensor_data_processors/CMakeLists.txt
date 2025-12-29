cmake_minimum_required(VERSION 3.23)

project(ROBOTSERVER)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sensor_data_processors)

target_include_directories(sensor_data_processors
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(sensor_data_processors
  PRIVATE
    GTest::gmock
)

target_sources(sensor_data_processors
  PRIVATE
   StreamProcessor.cpp
   SensorDataProcessorMock.cpp
   SensorDataDispatcher.cpp

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
)