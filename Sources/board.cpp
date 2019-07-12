#include "board.h"
#include"ui_mainwindow.h"
#include <QDebug>
#include <QMainWindow>
#include <algorithm>
#include <random>
#include<cstdlib>
#include<QBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

#include <QStack>
#include <QList>

#include "time.h"

using namespace std;

#define MAX_FAMILLE 14

Board::Board(QWidget *parent, int nbrMax) : QWidget(parent)
{
    qDebug() << "BUILDING BOARD";
    this->nbrMax = nbrMax;
    this->nbrTuilesDispo = nbrMax;


    listDistributionfamilles = new int[nbrMax];
    qDebug() << "FAMILY DISTRUB";

    (nbrMax > 8) ? this->nbrFamille = nbrMax / 4 : this->nbrFamille = nbrMax / 2;
    qDebug() << "Nombre de familles = " << nbrFamille;

    famillesUsed = new int[nbrFamille];

    for(int i= 0; i < nbrFamille; i++) {
        srand ( time(NULL)); //Fonction pour renforcer random
        int test = (rand() % MAX_FAMILLE) + 1;
        int repeated = 0;
        if(i > 0) {
            do {
                repeated = 0;
                for(int j = 0; j < i; j++) {
                    if(famillesUsed[j] == test) {
                        repeated = 1 ;
                        test = (rand() % MAX_FAMILLE) + 1;
                        break;
                    }
                }
            } while(repeated == 1);
        }
        if(repeated == 0) famillesUsed[i] = test;
    }
    qDebug() << "FAMILY USED CREATED";

    for(int i(1); i <= nbrMax / nbrFamille; i++) {
        for(int j(0); j < nbrFamille; j++) {
            listDistributionfamilles[j + (i - 1) * nbrFamille] = famillesUsed[j];
        }
    }
    qDebug() << "FAMILY DISTRUB CREATED";

    qDebug() << "FAMILY DISTRUB RANDOMIZED SUCCESSFULLY";

    addTuiles(parent);
    randomShuffle();
    qDebug() << "BOARD BUILT SUCCESSFULLY";
}

void Board::addTuiles(QWidget* P) {

    int x = 450;
    int y = 150;
    qDebug() << "ADDING TUILES";
    for(int i(0); i < 6; i++) {
        x = 450;
        for(int j(0); j < 6; j++) {
            qDebug() << "ADDING TUILE N = " << 6 * i + j << "FAMILLE = " << listDistributionfamilles[6 * i + j] + 1;
            // probleme famille = 24+1 n existe pas
            tabTuiles[6 * i + j] = new Tuile(P, 6 * i + j, listDistributionfamilles[6 * i + j] + 1);
            tabTuiles[6 * i + j]->setGeometry(x, y, 65, 85);
            x += 61;
        }
        y += 82;
    }
    if(nbrMax > 36) {
        x = 450 + 58;
        y = 150 + 77;
        for(int i(0); i < 4; i++) {
            x = 450 + 58;
            for(int j(0); j < 4; j++) {
                qDebug() << "ADDING TUILE N = " << 4 * i + j + 36 << "FAMILLE = " << listDistributionfamilles[4 * i + j + 36] + 1;
                // probleme famille = 24+1 n existe pas
                tabTuiles[4 * i + j + 36] = new Tuile(P, 4 * i + j + 36, listDistributionfamilles[4 * i + j + 36] + 1, 1);
                tabTuiles[4 * i + j + 36]->setGeometry(x, y, 65, 85);
                x += 61;
            }
            y += 82;
        }

        x = 450 + 2 * (58 - 3 / 2);
        y = 150 + 2 * (79 - 3 / 2);
        for(int i(0); i < 2; i++) {
            x = 450 + 2 * (58 - 3 / 2);
            for(int j(0); j < 2; j++) {
                qDebug() << "ADDING TUILE N = " << 2 * i + j + 52 << "FAMILLE = " << listDistributionfamilles[2 * i + j + 52] + 1;
                // probleme famille = 24+1 n existe pas
                tabTuiles[2 * i + j + 52] = new Tuile(P, 2 * i + j + 52, listDistributionfamilles[2 * i + j + 52] + 1, 1);
                tabTuiles[2 * i + j + 52]->setGeometry(x, y, 65, 85);
                x += 61;
            }
            y += 82;
        }
    }
    setRelations();
    setPriorite();
}

