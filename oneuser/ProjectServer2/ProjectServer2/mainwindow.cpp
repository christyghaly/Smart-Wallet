//*****************************************************************
//this program respomds to only one client and write its name, Id and age in the GUI
//**********************************************************************
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"account.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include<QDebug>
#include<QString>
#include<QMessageBox>
#include<QPushButton>
#include<QHBoxLayout>
#include<QThread>
#include<QObject>
#include <iterator>
#include <map>
#include<QDateTime>
#include<QDate>

#include<QMutex>
using namespace std;

map<int, int> all_acc_no;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

} 
// the function that binds between the main thread and the thread opened  
void MainWindow::ThreadCaller(int x,int socketfd)
{
    QThread*thread=new QThread;
    Worker *worker=new Worker(x);
    worker->fdescriptor=socketfd;
    worker->moveToThread(thread);
    QObject::connect(thread,SIGNAL(started()),worker,SLOT(readFromClient()));
    QObject::connect(worker,SIGNAL(update(QString,int)),this,
                     SLOT(update_ui(QString,int)));
    QObject::connect(worker,SIGNAL(terminate()),thread,SLOT(quit()));
    QObject::connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));


    thread->start();

}

// Start button which makes the server ready to listen to the clients


void MainWindow::on_pushButton_clicked()
{
        for(int i=0;i<1000;i++){ // initializing the map that have the account numbers that is already assigned to accounts
           all_acc_no.insert(pair<int,int>(i,0));
        }
       int noThread = 0;
       int connFd1;
    int pId, portNo, listenFd;
        bool loop = false;
        struct sockaddr_in svrAdd, clntAdd;

        portNo = atoi("54000");

           if((portNo > 65535) || (portNo < 2000))
           {
               qDebug() << "Please enter a port number between 2000 - 65535";
               return;
           }
           //create socket
              listenFd = socket(AF_INET, SOCK_STREAM, 0);

              if(listenFd < 0)
              {
                  qDebug()<< "Cannot open socket";
                  return;
              }
           bzero((char*) &svrAdd, sizeof(svrAdd));

               svrAdd.sin_family = AF_INET;
               svrAdd.sin_addr.s_addr = INADDR_ANY;
               svrAdd.sin_port = htons(portNo);

               //bind socket
               if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
               {
                   qDebug() << "Cannot bind";
                   return;
               }

               listen(listenFd, 3); // listen to 3 clients only


               if(noThread<3) // In multi threaded it while be while (noThread<3)
               {

                 socklen_t  len = sizeof(clntAdd);

                   qDebug() << "Listening" << endl;

                   //this is where client connects. svr will hang in this mode until client conn
                       connFd1 = accept(listenFd, (struct sockaddr *)&clntAdd, &len);
                       if (connFd1 < 0)
                       {
                           qDebug() << "Cannot accept connection";
                           return;
                       }
                       else
                       {
                           qDebug() << "Connection successful";
                       }


                      ThreadCaller(noThread,connFd1);

                   noThread++;
               }

}


