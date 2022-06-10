// Banking System Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

#define MB 500
class InsufficientFunds {};

class Account
{
	long accountnumber;
	string fname;
	string lname;
	float balance;
	static long nextaccountnumber;
public:
	//basic constructor
	Account() {}
	Account(string fn, string ln, float bal);
	string getfirstname() { return fname; }
	string getlastname() { return lname; }
	long getAccNo() { return accountnumber; }
	float getbalance() { return balance; }
	//basic functions
	void Depoist(float bal);
	void Withdraw(float bal);
	//account number allocation
	static void setaccountnumber(long accounm);
	static long getaccountnumber();
	//display functions
	friend ofstream& operator<<(ofstream& ofs, Account& acc);
	friend ifstream& operator>>(ifstream& ifs, Account& acc);
	friend ostream& operator<<(ostream& os, Account& acc);
};

long Account::nextaccountnumber = 0;

class Bank //the same functions are called in bank cause though the transactions are made in accunt the banks keeps a record
{
	map<long, Account> accounts;
public:
	Bank();
	Account OpenAccount(string fn, string ln, float bal);
	Account Depoist(long accnum, float bal);
	Account Withdraw(long accnum, float bal);
	Account BalanceEnquiry(long accnum);
	void CloseAccount(long accnum);
	void ShowAllAccounts();
	~Bank();
};

int main()
{
	Bank b;
	Account acc;
	int choice;
	long accountnumber;
	string fname;
	string lname;
	float balance,added;
	do
	{
		cout << "**Banking System Project**" << endl;
		cout << "\n\t1)Opening an account";
		cout << "\n\t2)Balance Enquiry";
		cout << "\n\t3)Depoist";
		cout << "\n\t4)Withdrawal";
		cout << "\n\t5)Closing account";
		cout << "\n\t6)Show All Accounts in database";
		cout << "\n\t7)Exit from interface";
		cout << "\n\tEnter your choice:";
		cin >> choice;
		switch (choice)
		{
		case 1://creating an account
			cout << "Enter your first name:";
			cin >> fname;
			cout << "Enter your last name:";
			cin >> lname;
			cout << "Enter Balance to be added:";
			cin >> balance;
			if (balance < MB)
			{
				cout << "Insufficient Funds to start an account\n";
				break;
			}
			else
				acc = b.OpenAccount(fname, lname, balance);
			cout << "Account successfully Created\n";
			cout << acc;
			break;
		case 2://balance enquiry
			cout << "Enter your Account Number:";
			cin >> accountnumber;
			acc = b.BalanceEnquiry(accountnumber);
			cout << endl << "Your Account Details:" << endl;
			cout << acc;
			break;
		case 3://Depoisting
			cout << "Enter your Account Number:";
			cin >> accountnumber;
			cout << "Enter your Depoisting balance:";
			cin >> added;
			acc = b.Depoist(accountnumber, added);
			cout << endl << "Amount is Deposited\n" << endl;
			cout << acc;
			break;
		case 4://Withdrawing
			cout << "Enter your Account Number:";
			cin >> accountnumber;
			cout << "Enter your Withdrawing balance:";
			cin >> added;
			acc = b.Withdraw(accountnumber, added);
			cout << endl << "Amount Withdrawn\n" << endl;
			cout << acc;
			break;
		case 5://closing account
			cout << "Enter your Account Number:";
			cin >> accountnumber;
			b.CloseAccount(accountnumber);
			cout << endl << "Account is Closed\n" << endl;
		case 6://showing all accout
			b.ShowAllAccounts();
		case 7:
		default:
			cout << "Enter a valid option\n";
			break;
 		}
	} while (choice != 7);

	return 0;
}

Account::Account(string fn, string ln, float bal)
{
	nextaccountnumber++;
	accountnumber = nextaccountnumber;
	fname = fn;
	lname = ln;
	balance = bal;
}

void Account::Depoist(float bal)
{
	balance += bal;
}

void Account::Withdraw(float bal)
{
	if (balance - bal < MB)
		throw InsufficientFunds();
	balance -= bal;
}

void Account::setaccountnumber(long accountnumber)
{
	nextaccountnumber = accountnumber;
}

long Account::getaccountnumber()
{
	return nextaccountnumber;
}

ofstream& operator<<(ofstream& ofs, Account& acc)
{
	ofs << acc.accountnumber << endl;
	ofs << acc.fname << endl;
	ofs << acc.lname << endl;
	ofs << acc.balance << endl;
	return ofs;
}

ifstream& operator>>(ifstream& ifs, Account& acc)
{
	ifs >> acc.accountnumber;
	ifs >> acc.fname;
	ifs >> acc.lname;
	ifs >> acc.balance;
	return ifs;
}

ostream& operator<<(ostream& os, Account& acc)
{
	os << "First Name:" << acc.getfirstname() << endl;
	os << "Last Name:" << acc.getlastname() << endl;
	os << "Account Number:" << acc.getAccNo() << endl;
	os << "Balance:" << acc.getbalance() << endl;
	return os;
}

Bank::Bank()
{
	Account account;
	ifstream infile;
	infile.open("Bank.txt");
	if (!infile)
	{
		cout<<"Error in Opening! File Not Found!!"<<endl;
		return;
	}
	while (!infile.eof())
	{
		infile >> account;
		accounts.insert(pair<long, Account>(account.getAccNo(), account));
	}
	Account::setaccountnumber(account.getAccNo());
	infile.close();
}

Account Bank::OpenAccount(string fname, string lname, float balance)
{
	ofstream outfile;
	Account account(fname, lname, balance); 
	accounts.insert(pair<long, Account>(account.getAccNo(), account));

	outfile.open("Bank.txt", ios::trunc);
	map<long, Account>::iterator itr;
	for (itr = accounts.begin(); itr != accounts.end(); itr++)
	{
		outfile << itr->second;
	}
	outfile.close();
	return account;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	return itr->second;
}

Account Bank::Depoist(long accountNumber, float amount)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	itr->second.Depoist(amount);
	return itr->second;
}

Account Bank::Withdraw(long accountNumber, float amount)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	itr->second.Withdraw(amount);
	return itr->second;
}

void Bank::CloseAccount(long accountNumber)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	cout << "Account Deleted" << itr->second;
	accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts()
{
	map<long, Account>::iterator itr;
	for (itr = accounts.begin(); itr != accounts.end(); itr++)
	{
		cout << "Account " << itr->first << endl << itr->second << endl;
	}
}

Bank::~Bank()
{
	ofstream outfile;
	outfile.open("Bank.txt", ios::trunc);
	map<long, Account>::iterator itr;
	for (itr = accounts.begin(); itr != accounts.end(); itr++)
	{
		outfile << itr->second;
	}
	outfile.close();
}