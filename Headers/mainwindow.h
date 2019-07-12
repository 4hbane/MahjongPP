#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>
#include <QMediaPlayer>
#include <QStack>

#include "joueur.h"
#include "tuile.h"
#include "board.h"
#include "famillestate.h"

// La fenetre principale est celle qui gere l'ensemble des jeu ,y compris la logique et la distribution des parties


namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString, QWidget *parent = nullptr); // Constructeur de fenetre principale
    ~MainWindow();

    bool errorFlag;

    bool setPlayers(); // Setter des joueurs
    bool setGameLevel(); // Setter du niveau
    void setGamePartie(); // Setter de design de la partie (Interface de la partie)
    bool pauseScreen(); // Fonction pour suspendre le jeu
    int calculScore(int); // Fonction pour calculer le score pour le joueur courant
//    void createBoard();
    void winner(); // Fonction appelée dès que le Board soit vide et elle affiche le joueur gagnant

    bool aboutPopup();

    bool soundIsOn;
    QMenu *Menu; // Menu principale (pas encore implemente)
    QMenu *Regles ; // Menu pour afficher les regles (pas encore implemente)
    QMenu *Apropos; // Menu pour afficher 'a propos' (pas encore implemente)

    void recherchePaire(); // Fonction utilisée par l'ordinateur pour rechercher la tuile correspondante

    void fillTable();
    void sortTable();
    void updateTable();
    void clearTable();
    void displayTable();


public slots:
    void startGame(); // Fonction pour declancher le jeu
    void on_pushButton_clicked(); // Fonction du bouton 'PAUSE'
    void emitShuffleDecrementOne(); // Fonction pour decrementer le shuffle au joueur 1
    void emitShuffleDecrementTwo(); // Fonction pour decrementer le shuffle au joueur 2
    void jouer(); // Fonction qui gère la distribution des tour selon le cycle du chrono
    void Rejouer(); // Fonction pour reinitialiser tout les données pour relancer une nouvelle partie
    void tuileClicked(int, int); // Fonction pour comparer 2 tuiles
    bool MenuRegles();
    bool menuAbout();
    void switchSound();


private:
    Ui::MainWindow *ui;

    int chronoRev; // Le chrono du jeu
    int chosenChrono; // Le chrono initiale du jeu selon le niveau
    QTimer *t ; // Gestionneur du temps
    int tour; // Attribut de tour pour distinguer entre le tour de joueur 1 et le tour du joueur 2
    int nbrpair; // Attribut de nombre de paires trouvés durant un tour
    int nbrClick; // Attribut de nombre de cliques de tuiles durant un tour
    int score; // Un score temporaire pour aider la fonction qui calcul le score

    QPushButton *pushButton;  // ?
    bool pause = false; // Flag de pause (=1 si le jeu est suspendu, 0 sinon)

    QString gameLevel; // Attribut de niveau choisi (chaine de caracteres)
    QString gameMode; // Attribut de mode du jeu choisi (chaine de caracteres)

    Joueur playerOne; // Objet du joueur1
    Joueur playerTwo; // Objet du joueur2

    int AI; // Nombre aleatoire utilisé par l'ordinateur pour simuler le temps de reflexion et de recherche
    int needToShuffle;
    QStack<int> pickOrder;

    Board* B; // Objet de Board de tuile

    FamilleState *stateTable;

    Tuile *tempTuileOne; // Tuile temporaire utilisée pour enfiler la 1ere tuile selectionnée
    Tuile *tempTuileTwo; // Tuile temporaire utilisée pour enfiler la 2eme tuile selectionnée


    // Les sons implementés dans le jeu
    QMediaPlayer* tuileClickedSound;
    QMediaPlayer* tuileValidSound;
    QMediaPlayer* tourSound;
    QMediaPlayer* timerSound;
    QMediaPlayer* winnerSound;
    QMediaPlayer* shuffleSound;
    QMediaPlayer* backgroundMusic;

     void smartINAO();
};

#endif // MAINWINDOW_H
