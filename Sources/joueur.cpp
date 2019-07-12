#include "Joueur.h"
#include <QDebug>

Joueur::Joueur(QString n, int score, int shuffleLeft)
{
    this->name = n;
    this->score = score;
    this->shuffleLeft = shuffleLeft;
}

void Joueur::setName(QString n) {
    this->name = n;
}

void Joueur::setScore(int s) {
    this->score = s;
}

void Joueur::setShuffle(int sh) {
    this->shuffleLeft = sh;
}

QString Joueur::getName() {
    return this->name;
}

int Joueur::getScore() {
    return this->score;
}

int Joueur::getShuffleLeft() {
    return this->shuffleLeft;
}

void Joueur::shuffleDecrement() {
    if (this->shuffleLeft)
        this->shuffleLeft--;
}