// Update ui upon receiving the name and Id of the client
void MainWindow::update_ui(QString str, int no_of_thread)
{

    if(str.contains(",")){
    QStringList list=str.split(",");
    if (no_of_thread==0){
    ui->lineEdit_1->setText(list[0]);
    ui->lineEdit_2->setText(list[1]);
    ui->lineEdit_3->setText(list[2]);
    }
    else if (no_of_thread==1){
        ui->lineEdit_4->setText(list[0]);
        ui->lineEdit_5->setText(list[1]);
        ui->lineEdit_6->setText(list[2]);
    }
    else {
        ui->lineEdit_7->setText(list[0]);
        ui->lineEdit_8->setText(list[1]);
        ui->lineEdit_9->setText(list[2]);
    }

    ui->statusbar->showMessage(list[0]);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

//****************************************************************
// Worker is the wrapper of the thread
//******************************************************************
Worker::Worker(int x,QObject *parent) : QObject(parent)
{
    this->threadNo=x;

}


void Worker::readFromClient()
{

    char test[300];
    bzero(test, 301);
    bool loop = false;
	int counter=0;
    QString l;
    while(!loop)
    {
        bzero(test, 301);
        char t[300]="di";
        string s="DONE";
        bzero(t, 301);
        strcpy(t,s.c_str());

        recv(fdescriptor, test, 300,0);
        string tester (test);
        l=test;

        if(l.contains(",")){
            emit update(l,threadNo);
			counter ++;
            QStringList list1=l.split(",");
            if(list1.contains("deposit")){

                char*out;
                char*resptr;
                resptr=(char*)malloc(4096);
                out=deposit_withdraw(resptr,list1,1);
                char out2[4096];
                bzero(out2,4097);
                for(int i=0;i<4096;i++)
                    out2[i]=*(out+i);
                string output (out2);
                free(resptr);

                send(fdescriptor,output.c_str(),output.size()+1,0); // send the respond of the server to the client

            }
            else if(((list1.contains("withdraw"))|| (list1.contains("telephoneBill"))||(list1.contains("donate")))){

                char*out;
                char*resptr;
                resptr=(char*)malloc(4096);
                if(list1.contains("withdraw"))
                    out=deposit_withdraw(resptr,list1,2);
                else if(list1.contains("telephoneBill")){
                    out=deposit_withdraw(resptr,list1,3);
				
                }
				else if(list1.contains("donate"))
					out=deposit_withdraw(resptr,list1,4);



                char out2[4096];
                bzero(out2,4097);
                for(int i=0;i<4096;i++)
                    out2[i]=*(out+i);
                string output (out2);
                free(resptr);

                send(fdescriptor,output.c_str(),output.size()+1,0);

            }
            else if(list1.contains("getamount")||list1.contains("createaccount")){

                char*out;
                char*resptr;
                resptr=(char*)malloc(4096);
                out=display_sp(resptr,list1);
                char out2[4096];
                bzero(out2,4097);
                for(int i=0;i<4096;i++)
                    out2[i]=*(out+i);
                string output (out2);
                free(resptr);

                send(fdescriptor,output.c_str(),output.size()+1,0);

            }
        }
        else{ 
		///if the data received from the  client is rubbish not a valid request to it resends it back

            send(fdescriptor, tester.c_str(), tester.size()+1,0);

        }
        if(tester == "exit"){ /// if client entered exit it will close this thread
            loop=true;
            break;
       }
    }
    emit terminate(); /// upon ending the connection the it sends signal event to close the thread and delete the worker instance made 
    l=" , , "; // upon 
    emit update(l,threadNo); /// upon ending the connection the clear the fields in the ui that contains the name of the client
    close(fdescriptor);
}




char*display_sp(char*resptr,QStringList clientList) // display a specific account or create  an account
{
	QString ID=clientList[2];
    QString accountNum=clientList[4];
    account ac;
    char id[14];
    int accon_no=accountNum.toInt();
    bool flag = false;
    ifstream inFile;
    string result;
    char res[4096];
    memset(res,0,4096);
    inFile.open("accountfile.dat", ios::binary); // the file that contains all the data of the users
    if (!inFile) // file not found so we creat it and create this new account
    {
        int cashmoney=0;
        if (((clientList.size()==7) && (clientList.contains("createaccount"))))
            cashmoney=clientList[5].toInt();
        else
            cashmoney=0;
        int no_of_account=write_account(clientList,cashmoney,true);
        result.append("New_account created with account number=");
        result.append(to_string(no_of_account));
        result.append("_With Cash Money=");
        result.append(to_string(cashmoney));
        strcpy(res,result.c_str());
        resptr=res;
        return resptr;
    }


    while (inFile.read(reinterpret_cast<char *> (&ac), sizeof(account))) // the account has been found so we return to the user its nedded data
    {

        for (int i=0;i<14;i++){
            id[i]=ac.client.Id[i];
        }
        QString nationalId (id);
        int comparison=QString::compare(ID,nationalId,Qt::CaseSensitive);

        if ((ac.retacno() == accon_no) && (comparison==0))
        {
            QString acn="Account NUmber=";
            QString dep="_Cash in the account=";
            QString nId="_National ID=";
            QString cash=QString::number(ac.retdeposit());
            QString reslt=acn+accountNum+dep+cash+nId+ID;
            result=reslt.toUtf8().constData();
            flag = true;
            strcpy(res,result.c_str());


        }
    }
    inFile.close();
    if (flag == false){
		int cashmoney=0;
        if (((clientList.size()==7) && (clientList.contains("createaccount"))))
            cashmoney=clientList[5].toInt();
        else
            cashmoney=0;
        int no_of_account=write_account(clientList,cashmoney,false);
        result.append("New_account created with account number=");
        result.append(to_string(no_of_account));
        result.append("_With Cash Money=");
        result.append(to_string(cashmoney));
        strcpy(res,result.c_str());

    }

    resptr=res;
    return resptr;
}
// client list {name,age,id,old,account_no,cash}
void  transaction_file_update(string transFile,account curr_ac,int donation,int draw,int deposit){ 
/// function to update the file that cotains all the transactions and deposition / or withdrawing done by the client
/// this file name is the last two numbers of national Id_accountnumber example id=12345678911125 and account number=20 so the file name is 25_20.txt
	ofstream transacfile;
	transacfile.open(transFile,ios::app);
	
    transacfile<<setw(10);
	transacfile<<left;
    transacfile<<(char *)&(curr_ac.client.name);//name
	
    transacfile<<setw(8);
	transacfile<<left;
    transacfile<<to_string(curr_ac.client.TeleBill);//Telephonebill

    transacfile<<setw(10);
	transacfile<<left;
    transacfile<<to_string(donation);//Donation
	
    transacfile<<setw(10);
	transacfile<<left;
    transacfile<<to_string(deposit);//Deposit
	

    transacfile<<setw(10);
	transacfile<<left;
    transacfile<<to_string(draw);//Withdraw

    transacfile<<setw(10);
    transacfile<<left;
    transacfile<<to_string(curr_ac.retdeposit());
	
    transacfile<<setw(10);
	transacfile<<left;
    transacfile<<(QLocale{QLocale::English}.toString(QDate(QDate::currentDate()), "dd.MM.yyyy")).toUtf8().constData();

    
	
    transacfile<<setw(10);
	transacfile<<left;
    transacfile<<(QLocale{QLocale::English}.toString(QDateTime(QDateTime::currentDateTime()), "hh:mm:ss")).toUtf8().constData()<<endl;
	transacfile.close();
	
	
	
}
int write_account(QStringList L,int money,bool newFile ) // function to write the data of the user in the file that contains all the user activities in binary and txt forms also create and update the personal file of each client
{
    account ac;
	int acc_auto=create_account_no();
    ofstream outFile;
    outFile.open("accountfile.dat", ios::binary | ios::app);
    ac.create_account(L[0],L[1],L[2],acc_auto,money);
    outFile.write(reinterpret_cast<char *> (&ac), sizeof(account));
    outFile.close();
    if(newFile==true)
        create_mainfile("allaccount.txt");
    create_update_mainfile("allaccount.txt",L,money,acc_auto);
	
   // ofstream readablefile,transacfile;
    
    string file_name;// the filename of each person
    file_name.append(L[2].right(2).toUtf8().constData());
    file_name.append("_");
    file_name.append(to_string(acc_auto));
    file_name.append(".txt");
    transaction_file_update(file_name,ac,0,0,money);// filename,account,donation,draw,money
    return acc_auto;
}

   
// autogenerate an account number 
int create_account_no(){
	int out=update_map(); // update the map that contains all the already used account numbers so the generated number is an ordered unique 
	if(out==0)
		return 0; // it means that there is no account saved at the server side so this is the first client and its account number will be 0
	map<int, int>::iterator itr;//the map that contains all the already used
    for (itr = all_acc_no.begin(); itr != all_acc_no.end(); ++itr) {
        if(itr->second==0){
             itr->second=1;
             return itr->first;
        }
      }     
}
int  update_map(){
	account ac;
	ifstream inFile;
    inFile.open("accountfile.dat", ios::binary);
	if(!inFile){
		return 0;
	}
	else{
		 while (inFile.read(reinterpret_cast<char *> (&ac), sizeof(account))){
			 map<int, int>::iterator it;
             it = all_acc_no.find(ac.retacno());
                 all_acc_no.find(ac.retacno())->second=1;
		
		 }
    }
	return 1;
}
//LIst L={Name,age,NationalId,acno,cash,deposit/withdraw}
char* deposit_withdraw(char*resptr,QStringList L, int type) /// this function allow deposition / withdrawing / donation and pay telephone bill
{
    bool found=false;
    account ac;
    fstream File;
    int n=L[3].toInt();
    int amt=0;
    if(type!=3)
        amt=L[4].toInt();
    string result;
    char res[4096];
    File.open("accountfile.dat", ios::binary|ios::in|ios::out);
    if(!File)
    {
        result="Error_while_uploading_the_data_from_the_server.";
        strcpy(res,result.c_str());
        resptr=res;
        return resptr;
    }
    while(!File.eof() && found==false)
    {
        File.read(reinterpret_cast<char *> (&ac), sizeof(account));
        if(ac.retacno()==n)
        {
            if(type==1) //deposit
            {
                ac.dep(amt);
                result.append("Your_Account_Now_Contains=");
                result.append(to_string(ac.retdeposit()));
                result.append("_$");
                strcpy(res,result.c_str());
                resptr=res;

            }
            else if(((type==2)||(type==3)||(type==4)))
            {
                int bal;
                if(((type==2)||(type==4))){
                     bal=ac.retdeposit()-amt;
                     if(bal<0){
                     result.append("Insufficient_balance");
                     result.append("_Your_balance_in_the_wallet_is");
                     result.append(to_string(ac.retdeposit()));
                     strcpy(res,result.c_str());
                     resptr=res;
                     }
                     else{
                         ac.draw(amt);
                         result.append("Your_account_now_contains=");
                         result.append(to_string(ac.retdeposit()));
                         result.append("_$");
                         strcpy(res,result.c_str());
                         resptr=res;
                     }
                }

                else if(type==3){

                     bal=ac.retdeposit()-ac.client.TeleBill;
                     if(bal<0){
                     result.append("Insufficience balance");
                     result.append("your Telephone Bill= ");
                     result.append(to_string(ac.client.TeleBill));
                     result.append(" and your balance in the wallet is ");
                     result.append(to_string(ac.retdeposit()));
                     strcpy(res,result.c_str());
                     resptr=res;
                     }
                     else{
                         ac.draw(ac.client.TeleBill);
                         result.append("your Telephone Bill was = ");
                         result.append(to_string(ac.client.TeleBill));
                         ac.client.TeleBill=0;
                         result.append(", Your account now contains ");
                         result.append(to_string(ac.retdeposit()));
                         result.append(" $");
                         strcpy(res,result.c_str());
                         resptr=res;
                     }
                }
            }
			/// update the data of the user in the binary file
            int pos=(-1)*static_cast<int>(sizeof(ac));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac), sizeof(account));
			
            string tranFile;
            tranFile.append(L[2].right(2).toUtf8().constData());
            tranFile.append("_");
            tranFile.append(to_string(ac.retacno()));
            tranFile.append(".txt");
            string newstring;
            newstring.append(to_string(ac.retdeposit()));
            newstring.append("$");
            get_pos("allaccount.txt",tranFile,newstring); 			/// update the data of the user in the text file that contains all the data


			if(type==1)//deposit
			{
                transaction_file_update(tranFile,ac,0,0,amt);// filename,account,donation,draw,deposit

			}
			else if(type==2)//withdraw
			{
            transaction_file_update(tranFile,ac,0,amt,0);// filename,account,donation,draw,deposit

			}
			else if(type==3)//telephonebill
			{
            transaction_file_update(tranFile,ac,0,0,0);// filename,account,donation,draw,deposit

			}
			else if(type==4)//donation
			{
            transaction_file_update(tranFile,ac,amt,0,0);// filename,account,donation,draw,deposit

			}
					
            found=true;
           }
         }
    File.close();
    if(found==false){
        result="Record_Not_Found";
        strcpy(res,result.c_str());
        resptr=res;
    }
    return resptr;

}

