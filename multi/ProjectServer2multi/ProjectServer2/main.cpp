#include "mainwindow.h"
#include <QApplication>
//#include<QString>
//#include<iostream>
//#include<string>
//#include<QDebug>
//#include<fstream>
//using namespace std;




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


}

