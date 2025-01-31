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
    int TeleBill=rand()%1000+1;


};

#endif // PERSON_H