void get_pos(string input_file,string wordToFind,string newstring){
     string line; 
     int position=0; //initializing position with zero.
     int line_Number=0,found=0;

        fstream file,tempFile; // opening file input stream.
        file.open(input_file.c_str(),ios::in|ios::out);
        if(file) //checking if file is available.
        {
            tempFile.open("temp.txt",ios::app|ios::out);
            while(getline(file,line)) 
            {
                line_Number++; // increasing value of line_Number by 1 (line_Number=line_Number+1)
                for(int i=line.find(wordToFind); i<line.length(); i=i+position) // loop to find word in whole line.
                {
                    position=line.find(wordToFind,i); //making position equal to the position where word to find is.
                    if(position != string::npos) //if word is there in line
                    {

                       line.erase(position+10,position+25);
                       line.insert(position+10,newstring);
                        found=1;//making found equal to 1.

                        break;
                    }
                    else break; //if word not found in line the break the loop.
                }
                tempFile.seekp(0,ios::beg); // to preserve the already enetredc data in the file and prevent overwriting on the data
                tempFile<<line<<endl;
            }
            int line_Number2=0;
            string line2;
            while(getline(file,line2)){
                line_Number2++;
                if(line_Number2>line_Number){
                    tempFile.seekp(0,ios::beg);
                    tempFile<<line2<<endl;
                }

            }
            tempFile.close();
            file.close(); //closing file.
            if(found==0) //if found is zero (i.e., word is not in the file), then
            {
                
                qDebug()<<"Have no account";
            }
            remove(input_file.c_str());
            rename("temp.txt",input_file.c_str());
        }
        else //if file provided is not available.
        {
            qDebug()<<" the file of all accounts is not found";
        }


}

