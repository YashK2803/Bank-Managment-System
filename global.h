#include <bits/stdc++.h>

using namespace std;

//Enum for account types
enum AccountType
{
  CURRENT = 1,
  SAVINGS,
  SALARY,
  FDA
};

// Enum for addresses
enum Address
{
  MUMBAI = 1,
  DELHI,
  KOLKATA,
  GUWAHATI,
  BENGALURU,
  CHANDIGARH,
  PANAJI
};

//enum for loan types
enum LoanType
{
  PERSONAL = 1,
  HOME,
  VEHICLE,
  EDUCATION,
  NONE

};

struct Coordinate
{
  double x;
  double y;
};

double getInterestRate(AccountType accountType)
{
  switch (accountType)
  {
  case CURRENT:
    return 0.0; // No interest for CURRENT account
  case SAVINGS:
    return 5.0; // 5% interest for SAVINGS account
  case SALARY:
    return 0.0; // No interest for SALARY account
  case FDA:
    return 6.0; // 6% interest for FDA account
  default:
    return 0.0; // Default to no interest if an invalid account type is provided
  }
}

double getLoanInterestRate(int loanType)
{
  switch (loanType)
  {
  case PERSONAL:
    return 10.0;
  case HOME:
    return 7.0;
  case VEHICLE:
    return 9.0;
  case EDUCATION:
    return 12.0;
  default:
    return 0.0;
  }
}