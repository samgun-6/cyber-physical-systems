cmake_minimum_required (VERSION 3.2) 

project (primecheckerSource)
add_library(primechecker main.cpp PrimeChecker.cpp)
target_include_directories(primechecker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable (${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/PrimeChecker.cpp)
