#include "Dates.h"

class Account
{
public:
  string name;
  int accountNumber;
  string pin;
  int address;
  int accountType;
  double balance;
  int FDtime;
  vector<pair<double, vector<string>>> transactions; 
  double dailyDepositLimit;
  double dailyWithdrawalLimit;
  string creationDate;
  string lastInterestDate;
  bool hasLoan;
  string loanBorrowDate;
  int loanType;
  int loanAmount;
  int loanDuration;

  Account() : name(""), accountNumber(0), pin(""), address(1), accountType(1), balance(0),
              dailyDepositLimit(0), dailyWithdrawalLimit(0), FDtime(0), loanType(1), loanAmount(0), loanDuration(0), hasLoan(0), loanBorrowDate("") {}

  Account(string name, int accountNumber, string pin, int address, int FDtime, int accountType, double balance, string creationDate)
      : name(name), accountNumber(accountNumber), pin(pin), address(address), FDtime(FDtime),
        accountType(accountType), balance(balance), creationDate(creationDate), lastInterestDate(creationDate), loanType(NONE), loanAmount(0), loanDuration(0), hasLoan(0), loanBorrowDate("")
  {

    
    setAccountLimits(accountType);
  }

  void setAccountLimits(int accountType)
  {
    switch (accountType)
    {
    case SAVINGS:
    case SALARY:
      dailyDepositLimit = 50000;
      dailyWithdrawalLimit = 50000;
      break;
    case CURRENT:
      dailyDepositLimit = 200000;
      dailyWithdrawalLimit = 200000;
      break;
    case FDA:
      dailyDepositLimit = 0; // FDA does not allow deposits/withdrawals
      dailyWithdrawalLimit = 0;
      break;
    }
  }

  void displayAccountInfo()
  {
    cout << endl;
    cout << "Account Number: " << accountNumber << endl
         << endl;
    cout << "Account Holder Name: " << name << endl
         << endl;
    cout << "Account Type: " << getAccountTypeString(accountType) << endl
         << endl;
    cout << "Address: " << getAddressString(address) << endl
         << endl;
    cout << "Balance: ₹" << balance << endl
         << endl;
    cout << "Account Created on: " << creationDate << endl
         << endl; // Display creation date
    if (accountType == FDA)
    {
      cout << "Maturity Duration: " << FDtime << endl
           << endl;
      cout << "Maturity Date: " << addMonthsToDate(transactions.back().second[1], FDtime) << endl
           << endl;
    }
    else if (hasLoan)
    {
      cout << "Loan Type: " << loanType << endl
           << endl;
      cout << "Principal Amount: ₹" << loanAmount << endl
           << endl;
      cout << "Loan Duration: " << loanDuration << " months" << endl
           << endl;
    }
    cout << "Transactions: \n";
    for (auto &transaction : transactions)
    {
      cout << transaction.second[0] << " on " << transaction.second[1] << " - ₹" << transaction.first << endl
           << endl;
    }
  }

  string getAccountTypeString(int accountType)
  {
    switch (accountType)
    {
    case CURRENT:
      return "Current";
    case SAVINGS:
      return "Savings";
    case SALARY:
      return "Salary";
    case FDA:
      return "FDA";
    default:
      return "Unknown";
    }
  }

  string getAddressString(int address)
  {
    switch (address)
    {
    case 1:
      return "Mumbai";
    case 2:
      return "Delhi";
    case 3:
      return "Kolkata";
    case 4:
      return "Guwahati";
    case 5:
      return "Bengaluru";
    case 6:
      return "Chandigarh";
    case 7:
      return "Panaji";
    default:
      return "Unknown";
    }
  }

  void addTransaction(double amount, const string &transactionType, const string &date)
  {
    vector<string> v;
    v.push_back(transactionType);
    v.push_back(date);
    if (amount < 0)
      amount *= -1;
    transactions.push_back(make_pair(amount, v));
  }

