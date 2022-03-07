cmake_minimum_required(VERSION 3.21)
project(AdaptiveTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF) # disable compiler specific extensions
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

# embed library picomp in project pi
add_subdirectory(my_lib)

add_executable(AdaptiveTest main.cpp main.cpp)

# Link your application with OpenCV libraries
target_link_libraries(AdaptiveTest ${OpenCV_LIBS})

# picomp already uses OpenMP, so no need to link OpenMP again to pi
#target_link_libraries(AdaptiveTest PUBLIC integral)