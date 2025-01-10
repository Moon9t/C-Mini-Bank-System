#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stdexcept>

enum AccountType { SAVINGS, CHECKING };

class Transaction {
private:
    std::string timestamp;
    std::string type;
    double amount;
    double balanceAfter;

public:
    Transaction(std::string type, double amount, double balanceAfter) {
        time_t now = time(0);
        this->timestamp = ctime(&now);
        this->type = type;
        this->amount = amount;
        this->balanceAfter = balanceAfter;
    }

    std::string getDetails() const {
        std::stringstream ss;
        ss << timestamp.substr(0, timestamp.length()-1) << " | " 
           << std::setw(10) << type << " | $" 
           << std::fixed << std::setprecision(2) << amount << " | Balance: $" 
           << balanceAfter;
        return ss.str();
    }

    std::string serialize() const {
        return timestamp + "|" + type + "|" + std::to_string(amount) + "|" + std::to_string(balanceAfter);
    }

    static Transaction deserialize(const std::string& data) {
        std::stringstream ss(data);
        std::string timestamp, type, amountStr, balanceAfterStr;
        std::getline(ss, timestamp, '|');
        std::getline(ss, type, '|');
        std::getline(ss, amountStr, '|');
        std::getline(ss, balanceAfterStr, '|');
        return Transaction(type, std::stod(amountStr), std::stod(balanceAfterStr));
    }
};

class Account {
private:
    std::string accountNumber;
    std::string ownerName;
    double balance;
    std::string pin;
    AccountType type;
    std::vector<Transaction> transactionHistory;
    const double SAVINGS_INTEREST_RATE = 0.025;

public:
    Account(std::string accNum, std::string name, std::string pin, AccountType type, double initialBalance = 0.0) {
        this->accountNumber = accNum;
        this->ownerName = name;
        this->pin = pin;
        this->type = type;
        this->balance = initialBalance;
        if (initialBalance > 0) {
            addTransaction("INITIAL", initialBalance);
        }
    }

    void addTransaction(std::string type, double amount) {
        transactionHistory.push_back(Transaction(type, amount, balance));
        saveTransactions();
    }

    void applyInterest() {
        if (type == SAVINGS) {
            double interest = balance * (SAVINGS_INTEREST_RATE / 12);
            balance += interest;
            addTransaction("INTEREST", interest);
        }
    }

    void saveTransactions() {
        std::ofstream file("transactions_" + accountNumber + ".txt", std::ios::app);
        if (file.is_open()) {
            file << transactionHistory.back().serialize() << std::endl;
            file.close();
        }
    }

    void loadTransactions() {
        std::ifstream file("transactions_" + accountNumber + ".txt");
        std::string line;
        while (std::getline(file, line)) {
            transactionHistory.push_back(Transaction::deserialize(line));
        }
    }

    bool verifyPin(const std::string& inputPin) const { return pin == inputPin; }

    bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            addTransaction("DEPOSIT", amount);
            return true;
        }
        return false;
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            addTransaction("WITHDRAW", -amount);
            return true;
        }
        return false;
    }

    bool transfer(Account& recipient, double amount) {
        if (withdraw(amount)) {
            recipient.deposit(amount);
            return true;
        }
        return false;
    }

    void displayTransactionHistory() const {
        std::cout << "\nTransaction History for Account " << accountNumber << ":\n";
        std::cout << "------------------------------------------------\n";
        for (const auto& transaction : transactionHistory) {
            std::cout << transaction.getDetails() << std::endl;
        }
    }

    std::string getAccountNumber() const { return accountNumber; }
    std::string getOwnerName() const { return ownerName; }
    double getBalance() const { return balance; }
    AccountType getType() const { return type; }
};

class Admin {
private:
    std::string username;
    std::string password;

public:
    Admin(std::string username, std::string password) 
        : username(username), password(password) {}

    bool authenticate(const std::string& inputUsername, const std::string& inputPassword) const {
        return username == inputUsername && password == inputPassword;
    }
};

class Bank {
private:
    std::vector<Account> accounts;
    std::vector<Admin> admins;
    const std::string ACCOUNTS_FILE = "accounts.dat";

public:
    Bank() {
        loadAccounts();
        admins.push_back(Admin("admin", "admin123"));
    }

    void saveAccounts() {
        std::ofstream file(ACCOUNTS_FILE, std::ios::binary);
        if (file.is_open()) {
            for (const auto& account : accounts) {
                file.write(reinterpret_cast<const char*>(&account), sizeof(Account));
            }
            file.close();
        }
    }

    void loadAccounts() {
        std::ifstream file(ACCOUNTS_FILE, std::ios::binary);
        if (file.is_open()) {
            Account account("", "", "", SAVINGS);
            while (file.read(reinterpret_cast<char*>(&account), sizeof(Account))) {
                accounts.push_back(account);
            }
            file.close();
        }
    }

    void createAccount() {
        std::string name, pin, accNum;
        double initialDeposit;
        int typeChoice;

        std::cout << "\nEnter account number: ";
        std::cin >> accNum;
        
        if (findAccount(accNum) != nullptr) {
            std::cout << "Account number already exists!\n";
            return;
        }

        std::cout << "Enter name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "Create PIN: ";
        std::cin >> pin;
        std::cout << "Account type (0 for Savings, 1 for Checking): ";
        std::cin >> typeChoice;
        std::cout << "Enter initial deposit: $";
        std::cin >> initialDeposit;

        accounts.push_back(Account(accNum, name, pin, static_cast<AccountType>(typeChoice), initialDeposit));
        saveAccounts();
        std::cout << "\nAccount created successfully!\n";
    }

