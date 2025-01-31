#ifndef ACCOUNT_H
#define ACCOUNT_H
#include"person.h"
#include <iostream>
#include<string>

//#include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>

using namespace std;

class account
{
private:
        int acno=-1;
        int deposit=0;
        int withdraw=0;

public:
    account();
    person client;
    void create_account(bool newaccount,bool withdraw);	// The user enter the data to be saved and used all over the session
    void AnotherAccount();	//function to ask about another account and delete the already name , age, account number,Id,.. and login to another account
    int retacno() const;	//function to return account number
    int retdeposit() const;	//function to return balance amount
    void setaccount(int i);// function to set account number
    void setdeposit(int i); // function to set the deposit
    int retwithdrawn(); // function to reurn the amount to withdarw or to donate


};

#endif // ACCOUNT_H

char* write_account(account* newaccount,char* resultstring);	//function to prepare the string that will be sent to the server
char* deposit_withdraw(int, int,account *newaccount,char* resultstring); // function to desposit/withdraw/donate for/from a given account
char* deposit_withdraw(account *newaccount,char* resultstring,char* telephoneNum); // overloaded function to pay telephone bill
void intro();	//introductory screen function
void connect_toserver(); // Main function to connect to the server
