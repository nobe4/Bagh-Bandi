#include "pageaide.h"
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QGroupBox>
#include <QPixmap>

PageAide::PageAide()
{
    this->setGeometry(0,0,0,0);
    this->setWindowTitle("Aide");
    this->setWindowIcon(QIcon("icon_aide.png"));
    this->setWindowModality(Qt::NonModal);

    QTabWidget *onglets = new QTabWidget(this);

    //définition des widgets de chaques pages
    QWidget *pageIntro = new QWidget;
    QWidget *pageDeplacements = new QWidget;
    QWidget *pageFin = new QWidget;

    //différents layout et groupBox
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;

    QVBoxLayout *layoutIntro = new QVBoxLayout;

    QVBoxLayout *layoutDeplacements = new QVBoxLayout;
    QHBoxLayout *layoutDeplacementsTigres = new QHBoxLayout;
    QHBoxLayout *layoutDeplacementsChevres = new QHBoxLayout;
    QGroupBox *gb_DeplacementTigres = new QGroupBox(QString("Tigres :"));
    QGroupBox *gb_DeplacementChevres = new QGroupBox(QString("Chevres :"));

    QVBoxLayout *layoutFinJeu = new QVBoxLayout;


    //
    // ++++     INTRODUCTION     ++++
    //
    //définition des zones de textes et des images => QLabel
    QLabel *l_IntroTexte = new QLabel(QString::fromUtf8("     Ce jeu se joue sur un plateau, et les pions sont posés sur les intersections des lignes de ce plateau. Il y a 2 pions noirs (les tigres) et 20 pions blancs (les chèvres) répartis en 4 piles de 5 pions au début de la partie."));
    l_IntroTexte->setWordWrap(true);
    QLabel *l_PlateauBase = new QLabel(this);
    l_PlateauBase->setPixmap(QPixmap("aide_plateau1.png"));
    l_PlateauBase->setAlignment(Qt::AlignHCenter);

    QLabel *l_LegendeImage = new QLabel(QString::fromUtf8("<EM>Positionnement initial des pièces et allure du plateau de jeu.<\EM>"));
    l_LegendeImage->setAlignment(Qt::AlignCenter);

    //mise en page de la page d'introduction
    layoutIntro->addWidget(l_IntroTexte);
    layoutIntro->addWidget(l_PlateauBase);
    layoutIntro->addWidget(l_LegendeImage);


    //
    // ++++     DEPLACEMENTS     ++++
    //
    //définition des zones de textes et des images => QLabel
    QLabel *l_TexteChevres = new QLabel(QString::fromUtf8("    >  Les chèvres commencent la partie; \n\n    >  Le joueur peut déplacer un pion chèvre isolé ou le pion du sommet d'une pile depuis une intersection vers une intersection adjacente libre; \n\n    >  Le joueur ne peut pas déplacer une pile entière; \n\n    >  Une intersection comportant une pile n'est pas considérée comme libre : une chèvrene peut pas revenir sur une pile."));
    l_TexteChevres->setWordWrap(true);
    l_TexteChevres->setMinimumHeight(210);
    layoutDeplacementsChevres->addWidget(l_TexteChevres);
    gb_DeplacementChevres->setLayout(layoutDeplacementsChevres);
    layoutDeplacements->addWidget(gb_DeplacementChevres);

    QLabel *l_TexteTigres = new QLabel(QString::fromUtf8("    >  Un tigre peut se déplacer comme une chèvre (aller d'une intersection à une intersection adjacente libre); \n\n    >  Un tigre peut éliminer une chèvre isolée en sautant par dessus en ligne droite (il arrive alors sur l'intersection libre située derrière la chèvre qu'il élimine). Dans ce cas, il peut enchaîner un autre saut pour éliminer une autre chèvre s'il le souhaite; \n\n    >  Un tigre peut sauter par dessus une pile, auquel cas il élimine la chèvre du dessus de la pile. Dans ce cas son tour s'arrête (il ne peut enchaîner d'autres sauts); \n\n    >  Le tigre n'est jamais obligé d'éliminer une chèvre ou d'enchaîner les sauts."));
    l_TexteTigres->setWordWrap(true);
    l_TexteTigres->setMinimumHeight(230);
    l_TexteTigres->setAlignment(Qt::AlignVCenter);

    layoutDeplacementsTigres->addWidget(l_TexteTigres);
    gb_DeplacementTigres->setLayout(layoutDeplacementsTigres);
    layoutDeplacements->addWidget(gb_DeplacementTigres);

    //
    // ++++     FIN DE PARTIE     ++++
    //
    QLabel *l_TexteFinPartie = new QLabel(QString::fromUtf8("    >  Les chèvres gagnent si elles arrivent à bloquer les tigres (plus de mouvements possibles).\n\n    >  Les tigres gagnent s'ils éliminent toutes les chèvres ou s'il n'y a plus assez de chèvres pour les bloquer."));
    l_TexteFinPartie->setWordWrap(true);
    l_TexteFinPartie->setAlignment(Qt::AlignVCenter);

    QLabel *l_PlateauFin = new QLabel;
    l_PlateauFin->setPixmap(QPixmap("aide_plateauFin.png"));
    l_PlateauFin->setAlignment(Qt::AlignHCenter);

    QLabel *l_LegendePlateauFin = new QLabel(QString::fromUtf8("<EM>Position finale gagnantes pour les chèvres avec le minimum de chèvres.<\EM>"));
    l_LegendePlateauFin->setAlignment(Qt::AlignCenter);

    layoutFinJeu->addWidget(l_TexteFinPartie);
    layoutFinJeu->addWidget(l_PlateauFin);
    layoutFinJeu->addWidget(l_LegendePlateauFin);

    //ajout des layouts dans les widgets
    pageIntro->setLayout(layoutIntro);
    pageDeplacements->setLayout(layoutDeplacements);
    pageFin->setLayout(layoutFinJeu);

    //ajout des widget dans le TabWidget
    onglets->addTab(pageIntro, "Introduction");
    onglets->addTab(pageDeplacements, QString::fromUtf8("Déplacements"));
    onglets->addTab(pageFin, "Fin de Partie");

    layoutPrincipal->addWidget(onglets);

    this->setLayout(layoutPrincipal);
}
PageAide::~PageAide()
{
}
