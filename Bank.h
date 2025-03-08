#include "Account.h"

class Bank
{
public:
  map<int, Account> accounts;
  vector<vector<double>> distanceMatrix;

  void createAccount();
  void checkBalance();
  void depositMoney();
  void withdrawMoney();
  void checkTransactionHistory();
  void deleteAccount();
  void showHomePage();
  void returnToHomePage();
  void getAccountInfo();
  int nearestCity(int city, const vector<vector<double>> &distanceMatrix);
  void transferAccount();
  void borrowLoan();
  vector<Coordinate> generateRandomCoordinates(int numCities);
  vector<vector<double>> createDistanceMatrix(const vector<Coordinate> &coordinates);
  vector<Coordinate> cityCoordinates;
  map<string, int> SalaryNameCount;
  map<string, int> SavingsNameCount;
  map<string, int> CurrentNameCount;

private:
  int generateAccNo();
  void getChoice(int &choice, int min, int max);
  bool checkPin(const string &pin);
  bool checkInitialDep(double deposit, int accountType);
  bool askReturnHomePage(string task);
  void clearScreen();
  double calculateDistance(const Coordinate &a, const Coordinate &b);
  vector<double> dijkstra(int city, const vector<vector<double>>& distanceMatrix);
};

void Bank::borrowLoan()
{
  if (askReturnHomePage("borrow a loan"))
    return;
  int accountNumber;
  string pin;
  clearScreen();

  cout << "\n\t\t\t\t\t\t\t***Borrow Loan***\n";
  // Ask for account number
  cout << "\nEnter your account number: ";
  cin >> accountNumber;

  // Check if account exists
  if (accounts.find(accountNumber) != accounts.end())
  {
    // Ask for PIN
    cout << "Enter PIN: ";
    cin >> pin;

    // Validate PIN
    if (accounts[accountNumber].pin == pin)
    {
      Account &account = accounts[accountNumber];
      if (account.hasLoan)
      {
        cout << "A loan is already active under this account!";
      }
      else if (account.accountType == FDA)
      {
        cout << "You cannot take a loan on FD Account!" << endl;
      }
      else
      {
        cout << endl;
        string date;
        cout << "\nEnter date (YYYY-MM-DD): ";
        cin >> date;
        while (!validateDate(date))
        {
          cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
          cin >> date;
        }
        string last_date = account.transactions[account.transactions.size() - 1].second[1];
        while (check_dates(date, last_date) == -1)
        {
          cout << "Last transaction was made on " << last_date << endl;
          cout << "Please enter a date after that: ";
          cin >> date;
          while (!validateDate(date))
          {
            cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
            cin >> date;
          }
        }
        int loanTypeChoice;
        cout << "1. The Principal Amount should range between ₹0 and ₹10,000\n";
        cout << "2. The Duration should range from 1 to 12 months\n";
        cout << "3. An account can only borrow on loan at a time.\n";
        cout << "4. The final repayment amount will be deducted from the account at the end of loan duration.\n";
        cout << "\nChoose loan type:\n";
        cout << "1. Personal Loan: 10% pa Simple Interest\n";
        cout << "2. Home Loan: 7% pa Simple Interest\n";
        cout << "3. Vehicle Loan: 9% pa Simple Interest\n";
        cout << "4. Education Loan: 12% pa Simple Interest\n";
        cout << "Enter your choice: ";
        getChoice(loanTypeChoice, PERSONAL, EDUCATION);
        account.loanType = loanTypeChoice;
        cout << "Enter Principal Amount (Maximum Limit: ₹10,000): ₹";
        int amount = 0;
        cin >> amount;
        while (amount > 10000 || amount <= 0)
        {
          cout << "Invalid Amount! It should range between ₹0 and ₹10,000" << endl;
          cout << "Please enter valid Principal Amount: ₹";
          cin >> amount;
        }
        cout << "Enter duration(in months from 1 to 12): ";
        int duration;
        cin >> duration;
        while (duration < 1 || duration > 12)
        {
          cout << "Invalid Time! It should range between 1 and 12 months" << endl;
          cout << "Please enter valid duration: ";
          cin >> duration;
        }

        account.loanAmount = amount;
        account.loanDuration = duration;
        account.balance += amount;
        account.loanBorrowDate = date;
        account.addTransaction(amount, "Loan Deposit", date);
        account.hasLoan = true;
        cout << "\nLoan approved. ₹" << amount << " has been deposited into your account." << endl;
      }
    }
    else
    {
      cout << "Incorrect PIN. Please try again.\n";
    }
  }
  else
  {
    cout << "Account not found.\n";
  }

  cout << "\nPress any key to return to the home page...";
  cin.ignore();
  cin.get();
  showHomePage();
}

