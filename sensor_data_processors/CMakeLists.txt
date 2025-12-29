cmake_minimum_required(VERSION 3.23)

project(ROBOTSERVER)

add_library(sensor_data_processors)

target_include_directories(sensor_data_processors
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(sensor_data_processors
  PRIVATE
   StreamProcessor.cpp

  PUBLIC
    FILE_SET myHeaders
    TYPE HEADERS
    BASE_DIRS
      .
    FILES
      SensorDataProcessorInterface.h
      StreamProcessor.h
)