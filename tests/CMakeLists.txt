
project (testsPrimeChecker)

enable_testing()
add_executable(${PROJECT_NAME} TestPrimeChecker.cpp)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC primechecker)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})



    

