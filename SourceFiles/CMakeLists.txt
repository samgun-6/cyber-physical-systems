cmake_minimum_required (VERSION 3.2) 
project (primechecker)
add_executable (${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/PrimeChecker.cpp)
