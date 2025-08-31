#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ------------------ Transaction ------------------
class Transaction
{
public:
    string type; // "Deposit", "Withdrawal", "Transfer"
    double amount;
    string note; // small description

    Transaction(const string &t, double a, const string &n)
        : type(t), amount(a), note(n) {}
};

// ------------------ Account ------------------
class Account
{
private:
    int accountNumber;
    double balance;
    vector<Transaction> history;

public:
    Account(int accNo) : accountNumber(accNo), balance(0.0) {}

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    void deposit(double amount)
    {
        if (amount <= 0)
        {
            cout << "Invalid deposit amount.\n";
            return;
        }
        balance += amount;
        history.emplace_back("Deposit", amount, "Amount deposited");
        cout << "Deposit successful. New balance: " << balance << "\n";
    }

    void withdraw(double amount)
    {
        if (amount <= 0)
        {
            cout << "Invalid withdrawal amount.\n";
            return;
        }
        if (amount > balance)
        {
            cout << "Insufficient balance.\n";
            return;
        }
        balance -= amount;
        history.emplace_back("Withdrawal", amount, "Amount withdrawn");
        cout << "Withdrawal successful. New balance: " << balance << "\n";
    }

    void transfer(Account &toAccount, double amount)
    {
        if (amount <= 0)
        {
            cout << "Invalid transfer amount.\n";
            return;
        }
        if (amount > balance)
        {
            cout << "Insufficient balance for transfer.\n";
            return;
        }
        balance -= amount;
        toAccount.balance += amount;

        history.emplace_back("Transfer", amount,
                             "Sent to Acc " + to_string(toAccount.getAccountNumber()));
        toAccount.history.emplace_back("Transfer", amount,
                                       "Received from Acc " + to_string(accountNumber));
        cout << "Transfer successful. Your new balance: " << balance << "\n";
    }

    void showTransactions(int lastN = 10) const
    {
        cout << "\n--- Transactions for Account " << accountNumber << " ---\n";
        if (history.empty())
        {
            cout << "No transactions yet.\n";
            return;
        }
        int start = 0;
        if ((int)history.size() > lastN)
        {
            start = (int)history.size() - lastN;
        }
        for (int i = start; i < (int)history.size(); i++)
        {
            cout << history[i].type << " | " << history[i].amount
                 << " | " << history[i].note << "\n";
        }
    }
};

// ------------------ Customer ------------------
class Customer
{
private:
    string name;
    int customerID;
    vector<Account> accounts;

public:
    Customer(const string &n, int id) : name(n), customerID(id) {}

    int getCustomerID() const { return customerID; }
    string getName() const { return name; }

    void createAccount(int accNo)
    {
        accounts.emplace_back(accNo);
        cout << "Account created for " << name
             << " with Account No: " << accNo << "\n";
    }

    void showAccounts() const
    {
        cout << "\n--- Accounts of " << name << " (ID " << customerID << ") ---\n";
        if (accounts.empty())
        {
            cout << "No accounts found.\n";
            return;
        }
        for (const auto &acc : accounts)
        {
            cout << "Account No: " << acc.getAccountNumber()
                 << " | Balance: " << acc.getBalance() << "\n";
        }
    }

    // Find account by number; returns pointer or nullptr
    Account *getAccount(int accNo)
    {
        for (auto &acc : accounts)
        {
            if (acc.getAccountNumber() == accNo)
                return &acc;
        }
        return nullptr;
    }
};

// ------------------ Helpers ------------------
int findCustomerIndexById(const vector<Customer> &customers, int id)
{
    for (int i = 0; i < (int)customers.size(); i++)
    {
        if (customers[i].getCustomerID() == id)
            return i;
    }
    return -1;
}

