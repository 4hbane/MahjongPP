#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QMainWindow>
#include <QWidget>

#include "tuile.h"

#define N 56

class Board : public QWidget
{
    Q_OBJECT
public:
    Board(QWidget *parent = nullptr, int nbrMax = 0); // Constructeur de board
    ~Board();

    void addTuiles(QWidget*); // Fonction de remplissage du Board

    void setRelations(); // Distributeur des positions au tuiles

    void setPriorite();//distributeur des preriotaire au tuiles (NB les tuiles qui ne sont pa libres ont Pr = -1)

    void updateBoard(); // Fonction pour reinistialiser le Board

    Tuile** triTabTuilesLibres(Tuile**,int);//fct qui trier le tableau des tuiles libres selon les preriorites

    Tuile** tabTuilesLeftGenerator(); // Generateur de tableau de tuiles restantes dans le board

    Tuile **tabTuilesLibresGenerator(); // Generateur de tuiles libres dans le board

    int selectTuile(); // Fonction utilisée pour selectionner une position d'une tuile aleatoirement

    bool boardVide(); // = 1 si le Board est vide, 0 sinon

    void removeTuile(int,QWidget*); // Fonction pour supprimer une tuile  

    void resetBoard(QWidget*);


//pour niveau difficile:
    void flipAllBack();
    void flipAllFace();

// les fcts du shuffle:
    void randomShuffle(); // pour facile
    bool bonShuffle(QWidget*);//pour moyen et difficile



    int nbrMax; // Attribut de nombre de max de tuiles dans le Board
    int nbrTuilesDispo; // Attribut de nombre de tuiles restantes dans le Board
    int nbrFamille; // // Attribut de nombre de familles utilisées  dans le Board
    int nbrTuilesLibres; // Attribut de nombre de tuiles libres dans le Board
    int* famillesUsed; // Tableau de familles utilisées dans le Board
    int* listDistributionfamilles; // Tableau de distribution de familles dans le Board aleatoirement selon le nombre max de tuiles

    Tuile *tabTuiles[N]; // Tableau de tuiles
    Tuile **tabTuilesLeft; // Tableau de tuiles restantes dans le board
    Tuile **tabTuilesLibres; // Tableau de tuiles libres dans le board



};



#endif // BOARD_H
