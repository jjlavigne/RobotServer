cmake_minimum_required(VERSION 3.23)

project(ROBOTSERVER)

add_library(sensor_data_processors)

target_sources(sensor_data_processors
  PRIVATE
   StreamProcessor.cpp

  PUBLIC
    FILE_SET myHeaders
    TYPE HEADERS
    BASE_DIRS
      include
    FILES
      include/SensorDataProcessorsInterface.h
      include/StreamProcessor.h
)