// ------------------ Main ------------------
int main()
{
    vector<Customer> customers;
    int nextCustomerId = 1;
    int nextAccountNo = 1001;

    while (true)
    {
        cout << "\n===== Banking System Menu =====\n";
        cout << "1. Create Customer\n";
        cout << "2. Create Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer\n";
        cout << "6. Show Accounts\n";
        cout << "7. Show Transactions\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";

        int choice;
        if (!(cin >> choice))
        { // basic input safety
            cout << "Invalid input. Exiting.\n";
            break;
        }

        if (choice == 1)
        {
            string name;
            cout << "Enter Customer Name (single word): ";
            cin >> name;
            customers.emplace_back(name, nextCustomerId);
            cout << "Customer created with ID: " << nextCustomerId << "\n";
            nextCustomerId++;
        }
        else if (choice == 2)
        {
            int id;
            cout << "Enter Customer ID: ";
            cin >> id;
            int idx = findCustomerIndexById(customers, id);
            if (idx == -1)
            {
                cout << "Customer not found.\n";
            }
            else
            {
                customers[idx].createAccount(nextAccountNo);
                nextAccountNo++;
            }
        }
        else if (choice == 3)
        { // Deposit
            int id, accNo;
            double amount;
            cout << "Customer ID: ";
            cin >> id;
            int idx = findCustomerIndexById(customers, id);
            if (idx == -1)
            {
                cout << "Customer not found.\n";
                continue;
            }
            cout << "Account No: ";
            cin >> accNo;
            Account *acc = customers[idx].getAccount(accNo);
            if (!acc)
            {
                cout << "Account not found.\n";
                continue;
            }
            cout << "Amount to deposit: ";
            cin >> amount;
            acc->deposit(amount);
        }
        else if (choice == 4)
        { // Withdraw
            int id, accNo;
            double amount;
            cout << "Customer ID: ";
            cin >> id;
            int idx = findCustomerIndexById(customers, id);
            if (idx == -1)
            {
                cout << "Customer not found.\n";
                continue;
            }
            cout << "Account No: ";
            cin >> accNo;
            Account *acc = customers[idx].getAccount(accNo);
            if (!acc)
            {
                cout << "Account not found.\n";
                continue;
            }
            cout << "Amount to withdraw: ";
            cin >> amount;
            acc->withdraw(amount);
        }
        else if (choice == 5)
        { // Transfer
            int fromId, fromAccNo, toId, toAccNo;
            double amount;
            cout << "From Customer ID: ";
            cin >> fromId;
            int fromIdx = findCustomerIndexById(customers, fromId);
            if (fromIdx == -1)
            {
                cout << "From-customer not found.\n";
                continue;
            }
            cout << "From Account No: ";
            cin >> fromAccNo;
            Account *fromAcc = customers[fromIdx].getAccount(fromAccNo);
            if (!fromAcc)
            {
                cout << "From-account not found.\n";
                continue;
            }

            cout << "To Customer ID: ";
            cin >> toId;
            int toIdx = findCustomerIndexById(customers, toId);
            if (toIdx == -1)
            {
                cout << "To-customer not found.\n";
                continue;
            }
            cout << "To Account No: ";
            cin >> toAccNo;
            Account *toAcc = customers[toIdx].getAccount(toAccNo);
            if (!toAcc)
            {
                cout << "To-account not found.\n";
                continue;
            }

            cout << "Amount to transfer: ";
            cin >> amount;
            fromAcc->transfer(*toAcc, amount);
        }
        else if (choice == 6)
        { // Show Accounts
            int id;
            cout << "Enter Customer ID: ";
            cin >> id;
            int idx = findCustomerIndexById(customers, id);
            if (idx == -1)
                cout << "Customer not found.\n";
            else
                customers[idx].showAccounts();
        }
        else if (choice == 7)
        { // Show Transactions
            int id, accNo, lastN;
            cout << "Customer ID: ";
            cin >> id;
            int idx = findCustomerIndexById(customers, id);
            if (idx == -1)
            {
                cout << "Customer not found.\n";
                continue;
            }
            cout << "Account No: ";
            cin >> accNo;
            Account *acc = customers[idx].getAccount(accNo);
            if (!acc)
            {
                cout << "Account not found.\n";
                continue;
            }
            cout << "Show last how many transactions? (e.g., 10): ";
            cin >> lastN;
            if (lastN <= 0)
                lastN = 10;
            acc->showTransactions(lastN);
        }
        else if (choice == 8)
        {
            cout << "Goodbye!\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
