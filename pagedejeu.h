#ifndef PAGEDEJEU_H
#define PAGEDEJEU_H

#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QFile>
#include <QCheckBox>
#include <QLabel>
#include <QStringList>
#include <QMessageBox>
#include "zonegraphique.h"
#include "modele.h"
#include "pageaide.h"
#include "pagenouvellepartie.h"
#include "finpartie.h"


class PageNouvelleP;

class PageDeJeu : public QMainWindow
{
    Q_OBJECT
public:
    PageDeJeu(Partie **p_partie);
    ~PageDeJeu();

public slots:
    void SauvegarderNiveau();
    void NouvellePartie(QString nomJoueur0, QString nomJoueur1);
    void ChargerNiveau();
    void AppelMAJ(int deplacementOk);

private:
    QMessageBox *APropos;
    PageAide *pageAide;
    PageNouvelleP *pageNP;
    FinPartie *pageFin;
    Partie *partie;
    QWidget *widgetCentral;
    QStringList *sl_SI; //stringlist du savoir inutile
    QLabel *l_SI;
    QLabel *l_tour;
    QLabel *l_nbChevres;
    QLabel *l_nomJoueurTigre;
    QLabel *l_nomJoueurChevre;
    QAction *actionSauvegardeAuto;
    int nombreSI;
    QGraphicsView *vueGraphique;
    QFile *fichierSauvegarde;
    ZoneGraphique *zoneGraphique;
    QLabel *l_scoreJoueurChevre;
    QLabel *l_scoreJoueurTigre;

};

#endif // PAGEDEJEU_H
