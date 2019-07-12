#ifndef FORM_H
#define FORM_H
#include "mainwindow.h"
#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

public slots:

    void on_jvj_clicked();
    void on_jvo_clicked();

public:
    Ui::Form *ui;
    MainWindow *w;





private slots:
    void on_regles_clicked();
    void on_demo_clicked();
};

#endif // FORM_H