Worker::~Worker(){}

void create_update_mainfile(string filename,QStringList L, int cashmoney, int accnum) // by entering an new client its data is entered in the main text file and a new transaction  personal text file is created with the header of the columns 
{
    ofstream readablefile,transacfile;
    readablefile.open(filename,ios::app|ios::out);
    readablefile.seekp(0,ios::beg);
    readablefile<<setw(10);
    readablefile<<left;
    readablefile<<(L[0].toUtf8().constData());//name

    readablefile<<setw(7);
    readablefile<<left;
    readablefile<<L[1].toUtf8().constData();//age

    readablefile<<setw(25);
    readablefile<<left;
    readablefile<<L[2].toUtf8().constData();//ID

    QString acc_auto_no=QString::number(accnum);
    readablefile<<setw(7);
    readablefile<<left;
    readablefile<<acc_auto_no.toUtf8().constData();//Account NUmber



    string personalfilename;
    personalfilename.append(L[2].right(2).toUtf8().constData());
    personalfilename.append("_");
    personalfilename.append(to_string(accnum));
    personalfilename.append(".txt");

    readablefile<<setw(10);
    readablefile<<left;
    readablefile<<personalfilename;//filename

    QString cash_money=QString::number(cashmoney);
    readablefile<<setw(15);
    readablefile<<left;
    readablefile<<cash_money.toUtf8().constData()<<endl;//Cash
    readablefile.close();

    transacfile.open(personalfilename,ios::app|ios::out);

    transacfile<<setw(10);
    transacfile<<left;
    transacfile<<"Name";

    transacfile<<setw(8);
    transacfile<<left;
    transacfile<<"Bill";

    transacfile<<setw(10);
    transacfile<<left;
    transacfile<<"Donation";

    transacfile<<setw(10);
    transacfile<<left;
    transacfile<<"Deposit";

    transacfile<<setw(10);
    transacfile<<left;
    transacfile<<"Withdraw";

    transacfile<<setw(10);
    transacfile<<left;
    transacfile<<"Balance";

    transacfile<<setw(10);
    transacfile<<left;
    transacfile<<"Date";

    transacfile<<setw(10);
    transacfile<<left;
    transacfile<<"Time"<<endl;
    transacfile.close();


}

void create_mainfile(string filename) // create the text file that contains all the data of all clients with the headers of the file 
{
    ofstream readablefile;
    readablefile.open("allaccount.txt",ios::app);

        readablefile<<setw(10);
        readablefile<<left;
        readablefile<<"Name";

        readablefile<<setw(7);
        readablefile<<left;
        readablefile<<"Age";

        readablefile<<setw(25);
        readablefile<<left;
        readablefile<<"ID";

        readablefile<<setw(7);
        readablefile<<left;
        readablefile<<"AccNum";



        readablefile<<setw(10);
        readablefile<<left;
        readablefile<<"File";

        readablefile<<setw(15);
        readablefile<<left;
        readablefile<<"Cash"<<endl;
        readablefile.close();
}
