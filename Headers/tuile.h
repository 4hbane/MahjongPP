#ifndef TUILE_H
#define TUILE_H
#include <QLabel>//The QLabel widget provides a text or image display.


class Tuile : public QLabel //la tuile a une famille "Label"
{
    Q_OBJECT

public:
// Constructeur de la tuile
    Tuile(QWidget *parent = 0,
          int position = 0,
          int famille = 0,
          bool existe = 1
            );

//Fonctions:

    void updateTuile(); //Fonction pour mettre a jour la tuile apres le shuffle

    bool gaucheLibre(); //Fonction pour tester la liberté  à gauche du tuile
    bool droiteLibre(); //Fonction pour tester la liberté  à droite du tuile
    bool topLibre(); //Fonction pour tester la liberté  au top du tuile

    bool libre(); //Fonction pour verifier si la tuile est libre

    void clearTuile(QWidget*); //Fonction pour supprimer la tuile

    void resetTuile(QWidget*, int); //Fonction pour reinistialiser la tuile (signal, existe)

    Tuile& operator=(const Tuile&); // Operateur d affectation

//Attributs:

    int position; // Attribut de la position
    int famille; // Attribut de la famille
    bool existe; // Attribut pour l'existance (=1 si la tuile existe, 0 sinon)
    bool *gauche, *droite, *top; // Attributs des positions (gauche = 0, la tuile libre au gauche. de meme pour droite et top
    int Pr; // Priorité

//pour le niveau difficile:

    bool isFlipped;// attribut boolean :si tuile a une famille caché ou non
    void flipTuileBack();//tuile avec la famille 0 "sans famille precis"
    void flipTuileFace();//tuile avec famille


//destructeur de la tuile
 ~Tuile();

protected:
    void mousePressEvent(QMouseEvent *ev); // Pour envoyer un evenement de click avec (position, famille) au parent

signals:
    void clicked(int, int); // La fonction qui envoie (position, famille) au parent
};

#endif // TUILE_H
