# C++ Banking System

This is a simple banking system implemented in C++. The program allows users to create accounts, perform transactions, and provides an admin interface for managing accounts and applying interest.

## Features

### User Features:
- **Account Creation:**
  - Users can create savings or checking accounts with an initial deposit.
- **Banking Operations:**
  - Deposit funds.
  - Withdraw funds.
  - Transfer funds between accounts.
  - View transaction history.
  - Check account balance.

### Admin Features:
- **View All Accounts:**
  - Admins can view details of all accounts, including balance and account type.
- **Apply Monthly Interest:**
  - Automatically applies interest to all savings accounts.

### Additional Features:
- Transaction logs saved locally for each account.
- User PIN verification for account security.
- Admin authentication with username and password.

## Files
- `bank.cpp`: Contains the complete implementation of the banking system.
- `transactions_<account_number>.txt`: Stores transaction history for individual accounts.
- `accounts.dat`: Serialized storage of account information.
- `bank.log`: Logs all actions performed in the system.

## Installation
1. Clone the repository:
   ```bash
   https://github.com/Moon9t/C-Mini-Bank-System.git
   ```
2. Navigate to the project directory:
   ```bash
   cd C-Mini-Bank-System
   ```
3. Compile the program:
   ```bash
   g++ bank.cpp -o banking-system
   ```
4. Run the program:
   ```bash
   ./banking-system
   ```

## Usage
### Main Menu:
1. **Create Account:** Create a new savings or checking account.
2. **Banking Operations:** Access existing accounts for transactions.
3. **Admin Login:** Login as an admin for advanced operations.
4. **Exit:** Exit the program.

### Admin Menu:
1. **View All Accounts:** Display details of all accounts.
2. **Apply Monthly Interest:** Apply interest to all savings accounts.

## Example Workflow
1. Start the program and create an account.
2. Use the account to deposit, withdraw, or transfer funds.
3. Admins can log in to view all accounts and apply interest.

## Dependencies
- Standard C++ libraries:
  - `<iostream>`
  - `<string>`
  - `<vector>`
  - `<fstream>`
  - `<ctime>`
  - `<iomanip>`
  - `<sstream>`
  - `<algorithm>`
  - `<stdexcept>`

## Future Enhancements
- Implement database integration for account storage.
- Add encryption for PIN and sensitive data.
- Create a graphical user interface (GUI).

## License
This project is licensed under the MIT License. See the `LICENSE` file for more details.

---

Feel free to contribute to this project by submitting issues or pull requests!
