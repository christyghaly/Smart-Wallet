#include "account.h"
#include<QObject>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include<arpa/inet.h>
#include<QDebug>

using namespace std;

account::account()
{

}

void account::create_account(bool flag,bool Withdrawing)  // The user enter the data to be saved and used all over the session
{///// flag=true,Withdrawing=false ->newaccount created so the message shown is entered your initial balance
    //// flag=false,Withdrawing=false ->Deposit money to the account so the message shown is enter the money to deposit
    /// flag=false,Withdrawing=true ->withdraw/donation to the account so the message shown is enter the money to withdraw/donate
    /// flag=true,WIthdrawing=true -> get THe information about the account at the beginning of the session



    int counter = 0;
        cout << "\n\nEnter The Name of The account Holder : ";
        cin.ignore();
        cin.getline(client.name, 50);
        while (client.age <= 0) { // to check if the age entered is correct
            if (counter != 0)
                cout << "Sorry this age is not allowed to be entered";
            counter++;
            cout << "\n\nEnter The Age of The account Holder:";
            cin >> client.age;
        }
        counter = 1;

        while (!(counter == 0)) {      ///while loop to prevent entring digits bigger or smaller than 14
            cout << "\n\nEnter NationalID of The account Holder:";
            cin.getline(client.Id, 20);
            counter++;
            int i = 0;
            for ( i = 0; i <=13; i++) {
                int x =( (int)(client.Id[i]))-48;
                if ((x > 9 ||x<0||cin.gcount()!=15) ) {
                    break;
                }
            }
            if (i == 14) {
                counter = 0;
                break;
            }
        }

        if(Withdrawing==false){// New account OR Deposit
        if(flag==true)
            cout << "\nEnter The Initial Balance:";
        else
            cout << "\nEnter The Cash Money you want to deposit:";


        cin >> deposit;
        }
        else{//Withdraw/Donate OR knowing the Balance of the account
            if(flag==false){ //withdraw/Donate
            cout<<"\nEnter The Cash Money you want to Withdraw/Donate:";
            cin>>withdraw;
            }
            else{ // need to know the balance of the account from the beginning of the session
                withdraw=0;
                deposit=0;
                cout<<"\n Enter your account number:";
                cin>>acno;
            }
        }


}

void account::AnotherAccount() // function to delete the already saved data
{
        acno=-1;
        deposit=0;
        withdraw=0;
        memset(client.name,0,50);
        memset(client.Id,'A',20);
        client.age=-1;


}

int account::retacno() const
{
        return acno;
};
int account::retdeposit() const
{
    return deposit;
}

void account::setaccount(int i)
{
    acno=i;
}

void account::setdeposit(int i)
{
    deposit=i;
}

int account::retwithdrawn()
{
    return withdraw;
}

char* write_account(account* newaccount,char* resultstring,char type)	////function to prepare the string that will be sent to the server in case of create new account/ get balance
{
    char res[4096];
    char NID[14];
    string result;
    memset(res,0,4096);


    if(type=='1') //create new account
        newaccount->create_account(true,false);
    else if(type=='S')
        newaccount->create_account(true,true);
    result.append(newaccount->client.name);
    result.append(",");
    result.append(to_string(newaccount->client.age));
    result.append(",");
    for(int i=0;i<14;i++)
        NID[i]=newaccount->client.Id[i];
    result.append(NID);
    if(type=='1'){// create new account
        result.append(",new,0,");
        result.append(to_string(newaccount->retdeposit()));
        result.append(",createaccount");
    }
    else{// get the information about the account
        result.append(",old,");
        result.append(to_string(newaccount->retacno()));
        result.append(",getamount");

    }

strcpy(res,result.c_str());
resultstring=res;
return resultstring;

}


//***************************************************************
//    	function to deposit and withdraw/donate amounts and prepare their string that will be sent to the server
//****************************************************************

char* deposit_withdraw(int n, int option,account* newaccount,char *resultstring)
{

    char res[4096];
    char NID[14];
    string result;
    memset(res,0,4096);

    result.append(newaccount->client.name);
    result.append(",");
    result.append(to_string(newaccount->client.age));
    result.append(",");
    for(int i=0;i<14;i++)
        NID[i]=newaccount->client.Id[i];
    result.append(NID);
    result.append(",");
    result.append(to_string(newaccount->retacno()));
    result.append(",");
    result.append(to_string(n));
    result.append(",");


            if (option == 1)
            {
                result.append("deposit");


            }
            else if (option == 2)
            {
                result.append("withdraw");

            }
            else if(option==4)
                result.append("donate");


            cout<<result;

            strcpy(res,result.c_str());
            resultstring=res;
            return resultstring;

}


//***************************************************************
//    overloaded function to Pay telephone Bill nd prepare their string that will be sent to the server
//****************************************************************

