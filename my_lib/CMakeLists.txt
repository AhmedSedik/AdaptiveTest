cmake_minimum_required(VERSION 3.21)
project(AdaptiveTest )

# search for OpenMP, REQUIRED -> fatal error if not found
find_package(OpenMP REQUIRED)

#add a library to the project using the specified source files
add_library(integral STATIC include/integral.h)

set_target_properties(integral PROPERTIES LINKER_LANGUAGE CXX)

# PUBLIC -> targets that link to this target get that include directory
target_include_directories(integral PUBLIC include)

# link to the imported target provided by the FindOpenMP module
target_link_libraries(integral PUBLIC OpenMP::OpenMP_CXX)