void Board::setRelations() {
    //chaque tuile pointe sur l'existe des tuiles a cotee et au tuile top.
    int n = 6;
    for(int i=0 ; i<n ; i++){
        for(int j =0 ; j<n ; j++){
            if(n*i+j!=n*i) {tabTuiles[n*i+j]->gauche = &(tabTuiles[n*i+j-1]->existe);}
            if(n*i+j!=n*i+5) {tabTuiles[n*i+j]->droite = &(tabTuiles[n*i+j+1]->existe);}
        }
    }
    n = 4;
    if(nbrMax == 56) {
        for(int i=0 ; i< n; i++){
            for(int j =0 ; j<n ; j++){
                if(n * i + j + 36!=n*i+36) {tabTuiles[n*i+j+36]->gauche = &(tabTuiles[n*i+j-1+36]->existe);}
                if(n*i+j + 36 !=n*i+3 + 36) {tabTuiles[n*i+j+36]->droite = &(tabTuiles[n*i+j+1+36]->existe);}

            }
        }
        tabTuiles[7]->top = &(tabTuiles[7+29]->existe);
        tabTuiles[8]->top = &(tabTuiles[8+29]->existe);
        tabTuiles[9]->top = &(tabTuiles[9+29]->existe);
        tabTuiles[10]->top = &(tabTuiles[10+29]->existe);

        tabTuiles[13]->top = &(tabTuiles[13+27]->existe);
        tabTuiles[14]->top = &(tabTuiles[14+27]->existe);
        tabTuiles[15]->top = &(tabTuiles[15+27]->existe);
        tabTuiles[16]->top = &(tabTuiles[16+27]->existe);

        tabTuiles[19]->top = &(tabTuiles[19+25]->existe);
        tabTuiles[20]->top = &(tabTuiles[20+25]->existe);
        tabTuiles[21]->top = &(tabTuiles[21+25]->existe);
        tabTuiles[22]->top = &(tabTuiles[22+25]->existe);

        tabTuiles[25]->top = &(tabTuiles[25+23]->existe);
        tabTuiles[26]->top = &(tabTuiles[26+23]->existe);
        tabTuiles[27]->top = &(tabTuiles[27+23]->existe);
        tabTuiles[28]->top = &(tabTuiles[28+23]->existe);


        tabTuiles[41]->top = &(tabTuiles[41+11]->existe);
        tabTuiles[42]->top = &(tabTuiles[42+11]->existe);

        tabTuiles[45]->top = &(tabTuiles[45+9]->existe);
        tabTuiles[46]->top = &(tabTuiles[46+9]->existe);


        tabTuiles[52]->droite = &(tabTuiles[52+1]->existe);
        tabTuiles[53]->gauche = &(tabTuiles[52]->existe);
        tabTuiles[54]->droite = &(tabTuiles[54+1]->existe);
        tabTuiles[55]->gauche = &(tabTuiles[54]->existe);
    }

}

void Board::setPriorite(){

       tabTuilesLibresGenerator();
        for(int i = 0 ; i<nbrTuilesLibres ; i++){
               if(*tabTuilesLibres[i]->droite) {
                   tabTuilesLibres[i]->Pr = - tabTuiles[tabTuilesLibres[i]->position+1]->libre();
                   tabTuilesLibres[i]->existe = 0;
                   tabTuilesLibres[i]->Pr += tabTuiles[tabTuilesLibres[i]->position+1]->libre();
                   tabTuilesLibres[i]->existe = 1;
               }
               else if(*tabTuilesLibres[i]->gauche) {
                   tabTuilesLibres[i]->Pr = - tabTuiles[tabTuilesLibres[i]->position-1]->libre();
                   tabTuilesLibres[i]->existe = 0;
                   tabTuilesLibres[i]->Pr += tabTuiles[tabTuilesLibres[i]->position-1]->libre();
                   tabTuilesLibres[i]->existe = 1;
               }
               else tabTuilesLibres[i]->Pr = 0;
               if(tabTuilesLibres[i]->position > 51) tabTuilesLibres[i]->Pr++;
               if(tabTuilesLibres[i]->position > 35) tabTuilesLibres[i]->Pr++;
    }
}

void Board::updateBoard() {
    qDebug() << "UPDATING BOARD";

    for(int i=0;i<nbrMax;i++){
        tabTuiles[i]->updateTuile();
    }
}

Tuile** Board::tabTuilesLeftGenerator() {
    int indice=0;
    Tuile** ex = new Tuile*[nbrTuilesDispo];
    for(int i=0;i<nbrMax;i++){
        if(tabTuiles[i]->existe){
            ex[indice]=tabTuiles[i];
            indice++;
        }
    }
    tabTuilesLeft=ex;
    return tabTuilesLeft;
}

Tuile** Board::tabTuilesLibresGenerator() {
    tabTuilesLeftGenerator();
    int indice=0;
    Tuile** libre = new Tuile*[nbrTuilesDispo];
    for(int i=0;i<nbrTuilesDispo;i++){
        if(tabTuilesLeft[i]->libre()){
            libre[indice]=tabTuilesLeft[i];
            indice++;
        }
    }
    tabTuilesLibres=libre;
    nbrTuilesLibres=indice;
    return tabTuilesLibres;
}

