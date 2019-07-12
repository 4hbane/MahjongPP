#ifndef STATETABLE_H
#define STATETABLE_H

#include <QObject>

#include "famillestate.h"

class StateTable : public QObject
{
    Q_OBJECT
public:
    explicit StateTable(int, QObject *parent = nullptr);

    FamilleState *f;

    void fillTable(int, int);

    void updateTable(int, int, int);

    void clearTable();

signals:

public slots:
};

#endif // STATETABLE_H