int Bank::generateAccNo()
{
  static int accountNumber = 1001; // Start from a base account number
  return accountNumber++;
}

void Bank::getChoice(int &choice, int min, int max)
{
  while (true)
  {
    cin >> choice;
    if (choice >= min && choice <= max)
      break;
    else
    {
      cout << "Invalid choice. Please enter a number between " << min << " and " << max << ": ";
    }
  }
}

void Bank::clearScreen()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

bool Bank::askReturnHomePage(string task)
{
  char choice = 'a';
  while (choice != 'Y' && choice != 'y' && choice != 'n' && choice != 'N')
  {
    cout << "Do you want to " << task << "? (Y/N): ";
    cin >> choice;
  }
  if (choice == 'Y' || choice == 'y')
    return false;
  return true;
}

bool Bank::checkPin(const string &pin)
{
  return (pin.length() >= 4 && pin.length() <= 6);
}

bool Bank::checkInitialDep(double deposit, int accountType)
{
  double minDeposit = Account::getMinimumDeposit(accountType);
  return deposit >= minDeposit;
}

void Bank::showHomePage()
{
  int choice = 0;
  while (true)
  {
    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***** National Bank of India *****\n";
    cout << "\t\t\t\t\t\t\t\t By Yash Kharangate\n"
         << endl;
    cout << "\n Welcome!\n";
    cout << "1. Create Account\n";
    cout << "2. Check Balance\n";
    cout << "3. Deposit Money\n";
    cout << "4. Withdraw Money\n";
    cout << "5. Check Account Details\n";
    cout << "6. Check Transaction History\n";
    cout << "7. Delete Account\n";
    cout << "8. Transfer Account to new Branch\n";
    cout << "9. Borrow Loan\n";
    cout << "10. Exit\n";
    cout << "Enter your choice: ";
    getChoice(choice, 1, 10);

    switch (choice)
    {
    case 1:
      createAccount();
      break;
    case 2:
      checkBalance();
      break;
    case 3:
      depositMoney();
      break;
    case 4:
      withdrawMoney();
      break;
    case 5:
      getAccountInfo();
      break;
    case 6:
      checkTransactionHistory();
      break;

    case 7:
      deleteAccount();
      break;

    case 8:
      transferAccount();
      break;

    case 9:
      borrowLoan();
      break;

    case 10:
      askReturnHomePage("exit the system");
      cout << "\nThank You for your time with the National Bank of India!\n";
      cout << "Exiting the system.\n";
      exit(0);
    }
  }
}

