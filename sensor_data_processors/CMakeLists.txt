cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../folly-deps/installed")

find_package(folly REQUIRED)
find_package(gflags REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenCV REQUIRED)

add_library(sensor_data_processors)

add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/googletest ${CMAKE_BINARY_DIR}/googletest)

target_include_directories(sensor_data_processors
  SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(sensor_data_processors
  PRIVATE
    GTest::gmock
    Folly::folly
    SDL2::SDL2
    ${OpenCV_LIBS}
)

target_sources(sensor_data_processors
  PRIVATE
   StreamProcessor.cpp
   SensorDataProcessorMock.cpp
   SensorDataDispatcher.cpp
   SDLWorker.cpp
   WorkerManager.cpp
   UserInputProcessorWorker.cpp
   RemoteWebcamProviderWorker.cpp
   SimulatedWebcamSender.cpp
   PetDetectionWorker.cpp

  PUBLIC
    FILE_SET myHeaders
    TYPE HEADERS
    BASE_DIRS
      .
    FILES
      SensorDataWorkerInterface.h
      StreamProcessor.h
      SensorDataProcessorMock.h
      SensorDataDispatcher.h
      WorkerInterface.h
      SDLWorker.h
      WorkerManager.h
      UserInputProcessorWorker.h
      RemoteWebcamProviderWorker.h
      SimulatedWebcamSender.h
      PetDetectionWorker.h
)
