#ifndef JOUEUR_H
#define JOUEUR_H

#include <QObject>

class Joueur
{

public:

//Constructeur de Joueur() avec shuffle = 3 par defaut
    Joueur(QString, int = 0, int = 3);

//setters:
    void setName(QString); //Setter de nom
    void setScore(int); //Setter de score
    void setShuffle(int); //Setter de Shuffle

//getters:
    QString getName(); //Getter de nom
    int getScore();  //Getter de score
    int getShuffleLeft(); //Getter de Shuffle


private://attributs:
    QString name; //Le nom
    int score; //Le score
    int shuffleLeft; //Le shuffle

public slots:
    void shuffleDecrement(); //Fonction pour decrementer le shuffle par un signal

};

#endif // JOUEUR_H
