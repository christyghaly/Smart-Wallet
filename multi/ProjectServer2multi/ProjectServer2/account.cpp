#include "account.h"
#include<QDebug>
#include<QString>
account::account()
{

}

int account::retacno() const
{
        return acno;
};
int account::retdeposit() const
{
    return deposit;
}
void account::create_account(QString nme,QString ag,QString nid,int accno,int money) // function to create the client and the account data get from the client side
{
        acno=accno;
        qDebug()<<"NID FROM CREATE /////";
        qDebug()<<nid;
		
        string clientname=nme.toUtf8().constData();
        strcpy(client.name,clientname.c_str());
		
        deposit=money;
		
        string clientnid="";
        clientnid=nid.toUtf8().constData();
		strcpy(client.Id,clientnid.c_str());  
		
		client.age=ag.toInt();
		


}
void account::dep(int x)
{
    deposit+=x;
}
void account::draw(int x)
{
    deposit-=x;
}


