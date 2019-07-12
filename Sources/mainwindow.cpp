#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"ui_form.h"

#include "tuile.h"
#include "famillestate.h"

#include<QTimer>
#include<QMenuBar>
#include <QDebug>
#include <QInputDialog>
#include <QStyle>
#include <QMessageBox>

#include <QMediaPlayer>

#define n 24

// Version 7.1 


MainWindow::MainWindow(QString s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    playerOne(""),
    playerTwo("")
{
    ui->setupUi(this);
    errorFlag = false;
    this->gameMode = s;
    startGame();
    setWindowTitle(tr("MAHJONG++"));
    setWindowIcon(QIcon(":/assets/icon.png"));
    if(!this->errorFlag) {
        tempTuileOne = new Tuile(this, 0, 0);
        tempTuileTwo = new Tuile(this, 0, 0);
        tempTuileOne->setVisible(false);
        tempTuileTwo->setVisible(false);

        t = new QTimer(this);
        connect(t,SIGNAL(timeout()),this,SLOT(jouer()));
        t->start(1000);

        QMenu *Menu = menuBar()->addMenu("&Menu");
        QAction *Restart = new QAction("&Rejouer", this);
        Menu->addAction(Restart);
        Restart->setShortcut(QKeySequence("Ctrl+N"));
        QAction *actionQuitter = new QAction("&Quitter", this);
        Menu->addAction(actionQuitter);
        actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
        ui->pushButton->setShortcut(QKeySequence("Ctrl+P"));
        QMenu *Regles=menuBar()->addMenu("&Regles");
        QAction *regles = new QAction("&Regles Principales", this);
        Regles->addAction(regles);
        regles->setShortcut(QKeySequence("Ctrl+R"));
        QMenu *Apropos = menuBar()->addMenu("&À propos");
        QAction *about = new QAction("&À propos", this);
        Apropos->addAction(about);
        about->setShortcut(QKeySequence("Ctrl+A"));
        connect(regles, SIGNAL(triggered()), this, SLOT(MenuRegles()));
        connect(actionQuitter, SIGNAL(triggered()),qApp, SLOT(quit()));
        connect(Restart, SIGNAL(triggered()),this, SLOT(Rejouer()));
        connect(about, SIGNAL(triggered()), this, SLOT(menuAbout()));
        connect(ui->pushButton,SIGNAL(clicked()),t,SLOT(on_pushButton_clicked()));
        connect(ui->j1shuffleButton,SIGNAL(clicked()),this,SLOT(emitShuffleDecrementOne()));
        connect(ui->j2shuffleButton,SIGNAL(clicked()),this,SLOT(emitShuffleDecrementTwo()));
        connect(ui->soundswitch,SIGNAL(clicked()),this,SLOT(switchSound()));
        ui->soundswitch->setStyleSheet("#soundswitch { border-image: url(:/assets/buttons/sound_off.png) }");


        soundIsOn = true;
        tuileClickedSound = new QMediaPlayer();
        tuileClickedSound->setMedia(QUrl("qrc:/assets/sounds/tuileClick.mp3"));

        tuileValidSound = new QMediaPlayer();
        tuileValidSound->setMedia(QUrl("qrc:/assets/sounds/tuileValid.mp3"));

        tourSound = new QMediaPlayer();
        tourSound->setMedia(QUrl("qrc:/assets/sounds/tour.wav"));

        timerSound = new QMediaPlayer();
        timerSound->setMedia(QUrl("qrc:/assets/sounds/timer.wav"));

        winnerSound = new QMediaPlayer();
        winnerSound->setMedia(QUrl("qrc:/assets/sounds/winner.flac"));

        shuffleSound = new QMediaPlayer();
        shuffleSound->setMedia(QUrl("qrc:/assets/sounds/shuffle.wav"));

        backgroundMusic = new QMediaPlayer();
        backgroundMusic->setMedia(QUrl("qrc:/assets/sounds/music.mp3"));

        if(backgroundMusic->state() == QMediaPlayer::StoppedState && soundIsOn) {
            backgroundMusic->play();
        }

        needToShuffle = 0;
        this->errorFlag = false;
    }
}

void MainWindow::startGame() {
    qDebug() << "Commencer le jeu";
    if (setPlayers()) {
        if(setGameLevel())
            setGamePartie();
        else this->errorFlag = true;
    }
    else this->errorFlag = true;
}


bool MainWindow::setPlayers() {
    if (this->gameMode == "JVJ") {
        bool ok;
        QString gamer1 = QInputDialog::getText(this,
                                               tr("Le nom du joueur 1"),
                                               tr("Le nom :"),
                                               QLineEdit::Normal,
                                               tr(""), &ok);
        if (ok && !gamer1.isEmpty()) {
            qDebug() << "Bonjour" << gamer1;
            this->playerOne.setName(gamer1);
            bool ok;
            QString gamer2 = QInputDialog::getText(this,
                                                   tr("Le nom du joueur 2"),
                                                   tr("Le nom :"),
                                                   QLineEdit::Normal,
                                                   tr(""), &ok);
            if (ok && !gamer2.isEmpty()) {
                qDebug() << "Bonjour" << gamer2;
                this->playerTwo.setName(gamer2);
                return true;
            }
            return false;
        }
    }
    if (this->gameMode == "JVO") {
        bool ok;
        QString gamer1 = QInputDialog::getText(this,
                                               tr("Le nom du joueur 1"),
                                               tr("Le nom :"),
                                               QLineEdit::Normal,
                                               tr(""), &ok);
        if (ok && !gamer1.isEmpty()) {
            qDebug() << "Bonjour" << gamer1;
            this->playerOne.setName(gamer1);
            this->playerTwo.setName("INAO");
        }
        return true;
    }
    if (this->gameMode == "DEMO") {
        this->playerOne.setName("Joueur 1");
        this->playerTwo.setName("Joueur 2");
        QMessageBox msgBox;
        msgBox.setText("Bienvenue dans le mode DEMO !\n\nCliquez sur OK pour lancer un tutorial du jeu.");
        msgBox.exec();
        return true;
    }
    return false;
}

bool MainWindow::setGameLevel() {
    if(this->gameMode != "DEMO") {
        bool ok;
        QStringList gameLevelsList;
        if(this->gameMode == "JVJ") {
            gameLevelsList << tr("Facile") << tr("Moyen") << tr("Difficile");
        }
        else gameLevelsList << tr("Facile") << tr("Moyen")<< tr("Difficile");
        QString gameLevelChosen = QInputDialog::getItem(this, tr("Niveau du jeu"),
                                                        tr("Niveau :"), gameLevelsList, 0, false, &ok);
        if (ok && !gameLevelChosen.isEmpty()) {
            this->gameLevel = gameLevelChosen;
            qDebug() << "Level :" << gameLevelChosen;
            if (this->gameLevel == "Facile") this->chosenChrono = 15;
            if (this->gameLevel == "Moyen") this->chosenChrono = 20;
            if(this->gameLevel == "Difficile") this->chosenChrono = 25;

            return true;
        }
    }
    if (this->gameMode == "DEMO") {
        this->gameLevel = "Facile";
        this->chosenChrono = 15;
        return true;
    }
    return false;

}

void MainWindow::setGamePartie() {

    chronoRev = chosenChrono;
    tour = 0;
    nbrClick = 0;
    nbrpair = 0;

    if(this->gameLevel == "Facile") {
        qDebug() << "Loading Board for facile";
        B = new Board(this, 36);

    }
    if(this->gameLevel == "Moyen" || this->gameLevel == "Difficile") {
        qDebug() << "Loading Board for moyen";
        B = new Board(this, 56);
        B->bonShuffle(this);
    }

    // Let's build state of current familles in Board


    stateTable = new FamilleState[B->nbrFamille];
    fillTable();
    displayTable();

    if(!QString::compare(this->gameLevel, "Difficile") && !QString::compare(this->gameMode, "JVJ")) {
        qDebug() << "Loading Board for difficile flipped";
        B->flipAllBack(); //** EXPERIMENTAL
    }

    ui->boardContainer->addWidget(B);
    ui->joueur1->setText(QString("%1").arg(this->playerOne.getName()));
    ui->joueur2->setText(QString("%1").arg(this->playerTwo.getName()));

    ui->j1score->setText(QString("%1").arg(this->playerOne.getScore()));
    ui->j2score->setText(QString("%1").arg(this->playerTwo.getScore()));

    ui->j1shuffle->setText(QString("%1").arg(this->playerOne.getShuffleLeft()));
    ui->j2shuffle->setText(QString("%1").arg(this->playerTwo.getShuffleLeft()));

    QString levelImgStyle = "#levelLabel { border-image: url(:/assets/buttons/" + this->gameLevel + ".png) }";
    ui->levelLabel->setStyleSheet(levelImgStyle);
    ui->j1tour->setHidden(false);
    ui->j2tour->setHidden(true);
    ui->j2shuffleButton->setDisabled(true);
    ui->j1shuffleButton->setDisabled(false);
} 

void MainWindow::fillTable() {
    B->setPriorite();
    qDebug() << "FILLING TABLE";
    B->tabTuilesLeftGenerator();
    B->tabTuilesLibresGenerator();
    B->triTabTuilesLibres(B->tabTuilesLibres , B->nbrTuilesLibres);
    for(int i(0); i < B->nbrFamille; i++) {
        stateTable[i].famille = B->famillesUsed[i] + 1;
        int pairFound = 0;
        int maxBlock = 0;
        for(int j(0); j < B->nbrTuilesLibres; j++) {

            if(B->tabTuilesLibres[j]->famille == stateTable[i].famille) {
                maxBlock = 0;
                pairFound++;
                stateTable[i].famillePositions << B->tabTuilesLibres[j]->position;
                stateTable[i].famillePriorite.append(B->tabTuilesLibres[j]->Pr);
                stateTable[i].maxBlock += B->tabTuilesLibres[j]->Pr;
            }
        }
        stateTable[i].familleLeft = stateTable[i].famillePositions.length();
    }
}

void MainWindow::updateTable() {
    qDebug() << "UPDATING TABLE";
    for(int i(0); i < B->nbrFamille; i++) {
        stateTable[i].famillePositions.clear();
        stateTable[i].famillePriorite.clear();
        stateTable[i].maxBlock = 0;
    }
    fillTable();
}

void MainWindow::sortTable() {

    //MultipleFirst Heuristic
    if(this->gameLevel == "Moyen"){
        for(int i(0); i < B->nbrFamille - 1; i++) {
            for(int j(i+1); j < B->nbrFamille; j++) {
                FamilleState temp;
                if(stateTable[i] <= stateTable[j]) {
                    temp = stateTable[i];
                    stateTable[i] = stateTable[j];
                    stateTable[j] = temp;
                }
            }
        }
    }

    //MaxBlock Heuristic
    else if (this->gameLevel == "Difficile") {
        for(int i(0); i < B->nbrFamille - 1; i++) {
            for(int j(i+1); j < B->nbrFamille; j++) {
                FamilleState temp;
                if(stateTable[i] < stateTable[j]) {
                    temp = stateTable[i];
                    stateTable[i] = stateTable[j];
                    stateTable[j] = temp;
                }
            }
        }
    }
}


void MainWindow::displayTable() {
    for(int i(0); i < B->nbrFamille; i++) {
        qDebug() << "CHOSEN FAMILLE : " << stateTable[i].famille;
        qDebug() << "TUILES LEFT :" << stateTable[i].getFamilleLeft();
        for(int j = 0; j < stateTable[i].familleLeft; j++) {
            qDebug() << stateTable[i].famille << "WAS FOUND IN POSITON " << stateTable[i].famillePositions.at(j) << "AND HAS PRIORITY" << stateTable[i].famillePriorite.at(j);
        }
        qDebug() << stateTable[i].famille << "POSSEDE" <<  stateTable[i].getFamillePaires() << "PAIRES" << "AND MAXBLOCK = "<< stateTable[i].maxBlock;
    }
}

void MainWindow::jouer(){

    ui->timer->setText(QString(" %1 s").arg(this->chronoRev));

    if(timerSound->state() == QMediaPlayer::StoppedState && chronoRev != 0 && soundIsOn) {
        timerSound->play();
    }

    if(this->gameMode == "DEMO") {
        if(!tour) {
            if(chronoRev == 15) {
                QMessageBox msgBox;
                msgBox.setText("Voici l'interface principale du jeu. \n"
                               "Le but du jeu est de faire disparaitre le maximum possible des tuiles identiques.");
                msgBox.exec();
                QMessageBox msgBoxTwo;
                msgBoxTwo.setText("Tuile transparente / grise = bloquée (Elle ne peut pas être sélectionnée.");
                msgBoxTwo.exec();
            }
            recherchePaire();
        }
        if(tour) {
            QMessageBox msgBoxTwo;
            msgBoxTwo.setText("J'espère que le jeu est clair. Vous pouvez jouer maintenant !");
            msgBoxTwo.exec();
            Rejouer();
        }
    }


    chronoRev--;
    AI++;
    if(chronoRev > 22 && this->gameLevel == "Difficile")
        B->flipAllFace();
    if(nbrpair != 0 && nbrClick == 0 && this->gameLevel == "Difficile" && this->gameMode == "JVJ") B->flipAllBack();


    if(chronoRev < 0){
        if(tourSound->state() == QMediaPlayer::StoppedState && soundIsOn) {
            tourSound->play();
        }
        tour++;
        nbrClick = 0;
        nbrpair=0;
        needToShuffle = 0;
        tempTuileOne->position = 0;
        chronoRev = chosenChrono;
        if(tour %2 != 0) {
            ui->j1tour->setHidden(true);
            ui->j2tour->setHidden(false);
            ui->j1shuffleButton->setDisabled(true);
            ui->j2shuffleButton->setDisabled(false);
        }
        if(tour %2 == 0) {
            ui->j1tour->setHidden(false);
            ui->j2tour->setHidden(true);
            ui->j1shuffleButton->setDisabled(false);
            ui->j2shuffleButton->setDisabled(true);
        }
    }



    if(B->boardVide()) {
        t->stop();
        winner();
    }
    if(B->nbrTuilesLibres < 2 || (B->nbrTuilesLibres == 2 && B->tabTuilesLibres[0]->famille != B->tabTuilesLibres[1]->famille)) winner();
    else{
            if (this->gameMode == "JVO"){
                if(this->gameLevel != "Difficile"){
                    if(AI%2 == 0 && tour % 2 != 0) {
                        if(this->gameLevel == "Facile")
                            recherchePaire();
                        else {
                            smartINAO();
                        }
                    }
                }
                else {
                    if(AI%1 == 0 && tour % 2 != 0) {
                        smartINAO();
                        B->flipAllFace();
                    }
                }
            }
    }

}

void MainWindow::emitShuffleDecrementOne() {
    if (playerOne.getShuffleLeft()) {
        if(shuffleSound->state() == QMediaPlayer::StoppedState && soundIsOn) {
            qDebug() << "PLAYING SOUND 1";
            shuffleSound->play();
        }
        if(this->gameLevel=="Facile") B->randomShuffle();
        if(this->gameLevel=="Moyen" || this->gameLevel=="Difficile") B->bonShuffle(this);

                qDebug() << "BOARD SHUFFLED SUCCESSFULLY";
                updateTable();
                displayTable();
                nbrpair = 0;
                this->playerOne.shuffleDecrement();
                ui->j1shuffle->setText(QString("%1").arg(this->playerOne.getShuffleLeft()));


    }
    if(this->gameMode == "DEMO") {
        QMessageBox msgBox;
        msgBox.setText("Chaque joueur a le droit de faire le Shuffle 3 fois au maximum.");
        msgBox.exec();
    }
    if (!playerOne.getShuffleLeft()) {
        ui->j1shuffleButton->setStyleSheet("#j1shuffleButton { background-color: rgba(0, 0, 0, 0) }");
        ui->j1shuffle->setStyleSheet("#j1shuffle { color: rgba(0, 0, 0, 0) }");
        ui->j1shuffleButton->setDisabled(true);
    }
}

void MainWindow::emitShuffleDecrementTwo() {
    if (playerTwo.getShuffleLeft()) {
        if(shuffleSound->state() == QMediaPlayer::StoppedState && soundIsOn) {
            qDebug() << "PLAYING SOUND 1";
            shuffleSound->play();
        }
        if(this->gameLevel=="Facile") B->randomShuffle();
        if(this->gameLevel=="Moyen" || this->gameLevel=="Difficile") B->bonShuffle(this);
                 qDebug() << "BOARD SHUFFLED SUCCESSFULLY";
                updateTable();
                displayTable();
                nbrpair = 0;
                this->playerTwo.shuffleDecrement();
                ui->j2shuffle->setText(QString("%1").arg(this->playerTwo.getShuffleLeft()));


    }
    if (!playerTwo.getShuffleLeft()) {
        ui->j2shuffleButton->setStyleSheet("#j2shuffleButton { background-color: rgba(0, 0, 0, 0) }");
        ui->j2shuffle->setStyleSheet("#j2shuffle { color: rgba(0, 0, 0, 0) }");
        ui->j2shuffleButton->setDisabled(true);
    }
}

void MainWindow::on_pushButton_clicked()
{
    pause =! pause;
    if (pause) {
        t->stop();
        B->flipAllBack();
        if(!pauseScreen()) {
            t->start();
            pause =! pause;
            B->flipAllFace();
        };
    }
    else
        t->start();
}

bool MainWindow::pauseScreen() {
    QMessageBox *buttonBox = new QMessageBox();
    buttonBox->setWindowTitle("Pause");
    buttonBox->setText("Le jeu est interrompu. Cliquer sur Reprendre !");
    qDebug() << "Pause screen works";
    buttonBox->setIconPixmap(QPixmap(":/assets/regles.png").scaled(650,659));
    QPushButton *resumeBtn = new QPushButton(tr("&Reprendre"));
    buttonBox->addButton(resumeBtn, QMessageBox::ActionRole);
    buttonBox->exec();
    if (buttonBox->clickedButton() == resumeBtn) {
        return false;
    }
    else return true;
}

bool MainWindow::aboutPopup() {
    QMessageBox *aboutBox = new QMessageBox();
    aboutBox->setWindowTitle("À Propos");
    aboutBox->setText("Mahjong++\n\nLe jeu est crée dans le cadre du module de Projet de Semestre du 1e année Genie Informatique, à l'ENSA de Marrakech."
                      "\n\nDéveloppé par : "
                      " \n\tLahmidi Oussama"
                      " \n\tHasnaoui Inas"
                      " \n\tFattas Amine"
                      " \n\tBouziti Nouhaila"
                      " \n\tAhbane Abdellah"
                      "\n\n © 2019 - MAHJONG++ 6.0");
    QPushButton *resumeBtn = new QPushButton(tr("&Reprendre"));
    aboutBox->addButton(resumeBtn, QMessageBox::ActionRole);
    aboutBox->exec();
    if (aboutBox->clickedButton() == resumeBtn) {
        return false;
    }
    else return true;
}


int MainWindow::calculScore(int score){
    switch(nbrpair) {
    case 0: score = 0; break;
    case 1: score = 10; break;
    case 2: score = 20; break;
    case 3: score = 40; break;
    default: score = 60; break;
    }
    return score;
}

void MainWindow::tuileClicked(int id, int famille) {
    qDebug() << "Tuile id = " << id << "et famille = " << famille;
    nbrClick++;
    qDebug() << "CLICK AFTER = " << nbrClick;
    if(tuileClickedSound->state() == QMediaPlayer::StoppedState && soundIsOn) {
        qDebug() << "PLAYING SOUND 1";
        tuileClickedSound->play();
    }
    if (nbrClick == 1) {
        tempTuileOne->position = id;
        tempTuileOne->famille = famille;
        if(this->gameLevel == "Difficile") B->tabTuiles[tempTuileOne->position]->flipTuileFace();
        qDebug() << "Tuile 1 rempli";
    }
    if (nbrClick == 2) {
        tempTuileTwo->position = id;
        tempTuileTwo->famille = famille;
        if(this->gameLevel == "Difficile") B->tabTuiles[tempTuileTwo->position]->flipTuileFace();
        qDebug() << "Tuile 2 rempli";

        if (tempTuileOne->position != tempTuileTwo->position
                && tempTuileOne->famille == tempTuileTwo->famille) {
            nbrpair++;
            qDebug() << "Les tuiles sont identiques. nbrCLick = " << nbrClick << " nbrpair = " << nbrpair;
            if(tuileValidSound->state() == QMediaPlayer::StoppedState && soundIsOn) {
                qDebug() << "PLAYING SOUND 1";
                tuileValidSound->play();
            }
            nbrClick = 0;
            B->removeTuile(tempTuileOne->position,this); // O(10) [1e ordre]
            B->removeTuile(tempTuileTwo->position,this); // O(10) [1e ordre]
            B->updateBoard();
          if(this->gameLevel=="Moyen"||this->gameLevel=="Difficile") { updateTable();
            displayTable();}
            //            if(this->gameLevel == "Difficile") B->flipAllBack();
            if(tour % 2 != 0){
                score = playerTwo.getScore();
                score += calculScore(score);
                playerTwo.setScore(score);
                ui->j2score->setText(QString(" %1 ").arg(this->playerTwo.getScore()));
            }
            if(tour % 2 == 0){
                score = playerOne.getScore();
                score += calculScore(score);
                playerOne.setScore(score);
                ui->j1score->setText(QString(" %1 ").arg(this->playerOne.getScore()));
            }
        }
        else {
            nbrClick = 1;
            qDebug() << "CLICK BEFORE = " << nbrClick;
            if(tempTuileOne->position != tempTuileTwo->position && nbrpair != 0 && this->gameLevel == "Difficile") {
                B->tabTuiles[tempTuileOne->position]->flipTuileBack();
            }

            tempTuileOne->position = tempTuileTwo->position;
            tempTuileOne->famille = tempTuileTwo->famille;

            tempTuileTwo->position = 0;
            tempTuileTwo->famille = 0;

            qDebug() << "Les tuiles ne sont pas identiques.  nbrpair = " << nbrpair;
        }
    }
}

void MainWindow::winner() {
    if(winnerSound->state() == QMediaPlayer::StoppedState) {
        qDebug() << "PLAYING SOUND 1";
        winnerSound->play();
    }
    QString winnerName;
    int winnerScore;
    if(playerOne.getScore() > playerTwo.getScore()) {
        winnerName = playerOne.getName();
        winnerScore = playerOne.getScore();
    }
    if(playerOne.getScore() < playerTwo.getScore()) {
        winnerName = playerTwo.getName();
        winnerScore = playerTwo.getScore();
    }
    if(playerOne.getScore() == playerTwo.getScore()){
        winnerScore = playerOne.getScore();
    }

    int answer = QMessageBox::information(this, "Le jeu est terminé",

                                          tr("Felicitations <B>%1<B> <br>").arg(winnerName) +tr(" <br> Votre score est de <B>%1</B> points <br> ").arg( QString::number(winnerScore) )+

                                          tr("<B> Vouler vous  Rejouer </B> ? ") ,QMessageBox::Yes | QMessageBox::No );

    if (answer == QMessageBox::Yes) Rejouer();
    else close();
}

void MainWindow:: Rejouer(){
    t->stop();
    B->flipAllBack();
    qDebug()<<"Rejouer";
    if(this->gameMode == "DEMO")this->gameMode = "JVJ";
    if (setPlayers()) {
        if(setGameLevel()) {
            delete B;
            // Restaurer l'affichage des boutons dans le cas où ils sont disparus.
            ui->j1shuffleButton->setStyleSheet("#j1shuffleButton { border-image: url(:/assets/buttons/shuffle_one.png) }");
            ui->j2shuffleButton->setStyleSheet("#j2shuffleButton { border-image: url(:/assets/buttons/shuffle_two.png) }");
            ui->j1shuffle->setStyleSheet("#j1shuffle { color: white }");
            ui->j2shuffle->setStyleSheet("#j2shuffle { color: white }");

            // Restauration des scores et des shuffles.
            this->playerOne.setScore(0);
            this->playerTwo.setScore(0);
            this->playerOne.setShuffle(3);
            this->playerTwo.setShuffle(3);

            setGamePartie();
            B->resetBoard(this);
            chronoRev = chosenChrono;
            tour = 0;
            t->start();
            nbrClick = 0;
            nbrpair = 0;
        }
        else {
            t->start();
            B->flipAllFace();
        }
    }
    else {
        t->start();
        B->flipAllFace();
    }
    t->start();
}

void MainWindow::recherchePaire(){
    Tuile *t = new Tuile;
    t = B->tabTuiles[B->selectTuile()];
    qDebug() << "La Tuile Choisie est de famille : " <<t->position<<"  "<<t->famille;
    B->tabTuiles[t->position]->flipTuileFace();
    for(int i=0;i<B->nbrTuilesLibres;i++){
        if(B->tabTuilesLibres[i]->famille == t->famille &&
                B->tabTuilesLibres[i]->position != t->position &&
                B->tabTuilesLibres[i]->existe && B->tabTuilesLibres[i]->famille){
            nbrpair++;
            qDebug() << "La 2e tuile : " << B->tabTuilesLibres[i]->famille;
            qDebug() << "Les tuiles sont identiques. nbrCLick = " << nbrClick << " nbrpair = " << nbrpair;
            if(this->gameMode == "DEMO" && nbrpair < 3) {
                QMessageBox msgBox;
                msgBox.setText(tr("Le joueur va sélectionner la tuile nº <B>%1<B> ").arg(t->position) +
                               tr(" et la tuile nº <B>%1</B> car elles sont identiques et libres. <br>").arg(B->tabTuilesLibres[i]->position));
                msgBox.exec();
            }
            if(tuileValidSound->state() == QMediaPlayer::StoppedState) {
                tuileValidSound->play();
            }
            nbrClick = 0;
            B->removeTuile(t->position, this);
            B->removeTuile(B->tabTuilesLibres[i]->position, this);
            B->updateBoard();
            score =playerTwo.getScore();
            score+= calculScore(score);
            if(this->gameMode != "DEMO") {
                playerTwo.setScore(score);
                ui->j2score->setText(QString(" %1 ").arg(this->playerTwo.getScore()));
            }
            if(this->gameMode == "DEMO" && nbrpair < 3) {
                playerOne.setScore(score);
                ui->j1score->setText(QString(" %1 ").arg(this->playerOne.getScore()));
                QMessageBox msgBox;
                msgBox.setText(tr("Le joueur gagne un nombre de points selon le nombre paires qu'il a trouvé. Son score maintenant est devenu <B>%1<B> ").arg(score));

                msgBox.exec();
            }
            if(needToShuffle) needToShuffle--; // Dès que INAO trouve un pair il va decrementer needToShuffle
            break;
        }
    }
    if(!nbrpair || nbrpair < 3) {
        needToShuffle++; // Par defaut = 0 , et il est incrementé à chaque fois INAO parcours le tableau et ne trouve aucun paire ou le nombre de paire qu'il a trouvé < 3
        qDebug() << "NEED TO SHUFFLE = " << needToShuffle;
        if(needToShuffle == 3) { // Dès que l'ordinateur fait 3 essais échéants il va shuffler.
            if(this->gameMode != "DEMO") emitShuffleDecrementTwo();
            if(this->gameMode == "DEMO" && !tour) {
                //                    QString messageBoxText = "Le joueur a sélectionné la tuile " + t->position + "";
                QMessageBox msgBox;
                msgBox.setText("Le joueur va faire le <b>Shuffle<b> car il n'a pas trouvé deux tuiles identiques.");
                msgBox.exec();
                emitShuffleDecrementOne();
            }
            needToShuffle = 0;
        }
    }
}
bool MainWindow:: MenuRegles(){
    pause =! pause;
    if (pause) {
        t->stop();
        if(!pauseScreen()) {
            t->start();
            pause =!pause;
        }
    }
    return false;
}

bool MainWindow::menuAbout() {
    pause =! pause;
    if (pause) {
        t->stop();
        B->flipAllBack();
        if(!aboutPopup()) {
            t->start();
            pause =! pause;
            B->flipAllFace();
        };
    }
    else
        t->start();
    return false;
}

void MainWindow::smartINAO(){
    qDebug() << "============ SMARTINAO ===============";
    int familleIndex = 0;
    sortTable();
    while(!stateTable[familleIndex].famillePaires && familleIndex < B->nbrFamille)
        familleIndex++;
    if(familleIndex == B->nbrFamille)
        return;
    if(stateTable[familleIndex].famillePaires) {
        qDebug() << "FAMILLE TO REMOVE : " << stateTable[familleIndex].famille << "AT INDEX" << familleIndex;
        qDebug() << "SELECTING FIRST";
        B->removeTuile(stateTable[familleIndex].famillePositions.at(0), this);
        B->removeTuile(stateTable[familleIndex].famillePositions.at(1), this);
        if(tuileValidSound->state() == QMediaPlayer::StoppedState) {
            tuileValidSound->play();
        }
        nbrpair++;
        B->updateBoard();
        qDebug() << "SHOT WAS SUCCESSFULLY DONE";
        updateTable();
        displayTable();
        int score = playerTwo.getScore();
        score += calculScore(score);
        playerTwo.setScore(score);
        ui->j2score->setText(QString(" %1 ").arg(this->playerTwo.getScore()));
    }
    else qDebug() << "I AM BLOCKED";
}

void MainWindow::switchSound() {
    qDebug() << "SWITCHING SOUND";
    soundIsOn = !soundIsOn;
    if(!soundIsOn) {
        ui->soundswitch->setStyleSheet("#soundswitch { border-image: url(:/assets/buttons/sound_off.png) }");
        if(backgroundMusic->state() == QMediaPlayer::PlayingState)
            backgroundMusic->pause();
    }
    else {
        ui->soundswitch->setStyleSheet("#soundswitch { border-image: url(:/assets/buttons/sound_on.png) }");
        if(backgroundMusic->state() == QMediaPlayer::PausedState)
            backgroundMusic->play();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



