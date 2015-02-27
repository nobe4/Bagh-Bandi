#ifndef PAGENOUVELLEP_H
#define PAGENOUVELLEP_H

#include <QWidget>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include "pagedejeu.h"
#include "modele.h"

class PageNouvelleP : public QWidget
{
    Q_OBJECT
public:
    PageNouvelleP(Partie **p_partie);
    ~PageNouvelleP();

signals:
    void AppelPageJeu(QString nomJoueurs0, QString nomJoueurs1);

public slots:
    void NouvellePartie();
    
private:
    Partie *partie;
    QLineEdit **nomJoueur;
};

#endif // PAGENOUVELLEP_H
