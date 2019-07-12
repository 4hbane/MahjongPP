#ifndef FAMILLESTATE_H
#define FAMILLESTATE_H

#include <QObject>
#include <QList>
 //cette class comme une base du donnee
// un grand tableau contient (famille , famillePositions , famillePriorite , famille left , famillePaires ,maxblock)
//ce tableau cree et generer dans mainwindow et utiliser par l'ordinateur
class FamilleState : public QObject
{
    Q_OBJECT
public:
    explicit FamilleState(QObject *parent = nullptr);

    int famille;
    QList<int> famillePositions; // list des positions des tuiles libres de meme famille
    int famillePaires;
    QList<int> famillePriorite; // list des priorites des tuiles dans chaque famille
    int familleLeft; // Nombre de tuiles restantes dans la table
    int maxBlock;//indique le max les tuiles liberont apres suppression des tuiles de meme famille

//getter:
    int getFamillePaires();
    int getFamilleLeft();
    int getMaxBlock(); // = somme de priorites

// Operateurs
    bool operator<(const FamilleState& b);
    bool operator<=(const FamilleState& b);

    FamilleState& operator=(const FamilleState&);


};

#endif // FAMILLESTATE_H