  static double getMinimumDeposit(int accountType)
  {
    switch (accountType)
    {
    case CURRENT:
      return 5000;
    case SAVINGS:
      return 500;
    case SALARY:
      return 0;
    case FDA:
      return 1000;
    default:
      return 0;
    }
  }

  double calculateTotalDepositsOnDate(const string &date)
  {
    double totalDeposits = 0;
    for (const auto &transaction : transactions)
    {
      if (transaction.second[1] == date && transaction.first > 0)
      {
        totalDeposits += transaction.first;
      }
    }
    return totalDeposits;
  }

  double calculateTotalWithdrawalsOnDate(const string &date)
  {
    double totalWithdrawals = 0;
    for (const auto &transaction : transactions)
    {
      if (transaction.second[1] == date && transaction.first < 0)
      {
        totalWithdrawals += -transaction.first; // Convert negative withdrawal to positive for total
      }
    }
    return totalWithdrawals;
  }

  void calculateAndUpdateInterest(const string &currentDate)
  {
    if (accountType == FDA)
    {
      return;
    }

    double interestRate = getInterestRate(SAVINGS);
    double totalInterest = 0.0;
    string startDate = lastInterestDate.empty() ? creationDate : lastInterestDate;
    string endDate = currentDate;

    while (addMonthsToDate(startDate, 1) <= endDate)
    {
      string nextDate = addMonthsToDate(startDate, 1);

      // Handle loan repayment if loan duration has ended
      if (hasLoan && compareDates(addMonthsToDate(loanBorrowDate, loanDuration), startDate) <= 0)
      {
        double amount = loanAmount + loanAmount * (getLoanInterestRate(static_cast<LoanType>(loanType)) / 12) * loanDuration / 100;
        double initial_balance = balance;
        balance -= min(balance, amount);
        addTransaction(-min(initial_balance, amount), "Loan Repayment", addMonthsToDate(loanBorrowDate, loanDuration));
        hasLoan = false;
      }

      if (accountType==SAVINGS)
      {
        double avgBalance = calculateAverageBalance(startDate, nextDate);

        double monthlyInterest = (avgBalance * (interestRate / 12)) / 100;
        totalInterest += monthlyInterest;

        // Record transaction for interest payout
        addTransaction(monthlyInterest, "Interest Payout", nextDate);
      }

      startDate = nextDate;
    }

    // Handle loan repayment if loan duration has ended after the loop
    if (hasLoan && compareDates(addMonthsToDate(loanBorrowDate, loanDuration), startDate) <= 0)
    {
      double amount = loanAmount + loanAmount * (getLoanInterestRate(static_cast<LoanType>(loanType)) / 12) * loanDuration / 100;
      double initial_balance = balance;
      balance -= min(balance, amount);
      addTransaction(-min(initial_balance, amount), "Loan Repayment", addMonthsToDate(loanBorrowDate, loanDuration));
      hasLoan = false;
    }

    // Add total interest to balance
    balance += totalInterest;
    lastInterestDate = addMonthsToDate(startDate, -1); // Update last interest date
  }

  double calculateAverageBalance(const string &startDate, const string &endDate)
  {
    double totalBalance = 0.0;
    int days = 0;
    string currentDate = startDate;

    while (currentDate < endDate)
    {
      totalBalance += getBalanceOnDate(currentDate);
      currentDate = addDaysToDate(currentDate, 1);
      days++;
    }

    return totalBalance / days;
  }

  double getBalanceOnDate(const string &date)
  {
    double balanceOnDate = 0.0;
    for (const auto &transaction : transactions)
    {
      if (transaction.second[1] <= date)
      {
        balanceOnDate += transaction.first;
      }
      else
      {
        break;
      }
    }
    return balanceOnDate;
  }

  string addDaysToDate(const string &date, int daysToAdd)
  {
    struct tm date_tm = {0};
    stringstream ss(date);
    ss >> get_time(&date_tm, "%Y-%m-%d");

    time_t time = mktime(&date_tm);
    time += daysToAdd * 24 * 60 * 60; 

    date_tm = *localtime(&time);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &date_tm);

    return string(buffer);
  }
};