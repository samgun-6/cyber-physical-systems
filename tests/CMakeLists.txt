
project (testsPrimeChecker)

enable_testing()
add_executable(${PROJECT_NAME} TestPrimeChecker.cpp)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC primechecker)
add_test(NAME ${PROJECT_NAME}1 COMMAND ${PROJECT_NAME})
add_test(NAME ${PROJECT_NAME}2 COMMAND ${PROJECT_NAME})
add_test(NAME ${PROJECT_NAME}3 COMMAND ${PROJECT_NAME})
add_test(NAME ${PROJECT_NAME}4 COMMAND ${PROJECT_NAME})






    