Tuile** Board::triTabTuilesLibres(Tuile** tabTuilesLibres,int nbrTuilesLibres){
    Tuile* temp;
    for (int i(0);i<nbrTuilesLibres;i++) {
        for (int j=i;j<nbrTuilesLibres;j++) {

            if(tabTuilesLibres[j]->Pr>tabTuilesLibres[i]->Pr){
                temp=tabTuilesLibres[i];
                tabTuilesLibres[i]=tabTuilesLibres[j];
                tabTuilesLibres[j]=temp;
            }
        }

    }return tabTuilesLibres;

}

int Board::selectTuile(){
    QList<int> listTuilesLibres;
    for(int i(0); i < nbrMax; i++) {
        if(tabTuiles[i]->existe && tabTuiles[i]->libre())
            listTuilesLibres.append(i);
    }
    int r = rand() % listTuilesLibres.length();
    qDebug() << "SELECTED TUILE = " << r;
    return listTuilesLibres.at(r);//donner la valeur de l'indice r
}

void Board::randomShuffle() {
    tabTuilesLeftGenerator();
    qDebug() << "Tuile left Generated";
    int *listFamillesLeft;
    listFamillesLeft = new int[nbrTuilesDispo];
    for(int i=0;i<nbrTuilesDispo;i++){
        listFamillesLeft[i]=tabTuilesLeft[i]->famille;
    }
    qDebug() << "New famille set.";
    std::random_shuffle(listFamillesLeft, listFamillesLeft + nbrTuilesDispo);
    for(int i=0;i<nbrTuilesDispo;i++){
        tabTuilesLeft[i]->famille=listFamillesLeft[i];
    }
    updateBoard(); // J'ai ajoute cette fonction pour fixer le blem.

}

bool Board::bonShuffle(QWidget *P) {
      int n = nbrTuilesDispo;
      QList<int> currentFamilles;//list des familles existants dans board
      QList<int> bonShuffleStack;//list des positios des tuiles existants ds board
      bool allExist[nbrMax];//noter l'existance des tuiles  car apres on fait remove puis on appel la fct resettuile
      Tuile* t1 = new Tuile;
      Tuile* t2 = new Tuile;
      int i(0);

    for(int i(0); i < nbrMax; i++) {
        if(tabTuiles[i]->existe)
            currentFamilles.append(tabTuiles[i]->famille);
        else currentFamilles.append(0);
    }
    qDebug() << "CURRENT FAMILLE CREATED SUCCESSFULLY";

    qSort(currentFamilles.begin(), currentFamilles.end());
    qDebug() << "CURRENT FAMILLE SORTED SUCCESSFULLY";


    for(int i(0); i < nbrMax; i++) {
        allExist[i] = tabTuiles[i]->existe;
    }


    qDebug() << "STACK FILLING";
    do {
        int errorHandler = 0;//on ajoute cette indice pour eviter boucle infini du do while
        do {
            t1 = tabTuiles[selectTuile()];
            t2 = tabTuiles[selectTuile()];
            errorHandler++;
        }while(t1->position == t2->position && errorHandler <= 500);
        if(errorHandler == 500)
            return false;
        bonShuffleStack.append(t1->position);
        bonShuffleStack.append(t2->position);
        removeTuile(t1->position,this);
        removeTuile(t2->position,this);
        i += 2;
    }while(!boardVide());

    qDebug() << "STACK IS FULL WITH " << bonShuffleStack.length() <<". GENERATING FAMILIES : " << currentFamilles.length() ;


    // Le code qui distribue les familles sera ajouté ici.
    for(int i(0); i < n; i ++) {
        nbrTuilesDispo += 1;
        tabTuiles[i]->existe = allExist[i];
        tabTuiles[i]->setHidden(!allExist[i]);

        tabTuiles[bonShuffleStack[i]]->resetTuile(P, currentFamilles[nbrMax - 1 - i]);
        qDebug() << tabTuiles[bonShuffleStack[i]]->position << "RECEIVED FAMILLE " << currentFamilles[nbrMax - 1 - i];


    }
    updateBoard();

    return true;
}

void Board::flipAllBack() {
    for(int i(0); i < nbrMax; i++) {
        tabTuiles[i]->flipTuileBack();
    }
}

void Board::flipAllFace() {
    for(int i(0); i < nbrMax; i++) {
        tabTuiles[i]->flipTuileFace();
    }
}

bool Board::boardVide() {
    if (!nbrTuilesDispo) return true;
    return false;
}

void Board::removeTuile(int id,QWidget*p) {
    this->tabTuiles[id]->clearTuile(p);
    nbrTuilesDispo -= 1;
}

void Board::resetBoard(QWidget *P) {
    nbrTuilesDispo = nbrMax;
    for(int i(0); i < nbrMax; i++) {
        tabTuiles[i]->resetTuile(P, listDistributionfamilles[i] + 1);
    }
   if(nbrMax==36) randomShuffle();
   if(nbrMax>36) bonShuffle(P);
}

Board::~Board() {
    for(int i(0); i < nbrMax; i++) {
        delete tabTuiles[i];
    }
}

