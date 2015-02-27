#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include "finpartie.h"

FinPartie::FinPartie(int gagnant)
{
    /*
      0 : les tigres gagnent
      1 : les chevres gagnent
      */

    this->setGeometry(0,0,0,0);
    this->setWindowTitle("Fin de partie");
    this->setWindowModality(Qt::ApplicationModal);

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *label = new QLabel;

    if(gagnant == 1)
        label->setPixmap(QPixmap("fin_Tigres.png"));
    else
        label->setPixmap(QPixmap("fin_Chevres.png"));

    layout->addWidget(label);
    this->setLayout(layout);
}

FinPartie::~FinPartie()
{
}