char* deposit_withdraw(account *newaccount,char* resultstring,char* telephoneNum){
    char res[4096];
    char NID[14];
    char Tele[11];
    string result;
    memset(res,0,4096);

    result.append(newaccount->client.name);
    result.append(",");
    result.append(to_string(newaccount->client.age));
    result.append(",");
    for(int i=0;i<14;i++)
        NID[i]=newaccount->client.Id[i];
    for(int i=0;i<11;i++)
        Tele[i]=*(telephoneNum+i);
    result.append(NID);
    result.append(",");
    result.append(to_string(newaccount->retacno()));
    result.append(",telephoneBill,");
    result.append(Tele);

    cout<<"\n";
    cout<<result<<endl;
    strcpy(res,result.c_str());
    resultstring=res;
    return resultstring;

}

//***************************************************************
//    	INTRODUCTION FUNCTION
//****************************************************************


void intro()
{
    cout << "\n\n\n\t  SMART";
    cout<<  "\n\n\t WALLET";
    cout << "\n\n\tMANAGEMENT";
    cout << "\n\n\t  SYSTEM";
    cin.get();
}

void connect_toserver(){
       int listenFd, portNo;
       struct sockaddr_in svrAdd;
      // struct hostent *server;
       portNo=atoi("54000");
       if((portNo > 65535) || (portNo < 2000))
           {
               cerr<<"Please enter port number between 2000 - 65535"<<endl;
               return;
           }
       listenFd = socket(AF_INET, SOCK_STREAM, 0);

          if(listenFd < 0)
          {
              cerr << "Cannot open socket" << endl;
              return;
          }
       //server = gethostbyname("christeen-VirtualBox"); // host was get by hostname in case of known host name
       string ipAddress="127.0.0.1";
//       if(server == NULL)
//           {
//               cerr << "Host does not exist" << endl;
//               return;
//           }
       bzero((char *) &svrAdd, sizeof(svrAdd));
           svrAdd.sin_family = AF_INET;

          // bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);

           svrAdd.sin_port = htons(portNo);
           inet_pton(AF_INET,ipAddress.c_str(),&svrAdd.sin_addr);

           int checker = connect(listenFd,(struct sockaddr *) &svrAdd, sizeof(svrAdd));

           if (checker < 0)
           {
               cerr << "Cannot connect!" << endl;
               return;
           }
char test[4096];
bzero(test,4097);
bool connec=true;
account* ac=new account();

while(connec==true)
           {

               char choosen;
               int num;
               QString Recvstring;
               QStringList RecvList;
               cout << "\n\n\n\tMAIN MENU";
               cout << "\n\n\t01. NEW ACCOUNT";
               cout << "\n\n\t02. DEPOSIT AMOUNT";
               cout << "\n\n\t03. WITHDRAW AMOUNT";
               cout << "\n\n\t04. BALANCE ENQUIRY";
               cout << "\n\n\t05. Donation";
               cout << "\n\n\t06. Pay Telephone Bill";
               cout << "\n\n\t07. Use Another Account";
               cout << "\n\n\t08. Disconnect";
               cout << "\n\n\tSelect Your Option (1-8) ";
               cin>>choosen;
               char*out;
               char*resptr;
               resptr=(char*)malloc(4096);
               cout<<"CLient: ";
               cout<<ac->client.name;


               switch (choosen)
               {

               case '1':
                   out=write_account(ac,resptr,'1');
                   break;
               case '2':
                   if(ac->client.age>0){  //this session started with creating account
                       //and then you need to deposit some money to your account
                       cout << "\n\n\tEnter the Cashmoney that you would deposit:";
                       cin >> num;

                       out=deposit_withdraw(num, 1,ac,resptr);

                   }
                   // if the account was created before and its data are on the server side
                   //And in this session your first choose is deposite money
                   else{
                       int accnum;
                       cout << "\nEnter The account No. :";
                       cin >>accnum;
                       ac->setaccount(accnum);
                       ac->create_account(false,false);
                       out=deposit_withdraw(ac->retdeposit(), 1,ac,resptr);

                   }
                   break;
               case '3':
                   if(ac->client.age>0){  //the data of the client is entered before
                       //and then you need to withdraw some money to your account
                       cout << "\n\n\tEnter the Cashmoney that you would withdraw:";
                       cin >> num;

                       out=deposit_withdraw(num, 2,ac,resptr);

                   }
                   // if the account was created before and its data are on the server side
                   //And in this session your first choose is withdraw money
                   else{
                       int accnum;
                       cout << "\nEnter The account No. :";
                       cin >>accnum;
                       ac->setaccount(accnum);
                       ac->create_account(false,true);
                       out=deposit_withdraw(ac->retwithdrawn(), 2,ac,resptr);
                   }
                   break;
               case '4':///Get Info about the account Case
                   if(ac->client.age>0){  // this session is Not started with getting the information
                       out=write_account(ac,resptr,'N');
                   }
                   else{ // this session starts with asking abouT the account 

                       out=write_account(ac,resptr,'S');
                   }
                   break;
               case'5'://Donation Case
                   if(ac->client.age>0){  //the data of the client is entered before
                       //and then you need to donate some money to your account
                       cout << "\n\n\tEnter the Cashmoney that you would Donate:";
                       cin >> num;

                       out=deposit_withdraw(num, 4,ac,resptr);

                   }
                   // if the account was created before and its data are on the server side
                   //And in this session your first choice is to donate some money
                   else{
                       int accnum;
                       cout << "\nEnter The account No. :";
                       cin >>accnum;
                       ac->setaccount(accnum);
                       ac->create_account(false,true);
                       out=deposit_withdraw(ac->retwithdrawn(),4,ac,resptr);

                   }
                   break;
               case'6'://telephoneBill
                   char tele[20];
                   if(ac->client.age>0){

                       int counter=1;
                       while (!(counter == 0)) { ///to prevent entering more than 11 numbers
                           cout << "\n\n\tEnter the Telephone Number:";
                           cin.getline(tele,20);
                           counter++;
                           int i = 0;
                           for ( i = 0; i <=10; i++) {
                               int x =( (int)(tele[i]))-48;
                               if ((x > 9 ||x<0||cin.gcount()!=12) ) {
                                   break;
                               }
                           }
                           if (i == 11) {
                               counter = 0;
                               break;
                           }
                       }

                       out=deposit_withdraw(ac,resptr,tele);

                   }
                   else{
                       int counter=1;
                       ac->create_account(true,true);
                       /// the while loop may be written outside the
                       /// the if-else to prevent duplication but it is not logical to
                       /// ask the user about the telephone number before knowing the account Number

                       while (!(counter == 0)) {
                           cout << "\n\n\tEnter the Telephone Number:";
                           cin.getline(tele,20);
                           counter++;
                           int i = 0;
                           for ( i = 0; i <=10; i++) {
                               int x =( (int)(tele[i]))-48;
                               if ((x > 9 ||x<0||cin.gcount()!=12) ) {
                                   break;
                               }
                           }
                           if (i == 11) {
                               counter = 0;
                               break;
                           }
                       }
                       out=deposit_withdraw(ac,resptr,tele);

                   }
                   break;


               case '7':
                   ac->AnotherAccount();
                   cout<<"\n\n Done";
                   break;

               case '8':
                   cout << "\n\n\tThanks for using bank managemnt system\n";
                   connec=false;
                   break;
               default:cout << "\a";
               }
               if(choosen!='7'){


               cout << "\n\n\nConnecting to the server...PLease wait";
               bzero(test,4097);
               char out2[4096];
               bzero(out2,4097);
               for (int i=0;i<4096;i++)
                   out2[i]=*(out+i);
               string userINput (out2);

               if(connec==false)
                   userINput="exit";
              int sendres=send(listenFd, userINput.c_str(), userINput.size()+1,0);
               if (sendres==-1){
                   cout<<"\n\nCouldn't send to server";
                   continue;
               }
               if(connec==true){
                   recv(listenFd,test,4096,0);
                   Recvstring=test;
                   if(Recvstring.contains("_"))
                       RecvList=Recvstring.split("_");
                   cout<<"\n\nServer Respond: "<<test;
               }
               if(choosen=='1'){ // create new account so the account number generated by the server should be returned

                   ac->setaccount(RecvList[1].right((RecvList[1].size()-(RecvList[1].indexOf("=")+1))).toInt());
                   ac->setdeposit(RecvList[2].right((RecvList[2].size()-(RecvList[2].indexOf("=")+1))).toInt());

               }
               else if(choosen=='2'||choosen=='3'||choosen=='5'){ // Deposit and get the account balance from the server
                   if(RecvList[2].contains("now")) // this case happens when the client is found in the server
                       ac->setdeposit(RecvList[3].right((RecvList[3].size()-(RecvList[3].indexOf("=")+1))).toInt());

               }
               else if(choosen=='4'){
                   if(RecvList[0].contains("New",Qt::CaseSensitive)){
                       //in case the user require to know information about the account
                       //but the account is not saved at the server
                       ac->setaccount(RecvList[1].right((RecvList[1].size()-(RecvList[1].indexOf("=")+1))).toInt());
                       ac->setdeposit(RecvList[2].right((RecvList[2].size()-(RecvList[2].indexOf("=")+1))).toInt());
                   }
                   else{
                       ac->setaccount(RecvList[0].right((RecvList[0].size()-(RecvList[0].indexOf("=")+1))).toInt());
                       ac->setdeposit(RecvList[1].right((RecvList[1].size()-(RecvList[1].indexOf("=")+1))).toInt());



                   }

               }
            }




free(resptr);

           }


close(listenFd);
}

//***************************************************************
//    			END OF  Client PROJECT
//***************************************************************
