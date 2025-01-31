#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include<string>
#include<QMutex>
#include"account.h"
using namespace std;
char* display_sp(char*arr,QStringList listclient);
int write_account(QStringList Listclient,int money,bool newFile);
int create_account_no();
int  update_map();
char* deposit_withdraw(char *resptr,QStringList, int);
void get_pos(string input_file,string wordToFind,string newstring);
void create_mainfile(string filename);
void create_update_mainfile(string filename,QStringList L,int cashmoney,int accnum);
void  transaction_file_update(string transFile,account curr_ac,int donation,int draw,int deposit);

//QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void ThreadCaller(int,int);

    virtual ~MainWindow();

public slots:
    void update_ui(QString,int);


private slots:
    void on_pushButton_clicked();
private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H



#ifndef WORKER_H
#define WORKER_H


class Worker : public QObject
{
    Q_OBJECT
public:
     Worker(int x,QObject *parent = nullptr);
     int threadNo;
     int fdescriptor;

     ~Worker();

public slots:
     void readFromClient();




signals:
    void terminate();
    void update(QString,int);

};

#endif // WORKER_H
