#include "famillestate.h"


FamilleState::FamilleState(QObject *parent) : QObject(parent)
{
    maxBlock = 0;
    famillePaires = 0;
    familleLeft = 0;
}

int FamilleState::getFamillePaires() {
    this->famillePaires = this->famillePositions.length() / 2;
    return this->famillePaires;
}

int FamilleState::getFamilleLeft() {
    this->familleLeft = this->famillePositions.length();;
    return this->familleLeft;
}

bool FamilleState::operator<(const FamilleState &b) {
    return maxBlock < b.maxBlock;
}

bool FamilleState::operator<=(const FamilleState &b) {
    return famillePositions.length() < b.famillePositions.length();
}

FamilleState& FamilleState::operator=(const FamilleState& a)  {
    this->famille = a.famille;
    this->famillePaires = a.famillePaires;
    this->familleLeft = a.familleLeft;
    this->maxBlock = a.maxBlock;

    this->famillePriorite = a.famillePriorite;
    this->famillePositions = a.famillePositions;

    return *this;
}


