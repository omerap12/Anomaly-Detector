cmake_minimum_required(VERSION 3.20)
project(advanced_programming_ex6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(advanced_programming_ex5 MainTrain.cpp anomaly_detection_util.cpp anomaly_detection_util.h
        AnomalyDetector.h CLI.cpp CLI.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h commands.h minCircle.h
        minCircle.cpp SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.h timeseries.cpp Server.cpp Server.h)
