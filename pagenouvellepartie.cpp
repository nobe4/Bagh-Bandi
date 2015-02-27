#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QFrame>
#include <QDebug>
#include "pagenouvellepartie.h"
#include "modele.h"

PageNouvelleP::PageNouvelleP(Partie **p_partie)
{
    this->setWindowTitle("Nouvelle partie");
    this->setWindowIcon(QIcon("icon_nouvelleP.png"));

    //definition de la partie
    partie = (*p_partie);

    //Champs de saisie des noms des joueurs
    nomJoueur = new QLineEdit*[2];
    nomJoueur[0] = new QLineEdit("Gilmour");
    nomJoueur[1] = new QLineEdit("Young");

    //layout de saisie des noms des joueurs
    QFormLayout *nomsJoueurs = new QFormLayout;
    nomsJoueurs->addRow("Nom joueur <strong>tigre</strong> :",nomJoueur[0]);
    nomsJoueurs->addRow("Nom joueur <strong>chevre</strong> :",nomJoueur[1]);

    //layout de la page
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;

    //bouton pour commencer à jouer une nouvelle partie
    QPushButton *appelDuJeu = new QPushButton ("Jouer une nouvelle partie");
    QObject::connect(appelDuJeu,SIGNAL(clicked()),this,SLOT(NouvellePartie()));

    layoutPrincipal->addLayout(nomsJoueurs);
    layoutPrincipal->addWidget(appelDuJeu);

    this->setLayout(layoutPrincipal);
}

PageNouvelleP::~PageNouvelleP()
{
    delete nomJoueur[0];
    delete nomJoueur[1];

    delete partie;
}

void PageNouvelleP::NouvellePartie()
{
    emit AppelPageJeu(nomJoueur[0]->text(),nomJoueur[1]->text());
}

