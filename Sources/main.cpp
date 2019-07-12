#include "mainwindow.h"
#include "form.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Form f;
    f.show();

    return a.exec();
}
