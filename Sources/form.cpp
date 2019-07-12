#include "form.h"
#include "ui_form.h"
#include<QTimer>
#include<QMenuBar>
#include <QDebug>
#include <QInputDialog>
#include <QStyle>
#include <QMessageBox>



Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)


{

    ui->setupUi(this);
    this->setStyleSheet("{background-image: url(:/bg.jpg);}");
    setWindowTitle(tr("MAHJONG++"));
    setWindowIcon(QIcon(":/assets/icon.png"));
}



void Form::on_jvj_clicked()
{
   w=new MainWindow("JVJ");
   if(!w->errorFlag) {
       hide();
       w->show();
   }
   else delete w;
}

void Form::on_jvo_clicked()
{
    w=new MainWindow("JVO");
    if(!w->errorFlag) {
        hide();
        w->show();
    }
    else delete w;
}

void Form::on_regles_clicked()
{
    QLabel *regles = new QLabel;
    regles->setPixmap(QPixmap(":/assets/regles.png").scaled(650,659));
    regles->show();
}

Form::~Form()
{
    delete ui;
}


void Form::on_demo_clicked()
{
    qDebug() << "LAUNCHING DEMO MODE";
    w=new MainWindow("DEMO");
    hide();
    w->show();

//    else delete w;
}
