#define CATCH_CONFIG_MAIN

#include "../HeaderFiles/catch.hpp"
#include "../HeaderFiles/PrimeChecker.hpp"

TEST_CASE("Test PrimeChecker 1.") {
    PrimeChecker pc;
    REQUIRE(pc.isPrime(5));
}
