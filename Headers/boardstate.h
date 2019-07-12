#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <QObject>
#include <QList>

class BoardState : public QObject
{
    Q_OBJECT
public:
    explicit BoardState(QObject *parent = nullptr);
    int famille;
    QList<int> famillePositionsDistrub;
    int familleNbrPaires;

    void updateBoardState();

signals:

public slots:
};

#endif // BOARDSTATE_H
