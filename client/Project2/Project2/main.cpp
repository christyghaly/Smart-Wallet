#include <QCoreApplication>
#include"account.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    char ch;
        int num;
        intro();
        do
        {
          

            cout<<"\n\n\t01.Connect to the Smart Wallet Server ";
            cout << "\n\n\t02. EXIT";
            cout << "\n\n\tSelect Your Option (1-9) ";
            cin >> ch;
            switch (ch)
            {
            case '1':
                connect_toserver();
                break;
            case '2':
                cout << "\n\n\tThanks for using bank managemnt system\n";
                return -1;
            default:cout << "\a";
            }
            cin.ignore();
            cin.get();
        } while (ch != '2');



    return a.exec();
}
