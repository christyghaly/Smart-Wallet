#ifndef ACCOUNT_H
#define ACCOUNT_H
#include"person.h"
#include <iostream>
#include<string>

//#include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>
#include<QString>

using namespace std;

class account
{
public:
    account();
    void create_account(QString,QString,QString,int,int);	 // function to create the client and the account data get from the client side
    void dep(int);	//function to accept amount and add to balance amount
    void draw(int);	//function to accept amount and subtract from balance amount
    int retacno() const;	//function to return account number
    int retdeposit() const;	//function to return balance amount
    person client;

private:
        int acno;
        int deposit;


};

#endif // ACCOUNT_H
