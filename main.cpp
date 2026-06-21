#include "login.h"
#include "user.h"
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initializeUsers();

    Login w;
    w.show();
    return a.exec();


}