    void adminLogin() {
        std::string username, password;
        std::cout << "Enter admin username: ";
        std::cin >> username;
        std::cout << "Enter admin password: ";
        std::cin >> password;

        for (const auto& admin : admins) {
            if (admin.authenticate(username, password)) {
                adminMenu();
                return;
            }
        }
        std::cout << "Invalid admin credentials!\n";
    }

    void adminMenu() {
        int choice;
        do {
            std::cout << "\nAdmin Menu:\n";
            std::cout << "1. View All Accounts\n";
            std::cout << "2. Apply Monthly Interest\n";
            std::cout << "3. Exit Admin Mode\n";
            std::cout << "Choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    viewAllAccounts();
                    break;
                case 2:
                    applyMonthlyInterest();
                    break;
            }
        } while (choice != 3);
    }

    void viewAllAccounts() const {
        std::cout << "\nAll Accounts:\n";
        std::cout << "------------------------------------------------\n";
        for (const auto& acc : accounts) {
            std::cout << "Account: " << acc.getAccountNumber() 
                     << " | Owner: " << acc.getOwnerName()
                     << " | Type: " << (acc.getType() == SAVINGS ? "Savings" : "Checking")
                     << " | Balance: $" << std::fixed << std::setprecision(2) 
                     << acc.getBalance() << std::endl;
        }
    }

    void applyMonthlyInterest() {
        for (auto& acc : accounts) {
            acc.applyInterest();
        }
        saveAccounts();
        std::cout << "Monthly interest applied to all savings accounts.\n";
    }

    Account* findAccount(const std::string& accNum) {
        auto it = std::find_if(accounts.begin(), accounts.end(), [&accNum](const Account& acc) {
            return acc.getAccountNumber() == accNum;
        });
        return it != accounts.end() ? &(*it) : nullptr;
    }

    void performTransaction() {
        std::string accNum, pin;
        std::cout << "\nEnter account number: ";
        std::cin >> accNum;

        Account* acc = findAccount(accNum);
        if (!acc) {
            std::cout << "Account not found!\n";
            return;
        }

        std::cout << "Enter PIN: ";
        std::cin >> pin;

        if (!acc->verifyPin(pin)) {
            std::cout << "Invalid PIN!\n";
            return;
        }

        int choice;
        do {
            std::cout << "\n1. Check Balance\n2. Deposit\n3. Withdraw\n"
                     << "4. Transfer\n5. Transaction History\n6. Exit\nChoice: ";
            std::cin >> choice;

            switch (choice) {
                case 1: {
                    std::cout << "\nBalance: $" << std::fixed << std::setprecision(2) 
                             << acc->getBalance() << std::endl;
                    break;
                }
                case 2: {
                    double amount;
                    std::cout << "Enter deposit amount: $";
                    std::cin >> amount;
                    if (acc->deposit(amount)) {
                        std::cout << "Deposit successful!\n";
                        saveAccounts();
                    } else {
                        std::cout << "Invalid amount!\n";
                    }
                    break;
                }
                case 3: {
                    double amount;
                    std::cout << "Enter withdrawal amount: $";
                    std::cin >> amount;
                    if (acc->withdraw(amount)) {
                        std::cout << "Withdrawal successful!\n";
                        saveAccounts();
                    } else {
                        std::cout << "Insufficient funds or invalid amount!\n";
                    }
                    break;
                }
                case 4: {
                    std::string targetAccNum;
                    double amount;
                    std::cout << "Enter recipient account number: ";
                    std::cin >> targetAccNum;
                    Account* recipient = findAccount(targetAccNum);
                    if (!recipient) {
                        std::cout << "Recipient account not found!\n";
                        break;
                    }
                    std::cout << "Enter transfer amount: $";
                    std::cin >> amount;
                    if (acc->transfer(*recipient, amount)) {
                        std::cout << "Transfer successful!\n";
                        saveAccounts();
                    } else {
                        std::cout << "Transfer failed!\n";
                    }
                    break;
                }
                case 5: {
                    acc->displayTransactionHistory();
                    break;
                }
            }
        } while (choice != 6);
    }
};

void logAction(const std::string& action) {
    std::ofstream logFile("bank.log", std::ios_base::app);
    if (logFile.is_open()) {
        logFile << action << std::endl;
    }
}

int main() {
    Bank bank;
    int choice;

    std::cout << "Welcome to C++ Banking System\n";

    do {
        std::cout << "\n1. Create Account\n2. Banking Operations\n"
                 << "3. Admin Login\n4. Exit\nChoice: ";
        std::cin >> choice;

        try {
            switch (choice) {
                case 1:
                    bank.createAccount();
                    logAction("Created new account");
                    break;
                case 2:
                    bank.performTransaction();
                    logAction("Performed transaction");
                    break;
                case 3:
                    bank.adminLogin();
                    logAction("Admin logged in");
                    break;
                case 4:
                    std::cout << "Thank you for using our banking system!\n";
                    logAction("System exit");
                    break;
                default:
                    throw std::invalid_argument("Invalid choice");
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            logAction("Error: " + std::string(e.what()));
        }
    } while (choice != 4);

    return 0;
}
