cmake_minimum_required(VERSION 3.12)
project(NGram)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
set(CONAN_INCLUDE_DIRS_MATH "/Users/olcay/.conan/data/Math/1.0.0/_/_/package/1aff124b71f6d938138ecd7179ff075f4304a85d/include")

add_executable(Test src/NGramNode.h src/NGram.h src/SimpleSmoothing.h src/TrainedSmoothing.h src/NoSmoothing.h src/NoSmoothingWithDictionary.h src/NoSmoothingWithNonRareWords.h src/LaplaceSmoothing.h src/GoodTuringSmoothing.h src/AdditiveSmoothing.h src/InterpolatedSmoothing.h Test/NGramTest.cpp Test/NoSmoothingTest.cpp Test/ReadCorpus.h Test/ReadCorpus.cpp Test/LaplaceSmoothingTest.cpp Test/GoodTuringSmoothingTest.cpp Test/AdditiveSmoothingTest.cpp Test/InterpolatedSmoothingTest.cpp)
target_link_libraries(Test ${CONAN_LIBS_MATH})
