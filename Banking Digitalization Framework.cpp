#include <iostream>
#include <fstream>
#include <map>
using namespace std;

#define MIN_BALANCE 500

long phone_no, aadharno;
string email, pan;
int loan;

class InsufficientFunds {};

class Account
{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;

public:
    Account() {}
    Account(string fname, string lname, float balance);

    long getAccNo() const { return accountNumber; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    float getBalance() const { return balance; }

    void Deposit(float amount);
    void Withdraw(float amount);

    static void setLastAccountNumber(long accNo);
    static long getLastAccountNumber();

    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, const Account &acc);
};

long Account::NextAccountNumber = 0;

class Bank
{
private:
    map<long, Account> accounts;

public:
    Bank();
    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accNo);
    Account Deposit(long accNo, float amount);
    Account Withdraw(long accNo, float amount);
    void CloseAccount(long accNo);
    void ShowAllAccounts();
    void showdetails();
    ~Bank();
};

int main()
{
    Bank bank;
    Account acc;
    string password, security;
    int choice;
    string fname, lname;
    long accNo;
    float balance, amount;

    cout << "UTTAR BIHAR GRAMIN BANK SYSTEM\n\n";
    cout << "Create admin password: ";
    cin >> password;

    cout << "Enter password to access system: ";
    cin >> security;

    if (password != security)
    {
        cout << "Access denied\n";
        return 0;
    }

    do
    {
        cout << "\n1 Open Account";
        cout << "\n2 Balance Enquiry";
        cout << "\n3 Deposit";
        cout << "\n4 Withdraw";
        cout << "\n5 Apply for Loan";
        cout << "\n6 Show All Accounts";
        cout << "\n7 Show Personal Details";
        cout << "\n8 Close Account";
        cout << "\n9 Quit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "First Name: ";
            cin >> fname;
            cout << "Last Name: ";
            cin >> lname;

            phone_no = 1111111111;
            email = "user@test.com";
            aadharno = 111122223333;
            pan = "ABCDE1234F";

            cout << "Initial Balance: ";
            cin >> balance;

            acc = bank.OpenAccount(fname, lname, balance);
            cout << "\nAccount Created Successfully\n";
            cout << acc;
            break;

        case 2:
            cout << "Account Number: ";
            cin >> accNo;
            cout << bank.BalanceEnquiry(accNo);
            break;

        case 3:
            cout << "Account Number: ";
            cin >> accNo;
            cout << "Amount: ";
            cin >> amount;
            cout << bank.Deposit(accNo, amount);
            break;

        case 4:
            cout << "Account Number: ";
            cin >> accNo;
            cout << "Amount: ";
            cin >> amount;
            try {
                cout << bank.Withdraw(accNo, amount);
            } catch (InsufficientFunds) {
                cout << "Insufficient Balance\n";
            }
            break;

        case 5:
            cout << "1 Student Loan\n2 Youth Loan\n3 Agriculture Loan\n";
            cin >> loan;
            if (loan == 1 || loan == 2 || loan == 3)
                cout << "Loan application submitted successfully\n";
            else
                cout << "Invalid option\n";
            break;

        case 6:
            bank.ShowAllAccounts();
            break;

        case 7:
            bank.showdetails();
            break;

        case 8:
            cout << "Account Number: ";
            cin >> accNo;
            bank.CloseAccount(accNo);
            break;
        }
    } while (choice != 9);

    return 0;
}

/* ================= ACCOUNT ================= */

Account::Account(string fname, string lname, float bal)
{
    accountNumber = ++NextAccountNumber;
    firstName = fname;
    lastName = lname;
    balance = bal;
}

void Account::Deposit(float amount)
{
    balance += amount;
}

void Account::Withdraw(float amount)
{
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
}

void Account::setLastAccountNumber(long accNo)
{
    NextAccountNumber = accNo;
}

long Account::getLastAccountNumber()
{
    return NextAccountNumber;
}

ofstream &operator<<(ofstream &ofs, Account &acc)
{
    ofs << acc.accountNumber << endl
        << acc.firstName << endl
        << acc.lastName << endl
        << acc.balance << endl;
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Account &acc)
{
    ifs >> acc.accountNumber >> acc.firstName >> acc.lastName >> acc.balance;
    return ifs;
}

ostream &operator<<(ostream &os, const Account &acc)
{
    os << "First Name: " << acc.getFirstName() << endl;
    os << "Last Name: " << acc.getLastName() << endl;
    os << "Account Number: " << acc.getAccNo() << endl;
    os << "Balance: " << acc.getBalance() << endl;
    return os;
}

/* ================= BANK ================= */

Bank::Bank()
{
    Account acc;
    ifstream infile("Bank.data");
    while (infile >> acc)
        accounts[acc.getAccNo()] = acc;
    Account::setLastAccountNumber(acc.getAccNo());
}

Account Bank::OpenAccount(string fname, string lname, float balance)
{
    Account acc(fname, lname, balance);
    accounts[acc.getAccNo()] = acc;

    ofstream outfile("Bank.data", ios::trunc);
    for (auto &p : accounts)
        outfile << p.second;

    return acc;
}

Account Bank::BalanceEnquiry(long accNo)
{
    return accounts[accNo];
}

Account Bank::Deposit(long accNo, float amount)
{
    accounts[accNo].Deposit(amount);
    return accounts[accNo];
}

Account Bank::Withdraw(long accNo, float amount)
{
    accounts[accNo].Withdraw(amount);
    return accounts[accNo];
}

void Bank::CloseAccount(long accNo)
{
    cout << "Account Closed:\n" << accounts[accNo];
    accounts.erase(accNo);
}

void Bank::ShowAllAccounts()
{
    for (auto &p : accounts)
        cout << p.second << endl;
}

void Bank::showdetails()
{
    cout << "Phone: " << phone_no << endl;
    cout << "Email: " << email << endl;
    cout << "Aadhaar: " << aadharno << endl;
    cout << "PAN: " << pan << endl;
}

Bank::~Bank()
{
    ofstream outfile("Bank.data", ios::trunc);
    for (auto &p : accounts)
        outfile << p.second;
}
