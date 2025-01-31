#ifndef PERSON_H
#define PERSON_H
#include <iostream>
#include<cctype>
#include<QString>
using namespace std;


class person
{
public:
    person();
    char name[50];
    int age=-1;
    char Id[14] = { 'A','A','A','A','A','A','A','A','A','A','A','A','A','A'};
    int TeleBill=rand()%1000+1; // a random number is created that represents the telephone bill with the creation of new client, so the user can only pay the bill once  


};

#endif // PERSON_H
