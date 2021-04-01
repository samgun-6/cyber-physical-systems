#include "../HeaderFiles/PrimeChecker.hpp"
#include <iostream>

int main (int argc, char **argv)
{
  if (argc == 2)
    {

      int number = std::stoi (argv[1]);
      PrimeChecker pc;
      std::cout << "last name, first name; " << number
                << " is a prime number? " << pc.isPrime (number) << std::endl;
    }
  return 0;
}
