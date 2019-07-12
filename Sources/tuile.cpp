#include "tuile.h"
#include <QDebug>

Tuile::Tuile(QWidget *parent,
             int position,
             int famille,
             bool existe):QLabel(parent)
{
    QPixmap map;//The QPixmap class is an off-screen image representation that can be used as a paint device
    map.load(":/assets/formes/" + QString::number(famille) +".png");
    setPixmap(map.scaled(65, 85));//doner l'image(label) et les dimantios du tuile.
    bool defaultPosition = false; //** Vaut mieux l'appeler defaultPosition, car elle es juste pour decalrer la position par defaut pour la 1ere fois.
    //** Elle est declarée juste dans le constructeur on l'est besoin que pour la creation de la tuile.

    this->position = position; // Position de la tuile dans le Board
    this->famille = famille;
    this->gauche = &defaultPosition;
    this->droite = &defaultPosition;
    this->top = &defaultPosition;
    this->existe = existe;
    this->isFlipped = false;
//    this->Pr = -1;//Pr est attribut des tuiles qui sont libres ,(en initialiser a -1 par defaut).

    //lorsque on click sur une tuile (sighnal:clicked()),on passe au fct tuileclicked() dans mainwin (slot).
    connect(this,SIGNAL(clicked(int, int)),parent,SLOT(tuileClicked(int, int)));
} // O(11) [1e ordre]

void Tuile::mousePressEvent ( QMouseEvent * event ){ emit clicked(position, famille); }//pour retourner la position et famille du tuile

void Tuile::updateTuile() {
    if(existe) {
        QPixmap map;
        map.load(":/assets/formes/" + QString::number(famille) +".png");
        setPixmap(map.scaled(65, 85));
        setDisabled(false);
        if(!libre())
            setDisabled(true);


    }
}

bool Tuile::gaucheLibre() {
    return !(*gauche);
}

bool Tuile::droiteLibre() {
    return !(*droite);
}

bool Tuile::topLibre() {
    return !(*top);
}

bool Tuile::libre() {
    // la tuile est libre si :elle est libre au top et (libre a gauche ou a droite).
    return (topLibre() && (gaucheLibre() || droiteLibre()));
}

void Tuile::clearTuile(QWidget*p) {
    this->hide();
    this->existe = false;
    //pas de connection sur l'objet (signal et slot) "ne peut pas passer event du click"
    disconnect(this,SIGNAL(clicked(int, int)),p,SLOT(tuileClicked(int, int)));
}

void Tuile::flipTuileBack() {
    QPixmap map;
    map.load(":/assets/formes/0.png");
    setPixmap(map.scaled(65, 85));
}

void Tuile::flipTuileFace() {
    QPixmap map;
    map.load(":/assets/formes/" + QString::number(famille) +".png");
    setPixmap(map.scaled(65, 85));
}

void Tuile::resetTuile(QWidget *P, int fam) {
    this->existe = true;
    this->setDisabled(false);
    this->setHidden(false);
    this->famille = fam;
    QPixmap map;
    map.load(":/assets/formes/" + QString::number(fam) +".png");
    setPixmap(map.scaled(65, 85));

    connect(this,SIGNAL(clicked(int, int)),P,SLOT(tuileClicked(int, int)));
}

Tuile::~Tuile() {
    this->hide();
}