vector<double> Bank::dijkstra(int city, const vector<vector<double>>& distanceMatrix) {
    int numCities = distanceMatrix.size();
    vector<double> minDistances(numCities, numeric_limits<double>::max());
    vector<bool> visited(numCities, false);
    
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    
    pq.push({0.0, city});
    minDistances[city] = 0.0;

    while (!pq.empty()) {
        int currentCity = pq.top().second;
        pq.pop();

        if (visited[currentCity]) continue;
        visited[currentCity] = true;

        for (int neighbor = 0; neighbor < numCities; ++neighbor) {
            if (distanceMatrix[currentCity][neighbor] != numeric_limits<double>::max() && !visited[neighbor]) {
                double newDist = minDistances[currentCity] + distanceMatrix[currentCity][neighbor];
                if (newDist < minDistances[neighbor]) {
                    minDistances[neighbor] = newDist;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }
    
    return minDistances;
}

int Bank::nearestCity(int city, const vector<vector<double>>& distanceMatrix) {
    vector<double> minDistances = dijkstra(city, distanceMatrix);
    
    double minDistance = numeric_limits<double>::max();
    int nearestCity = -1;

    for (int i = 0; i < minDistances.size(); ++i) {
        if (i != city && minDistances[i] < minDistance) {
            minDistance = minDistances[i];
            nearestCity = i;
        }
    }

    return nearestCity;
}

void Bank::transferAccount()
{
  if (askReturnHomePage("transfer you account"))
    return;
  int accountNumber;
  string pin;
  clearScreen();

  cout << "\n\t\t\t\t\t\t\t***Transfer Account***\n";
  // Ask for account number
  cout << "\nEnter your account number: ";
  cin >> accountNumber;

  // Check if account exists
  if (accounts.find(accountNumber) != accounts.end())
  {
    // Ask for PIN
    cout << "Enter PIN: ";
    cin >> pin;

    // Validate PIN
    if (accounts[accountNumber].pin == pin)
    {
      // Account found and PIN is correct, display account info
      Account &account = accounts[accountNumber];
      int addressChoice;
      cout << "Nearest Bank is present in " << account.getAddressString(nearestCity(account.address - 1, distanceMatrix) + 1) << "!" << endl
           << endl;
      cout << "\nChoose new address:\n1. Mumbai (Coordinates: ("<<cityCoordinates[0].x<<", "<<cityCoordinates[0].y<<"))\n";
      cout << "2. Delhi (Coordinates: ("<<cityCoordinates[1].x<<", "<<cityCoordinates[1].y<<"))\n";
      cout << "3. Kolkata (Coordinates: ("<<cityCoordinates[2].x<<", "<<cityCoordinates[2].y<<"))\n";
      cout << "4. Guwahati (Coordinates: ("<<cityCoordinates[3].x<<", "<<cityCoordinates[3].y<<"))\n";
      cout << "5. Bengaluru (Coordinates: ("<<cityCoordinates[4].x<<", "<<cityCoordinates[4].y<<"))\n";
      cout << "6. Chandigarh (Coordinates: ("<<cityCoordinates[5].x<<", "<<cityCoordinates[5].y<<"))\n";
      cout << "7. Panaji (Coordinates: ("<<cityCoordinates[6].x<<", "<<cityCoordinates[6].y<<"))\n\n";
      cout << "Enter your choice: ";
      getChoice(addressChoice, MUMBAI, PANAJI);
      account.address = addressChoice;

      cout << "Account Transfer Successful!" << endl;
    }
    else
    {
      cout << "Incorrect PIN. Please try again.\n";
    }
  }
  else
  {
    cout << "Account not found.\n";
  }

  cout << "\nPress any key to return to the home page...";
  cin.ignore();
  cin.get();
  showHomePage();
}

void Bank::getAccountInfo()
{
  if (askReturnHomePage("view account information"))
    return;
  int accountNumber;
  string pin;
  clearScreen();

  cout << "\n\t\t\t\t\t\t\t***Account Information***\n";
  // Ask for account number
  cout << "\nEnter your account number: ";
  cin >> accountNumber;

  // Check if account exists
  if (accounts.find(accountNumber) != accounts.end())
  {
    // Ask for PIN
    cout << "Enter PIN: ";
    cin >> pin;

    // Validate PIN
    if (accounts[accountNumber].pin == pin)
    {
      // Account found and PIN is correct, display account info
      Account &account = accounts[accountNumber];
      account.displayAccountInfo();
    }
    else
    {
      cout << "Incorrect PIN. Please try again.\n";
    }
  }
  else
  {
    cout << "Account not found.\n";
  }

  cout << "\nPress any key to return to the home page...";
  cin.ignore();
  cin.get();
  showHomePage();
}

void Bank::createAccount()
{
  string name, pin, confirmPin;
  int addressChoice, accountTypeChoice;
  double initialDeposit;
  string creationDate;

    if (askReturnHomePage("create an account"))
      return;

    clearScreen();
    cout << "\n\t\t\t\t\t\t\t***Create Account***\n";
    cout << "\nEnter account holder's name: ";
    cin.ignore();
    getline(cin, name);
    string lower_name = name;
    for (char &c : lower_name)
    {
      c = std::tolower(static_cast<unsigned char>(c));
    }
    cout << "\nChoose account type:\n";
    cout << "1. Current Account\n\n";
    cout << "- Minimum Initial Deposit: ₹5000\n";
    cout << "- 0%pa interest rate\n";
    cout << "- Daily Deposit Limit: ₹200000\n";
    cout << "- Daily Withdrawal Limit: ₹200000\n";
    cout << "- There can be at-most 1 current account under a name\n\n";
    cout << "2. Savings Account\n\n";
    cout << "- Minimum Initial Deposit: ₹500\n";
    cout << "- 5%pa interest rate (Monthly Interest Payout)\n";
    cout << "- Daily Deposit Limit: ₹50000\n";
    cout << "- Daily Withdrawal Limit: ₹50000\n";
    cout << "- There can be at-most 3 savings accounts under a name\n\n";
    cout << "3. Salary Account\n\n";
    cout << "- Minimum Initial Deposit: ₹0\n";
    cout << "- 0%pa interest rate \n";
    cout << "- Daily Deposit Limit: ₹50000\n";
    cout << "- Daily Withdrawal Limit: ₹50000\n";
    cout << "- There can be at-most 1 salary account under a name\n\n";
    cout << "4. Fixed Deposit Account\n\n";
    cout << "- Minimum Deposit: ₹1000\n";
    cout << "- 6%pa interest rate\n";
    cout << "- Cannot Deposit/Withdraw money before maturity\n";
    cout << "- There is no limit on number of Fixed Deposit accounts under a name\n\n";
    cout << "Enter your choice: ";
    getChoice(accountTypeChoice, CURRENT, FDA);
    int f = 0;
    if (accountTypeChoice == SAVINGS && SalaryNameCount[lower_name] >= 3)
    {
      cout << "There are already 3 Savings Accounts under the name of " << lower_name << "! " << endl;
      cout << "A person can have at-most 3 Savings Accounts!" << endl;
      f = 1;
    }

    else if (accountTypeChoice == SALARY && SalaryNameCount[lower_name] >= 1)
    {
      cout << "There is already a Salary Account under the name of " << lower_name << "! " << endl;
      cout << "A person can have at-most 1 Salary Account!" << endl;
      f = 1;
    }

    else if (accountTypeChoice == CURRENT && CurrentNameCount[lower_name] >= 1)
    {
      cout << "There is already a Current Account under the name of " << lower_name << "! " << endl;
      cout << "A person can have at-most 1 Current Account!" << endl;
      f = 1;
    }

    if (!f)
    {
      if (accountTypeChoice == SALARY)
      {
        SalaryNameCount[lower_name]++;
      }
      else if (accountTypeChoice == SAVINGS)
      {
        SavingsNameCount[lower_name]++;
      }
      else if (accountTypeChoice == CURRENT)
      {
        CurrentNameCount[lower_name]++;
      }
      cout << "\nEnter 4-6 digit PIN: ";
      cin >> pin;
      if (!checkPin(pin))
      {
        cout << "Invalid PIN. The PIN must be between 4 and 6 digits. Please try again: ";
        cin >> pin;
      }

      cout << "\nConfirm PIN: ";
      cin >> confirmPin;
      while (pin != confirmPin)
      {
        cout << "PINs do not match. Please re-enter the PIN: ";
        cin >> pin;
        while (!checkPin(pin))
        {
          cout << "Invalid PIN. The PIN must be between 4 and 6 digits. Please try again: ";
          cin >> pin;
        }
        cout << "\nConfirm PIN: ";
        cin >> confirmPin;
      }

      cout << "\nEnter account creation date (YYYY-MM-DD): ";
      cin >> creationDate;
      while (!validateDate(creationDate))
      {
        cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
        cin >> creationDate;
      }

      cout << "\nChoose address:\n1. Mumbai (Coordinates: ("<<cityCoordinates[0].x<<", "<<cityCoordinates[0].y<<"))\n";
      cout << "2. Delhi (Coordinates: ("<<cityCoordinates[1].x<<", "<<cityCoordinates[1].y<<"))\n";
      cout << "3. Kolkata (Coordinates: ("<<cityCoordinates[2].x<<", "<<cityCoordinates[2].y<<"))\n";
      cout << "4. Guwahati (Coordinates: ("<<cityCoordinates[3].x<<", "<<cityCoordinates[3].y<<"))\n";
      cout << "5. Bengaluru (Coordinates: ("<<cityCoordinates[4].x<<", "<<cityCoordinates[4].y<<"))\n";
      cout << "6. Chandigarh (Coordinates: ("<<cityCoordinates[5].x<<", "<<cityCoordinates[5].y<<"))\n";
      cout << "7. Panaji (Coordinates: ("<<cityCoordinates[6].x<<", "<<cityCoordinates[6].y<<"))\n\n";
      cout << "Enter your choice: ";
      getChoice(addressChoice, MUMBAI, PANAJI);

      cout << "\nEnter initial deposit amount: ₹";
      cin >> initialDeposit;
      while (!checkInitialDep(initialDeposit, accountTypeChoice))
      {
        cout << "Initial deposit does not meet the minimum requirement. Please enter a valid amount: ₹";
        cin >> initialDeposit;
      }
      int FDtime = 0;
      if (accountTypeChoice == FDA)
      {
        cout << "Enter tenure of deposit(in months): ";
        cin >> FDtime;
      }

      int accountNumber = generateAccNo();
      Account newAccount(name, accountNumber, pin, addressChoice, FDtime, accountTypeChoice, initialDeposit, creationDate);
      accounts[accountNumber] = newAccount;
      accounts[accountNumber].addTransaction(initialDeposit, "Initial Deposit", creationDate);
      cout << "Account created successfully. Account Number: " << accountNumber << endl;
    }
    cout << "Press any key to return to the home page...";
    cin.ignore();
    cin.get();
    showHomePage();
}
vector<Coordinate> generateRandomCoordinates(int numCities)
{
  vector<Coordinate> coordinates(numCities);
  srand(time(0));
  for (int i = 0; i < numCities; ++i)
  {
    coordinates[i] = {static_cast<double>(rand() % 101), static_cast<double>(rand() % 101)};
  }
  return coordinates;
}

// Function to calculate Euclidean distance between two points
double calculateDistance(const Coordinate &a, const Coordinate &b)
{
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Function to create the distance matrix
vector<vector<double>> createDistanceMatrix(const vector<Coordinate> &coordinates)
{
  int numCities = coordinates.size();
  vector<vector<double>> distanceMatrix(numCities, vector<double>(numCities, 0.0));
  for (int i = 0; i < numCities; ++i)
  {
    for (int j = i + 1; j < numCities; ++j)
    {
      double distance = calculateDistance(coordinates[i], coordinates[j]);
      distanceMatrix[i][j] = distanceMatrix[j][i] = distance;
    }
  }
  return distanceMatrix;
}

void Bank::checkBalance()
{
  if (askReturnHomePage("check balance"))
    return;
  int accountNumber;
  string pin;
  clearScreen();
  cout << "\n\t\t\t\t\t\t\t***Check Balance***\n";
  cout << "\nEnter your account number: ";
  cin >> accountNumber;

  if (accounts.find(accountNumber) != accounts.end())
  {
    cout << "Enter PIN: ";
    cin >> pin;
    if (accounts[accountNumber].pin == pin)
    {
      Account &account = accounts[accountNumber];
      cout << "Balance for Account Number " << accountNumber << ": ₹" << account.balance << endl;
    }
    else
    {
      cout << "Incorrect PIN. \n";
    }
  }
  else
  {
    cout << "Account not found.\n";
  }

  cout << "\nPress any key to return to the home page...";
  cin.ignore();
  cin.get();
  showHomePage();
}

void Bank::depositMoney()
{
  if (askReturnHomePage("deposit money"))
    return;
  int accountNumber;
  string pin;
  string date;
  double amount;
  clearScreen();

  cout << "\n\t\t\t\t\t\t\t***Deposit Money***\n";
  cout << "\nEnter your account number: ";
  cin >> accountNumber;

  if (accounts.find(accountNumber) != accounts.end())
  {
    cout << "Enter PIN: ";
    cin >> pin;
    if (accounts[accountNumber].pin == pin)
    {
      Account &account = accounts[accountNumber];
      if (account.accountType == FDA)
      {
        cout << "You cannot deposit money in FD Account!" << endl;
      }
      else
      {
        cout << "\nEnter date (YYYY-MM-DD): ";
        cin >> date;
        while (!validateDate(date))
        {
          cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
          cin >> date;
        }

        string last_date = account.transactions[account.transactions.size() - 1].second[1];
        while (check_dates(date, last_date) == -1)
        {
          cout << "Last transaction was made on " << last_date << endl;
          cout << "Please enter a date after that: ";
          cin >> date;
          while (!validateDate(date))
          {
            cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
            cin >> date;
          }
        }
        // Calculate the total deposits on the given date
        double totalDepositsOnDate = account.calculateTotalDepositsOnDate(date);
        cout << "Total deposits made today: ₹" << totalDepositsOnDate << endl;

        cout << "Enter the amount to deposit: ₹";
        cin >> amount;

        if (totalDepositsOnDate + amount > account.dailyDepositLimit)
        {
          cout << "Error: You have exceeded your daily deposit limit.\n";
        }
        else
        {
          account.calculateAndUpdateInterest(date);
          account.balance += amount;
          account.addTransaction(amount, "Deposit", date);
          cout << "₹" << amount << " deposited successfully.\n";
        }
      }
    }
    else
    {
      cout << "Incorrect PIN.\n";
    }
  }
  else
  {
    cout << "Account not found.\n";
  }

  cout << "\nPress any key to return to the home page...";
  cin.ignore();
  cin.get();
  showHomePage();
}

void Bank::withdrawMoney()
{
  if (askReturnHomePage("withdraw money"))
    return;
  int accountNumber;
  string pin;
  string date;
  double amount;
  clearScreen();

  cout << "\n\t\t\t\t\t\t\t***Withdraw Money***\n";
  cout << "\nEnter your account number: ";
  cin >> accountNumber;

  if (accounts.find(accountNumber) != accounts.end())
  {
    cout << "Enter PIN: ";
    cin >> pin;
    if (accounts[accountNumber].pin == pin)
    {
      Account &account = accounts[accountNumber];
      cout << "\nEnter date (YYYY-MM-DD): ";
      cin >> date;
      while (!validateDate(date))
      {
        cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
        cin >> date;
      }
      string last_date = account.transactions[account.transactions.size() - 1].second[1];
      while (check_dates(date, last_date) == -1)
      {
        cout << "Last transaction was made on " << last_date << endl;
        cout << "Please enter a date after that: ";
        cin >> date;
        while (!validateDate(date))
        {
          cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
          cin >> date;
        }
      }
      if (account.accountType == FDA)
      {
        if (check_dates(date, last_date) < account.FDtime)
        {
          cout << "You cannot withdraw money from FD Account before Maturity!" << endl;
          cout << "Maturity Date: " << addMonthsToDate(last_date, account.FDtime) << endl;
        }
        else
        {
          double interest = getInterestRate(FDA);
          double amount = (account.balance * interest * account.FDtime) / 100 + account.balance;
          cout << "₹" << amount << " withdrawn successfully!" << endl;
          accounts.erase(accountNumber);
          cout << "Account deactivated!" << endl;
        }
        cout << "\nPress any key to return to the home page...";
        cin.ignore();
        cin.get();
        showHomePage();
        return;
      }
      // Calculate the total withdrawals on the given date
      double totalWithdrawalsOnDate = account.calculateTotalWithdrawalsOnDate(date);
      cout << "Total withdrawals made today: ₹" << totalWithdrawalsOnDate << endl;

      cout << "Enter the amount to withdraw: ₹";
      cin >> amount;

      while (account.accountType == 2 && amount > 10000)
      {
        cout << "Maximum withdrawal limit for savings account cannot exceed ₹10,000 per transaction!" << endl;
        cout << "Enter the amount to withdraw: ₹";
        cin >> amount;
      }

      if (amount > account.balance)
      {
        cout << "Error: Insufficient balance.\n";
      }
      else if (amount > account.dailyWithdrawalLimit)
      {
        cout << "Error: You have exceeded your daily withdrawal limit.\n";
      }
      else
      {
        account.calculateAndUpdateInterest(date);
        account.balance -= amount;
        account.addTransaction(-amount, "Withdraw", date);
        cout << "₹" << amount << " withdrawn successfully.\n";
      }
    }
    else
    {
      cout << "Incorrect PIN.\n";
    }
  }
  else
  {
    cout << "Account not found.\n";
  }

  cout << "\nPress any key to return to the home page...";
  cin.ignore();
  cin.get();
  showHomePage();
}

void Bank::checkTransactionHistory()
{
  if (askReturnHomePage("view Transaction History"))
    return;
  int accountNumber;
  string pin;
  clearScreen();
  cout << "\n\t\t\t\t\t\t\t***Transaction History***\n";
  cout << "\nEnter your account number: ";
  cin >> accountNumber;

  if (accounts.find(accountNumber) != accounts.end())
  {
    cout << "Enter PIN: ";
    cin >> pin;

    if (accounts[accountNumber].pin == pin)
    {
      Account &account = accounts[accountNumber];
      cout << "\nTransaction History for Account Number " << accountNumber << ":\n";
      if (account.transactions.empty())
      {
        cout << "No transactions found.\n";
      }
      else
      {
        for (auto &transaction : account.transactions)
        {
          cout << transaction.second[0] << " on " << transaction.second[1] << " - ₹" << transaction.first << endl;
        }
      }
    }
    else
    {
      cout << "Incorrect PIN.\n";
    }
  }
  else
  {
    cout << "Account not found.\n";
  }

  cout << "\nPress any key to return to the home page...";
  cin.ignore();
  cin.get();
  showHomePage();
}

void Bank::deleteAccount()
{
  if (askReturnHomePage("delete you account"))
    return;
  int accountNumber;
  string pin;
  string date;
  clearScreen();
  cout << "\n\t\t\t\t\t\t\t***Delete Account***\n";
  cout << "\nEnter date (YYYY-MM-DD): ";
  cout << "\nEnter the account number to delete: ";
  cin >> accountNumber;

  if (accounts.find(accountNumber) != accounts.end())
  {
    cout << "Enter PIN: ";
    cin >> pin;
    if (accounts[accountNumber].pin == pin)
    {
      Account &account = accounts[accountNumber];
      cout << "\nEnter date (YYYY-MM-DD): ";
      cin >> date;
      while (!validateDate(date))
      {
        cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
        cin >> date;
      }

      string last_date = account.transactions[account.transactions.size() - 1].second[1];
      while (check_dates(date, last_date) == -1)
      {
        cout << "Last transaction was made on " << last_date << endl;
        cout << "Please enter a date after that: ";
        cin >> date;
        while (!validateDate(date))
        {
          cout << "Invalid date format. Please enter the date in YYYY-MM-DD format: ";
          cin >> date;
        }
      }
      account.calculateAndUpdateInterest(date);
      cout << "Amount returned : " << account.balance << endl;
      string lower_name = account.name;
      for (int i = 0; i < lower_name.length(); i++)
      {
        if (lower_name[i] >= 65 && lower_name[i] <= 90)
        {
          lower_name[i] += 32;
        }
      }
      if (account.accountType == SAVINGS)
      {
        SavingsNameCount[lower_name]--;
      }
      if (account.accountType == SALARY)
      {
        SalaryNameCount[lower_name]--;
      }
      if (account.accountType == CURRENT)
      {
        CurrentNameCount[lower_name]--;
      }
      accounts.erase(accountNumber);
      cout << "Account deleted successfully.\n";
    }
    else
    {
      cout << "Incorrect PIN. \n";
    }
  }
  else
  {
    cout << "Account not found.\n";
  }

  cout << "\nPress any key to return to the home page...";
  cin.ignore();
  cin.get();
  showHomePage();
}