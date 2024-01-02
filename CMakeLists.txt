cmake_minimum_required(VERSION 3.12)
project(NGram)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

find_package(math REQUIRED)
find_package(sampling REQUIRED)
find_package(data_structure REQUIRED)

add_executable(Test src/NGramNode.h src/NGram.h src/SimpleSmoothing.h src/TrainedSmoothing.h src/NoSmoothing.h src/NoSmoothingWithDictionary.h src/NoSmoothingWithNonRareWords.h src/LaplaceSmoothing.h src/GoodTuringSmoothing.h src/AdditiveSmoothing.h src/InterpolatedSmoothing.h Test/NGramTest.cpp Test/NoSmoothingTest.cpp Test/ReadCorpus.h Test/ReadCorpus.cpp Test/LaplaceSmoothingTest.cpp Test/GoodTuringSmoothingTest.cpp Test/AdditiveSmoothingTest.cpp Test/InterpolatedSmoothingTest.cpp)
target_link_libraries(Test math::math sampling::sampling data_structure::data_structure)
