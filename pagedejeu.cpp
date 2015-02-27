#include <QLabel>
#include <QStringList>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QIcon>
#include "pagedejeu.h"
#include "modele.h"
#include "controleurs.h"

PageDeJeu::PageDeJeu(Partie **p_partie)
{
    //récupération de la partie
    partie = (*p_partie);

    //configuration du menu
    QMenu *menuPartie = menuBar()->addMenu("Partie");

    QAction *actionNouvelleP = new QAction("Nouvelle partie", this);
    actionNouvelleP->setIcon(QIcon("icon_nouvelleP.png"));
    menuPartie->addAction(actionNouvelleP);

    QAction *actionChargerP = new QAction("Charger une partie", this);
    actionChargerP->setIcon(QIcon("icon_charger.png"));
    menuPartie->addAction(actionChargerP);
    QObject::connect(actionChargerP,SIGNAL(triggered()),this,SLOT(ChargerNiveau()));

    QAction *actionQuitter = new QAction("Fermer", this);
    menuPartie->addAction(actionQuitter);
    QObject::connect(actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));

    //barre de sauvegarde
    QMenu *menuSauvegarde = menuBar()->addMenu("Sauvegarde");

    QAction *actionSauvegarder = new QAction("Sauver",this);
    actionSauvegarder->setIcon(QIcon("icon_save.png"));
    QObject::connect(actionSauvegarder,SIGNAL(triggered()),this,SLOT(SauvegarderNiveau()));

    actionSauvegardeAuto = new QAction("Auto Save",this);
    actionSauvegardeAuto->setCheckable(true);
    menuSauvegarde->addAction(actionSauvegarder);
    menuSauvegarde->addAction(actionSauvegardeAuto);

    //barre d'aide
    QMenu *menuAide = menuBar()->addMenu("Aide");

    QAction *actionAppelAide = new QAction("Aide",this);
    actionAppelAide->setIcon(QIcon("icon_Aide.png"));
    menuAide->addAction(actionAppelAide);

    QAction *actionAPropos = new QAction("A Propos",this);
    actionAPropos->setIcon(QIcon("icon_APropos.png"));
    menuAide->addAction(actionAPropos);

    //différents layouts de la page
    QHBoxLayout *layoutJeu =  new QHBoxLayout;  //affichage du jeu et des scores/noms
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;

    QVBoxLayout *l_Gauche = new QVBoxLayout;//layout principal à gauche
    QVBoxLayout *l_Droite = new QVBoxLayout;//layout principal à droite

    QHBoxLayout *l_Messages = new QHBoxLayout; //informations du jeu

    //récupération de tous les phrases
    QFile *f_SI = new QFile("messages.txt");
    QString *s_SI = new QString;

    if (f_SI->open(QFile::ReadOnly))
    {
        QTextStream in(f_SI);
        *s_SI = in.readAll();
    }
    f_SI->close();

    //séparation des différentes phrases dans un QStringList
    sl_SI = new QStringList;

    *sl_SI = s_SI->split("\n", QString::SkipEmptyParts);
    delete s_SI;

    nombreSI = sl_SI->size();

    //définition du Qlabel qui va recevoir les phrases
    l_SI = new QLabel(QString::fromUtf8("Bonne partie à vous !"));
    l_SI->setAlignment(Qt::AlignHCenter);

    //QLabel affichant le joueur en cours
    l_tour =  new QLabel;
    l_tour->setAlignment(Qt::AlignLeft);
    if(partie->GetJoueurEnCours() == R_CHEVRE)
        l_tour->setText(QString("Tour ") +  QString::number(partie->GetNbTours() + 1) + QString::fromUtf8(" : Chèvres"));
    else
        l_tour->setText(QString("Tour ") +  QString::number(partie->GetNbTours() + 1) + QString::fromUtf8(" : Tigres"));

    //QLabel affichant le nombre de chevres restantes
    l_nbChevres = new QLabel(QString::fromUtf8("Nb chèvres : ") + QString::number(partie->GetNbPionsRestants(1)));
    l_nbChevres->setAlignment(Qt::AlignRight);

     //définition de l'affichage pour le joueur Tigre
    l_nomJoueurTigre = new QLabel(partie->GetNomJoueur(0));
    l_scoreJoueurTigre = new QLabel(QString::number(partie->GetScore(0)));

    QFormLayout *fl_Tigre = new QFormLayout;
    fl_Tigre->addRow("Nom :",l_nomJoueurTigre);
    fl_Tigre->addRow("Score :",l_scoreJoueurTigre);

    QGroupBox *gb_Tigre = new QGroupBox("Tigres :");
    gb_Tigre->setLayout(fl_Tigre);

    QLabel *l_ImageTigre = new QLabel;
    l_ImageTigre->setPixmap(QPixmap("image_tigre.png"));

    //définition du à propos
    APropos = new QMessageBox();
    APropos->setText(QString::fromUtf8("Ce Programme a été réalisé par : \n\n    -> André Guillaume\n    -> Morinière Sébastien \n    -> Haffreingue Victor \n\n\n\n Utilisation de Qt 4.7.4 \n Copyright 2011-2012.\n Tous droits réservés."));
    APropos->setIconPixmap(QPixmap("icon_APropos.png"));
    APropos->setWindowModality(Qt::ApplicationModal);
    QObject::connect(actionAPropos,SIGNAL(triggered()),APropos,SLOT(show()));

    //définition de l'affichage pour le joueur Chevre
    l_nomJoueurChevre = new QLabel(partie->GetNomJoueur(1));
    l_scoreJoueurChevre = new QLabel(QString::number(partie->GetScore(1)));

    QFormLayout *fl_Chevre = new QFormLayout;
    fl_Chevre->addRow("Nom :",l_nomJoueurChevre);
    fl_Chevre->addRow("Score :",l_scoreJoueurChevre);

    QGroupBox *gb_Chevre = new QGroupBox(QString::fromUtf8("Chèvres :"));
    gb_Chevre->setLayout(fl_Chevre);

    QLabel *l_ImageChevre = new QLabel;
    l_ImageChevre->setPixmap(QPixmap("image_chevre.png"));

     //définition de la page d'aide
    pageAide = new PageAide;
    pageAide->setWindowModality(Qt::NonModal);
    QObject::connect(actionAppelAide,SIGNAL(triggered()),pageAide,SLOT(show()));

    //definition de la page pour une nouvelle partie
    pageNP = new PageNouvelleP(p_partie);
    pageNP->setWindowModality(Qt::ApplicationModal);
    QObject::connect(actionNouvelleP,SIGNAL(triggered()),pageNP,SLOT(show()));
    QObject::connect(pageNP,SIGNAL(AppelPageJeu(QString,QString)),this,SLOT(NouvellePartie(QString,QString)));

    //fichier de sauvegarde
    fichierSauvegarde = new QFile("sauv.txt");

    //définition du centre du jeu : la zone graphique
    zoneGraphique = new ZoneGraphique(p_partie);
    vueGraphique = new QGraphicsView(zoneGraphique);
    vueGraphique->setFixedSize(500,500);
    vueGraphique->setMouseTracking(true);//la sourie est suivie par tmp

    //quand la zone graphique est modifiée on va mettre à jour le tableau de référence
    QObject::connect(zoneGraphique,SIGNAL(RetourZoneGraphique(int)),this,SLOT(AppelMAJ(int)));

    //on sauvegarde quand on clique sur le bouton de sauvegarde automatique
    QObject::connect(actionSauvegardeAuto,SIGNAL(triggered()),this,SLOT(SauvegarderNiveau()));

    //widget principal contenu dans la page
    widgetCentral = new QWidget;

    l_Droite->addWidget(gb_Chevre);
    l_Droite->addWidget(l_ImageChevre);

    l_Gauche->addWidget(gb_Tigre);
    l_Gauche->addWidget(l_ImageTigre);


    layoutJeu->addLayout(l_Gauche);
    layoutJeu->addWidget(vueGraphique);
    layoutJeu->addLayout(l_Droite);

    l_Messages->addWidget(l_tour);
    l_Messages->addWidget(l_SI);
    l_Messages->addWidget(l_nbChevres);

    layoutPrincipal->addLayout(layoutJeu);
    layoutPrincipal->addLayout(l_Messages);

    widgetCentral->setLayout(layoutPrincipal);

    this->setCentralWidget(widgetCentral);
}

void PageDeJeu::AppelMAJ(int deplacementOk)
{   
    /*
      deplacementOk :
      0 : Deplacement non autorisé
      1 : Deplacement ok
      2 : Saut Ok
      3 : Saut Ok par dessus une tour
      4 : les tigres passent leur tour
      5 : chargement du plateau
      -1 : C'est aux chevres de jouer
      -2 : C'est aux tigres de jouer
      -3 : il faut deplacer le même tigre
      -4 : un second deplacement n'est possible qu'avec un saut

      TestDeplacementTigre(partie)
      0 : pas de deplacements possibles
      1 : deplacement possible
      2 : saut possible

      Set Tigre joue : definie si le tigre peut rejouer ou non :
      0 : ne peut pas rejouer
      1 : commence a jouer
      2 : peut rejouer

     */

    T_Role tmp = partie->GetJoueurEnCours();
    int tigreRejoue = partie->GetTigreJoue();
    bool afficheSI =  true;

    if(actionSauvegardeAuto->isChecked() && deplacementOk > 0) //enregistrement
        SauvegarderNiveau();

    if(deplacementOk == 1) //bon deplacement
    {
        //changement de joueur
        if(partie->GetJoueurEnCours() == R_TIGRE)
        {//le tigre ne peut plus jouer on change de joueur
            tmp = R_CHEVRE;
            tigreRejoue = 0;
        }
        else
        {//si le tigre commence à jouer
            tigreRejoue = 1;
            tmp = R_TIGRE;
        }
    }
    else if(deplacementOk == 2)
    {
        if(TestDeplacementTigre(partie) == 2)
        {//si le tigre peut rejouer
            tmp = R_TIGRE;
            tigreRejoue = 2;
        }
        else
        {//le tigre ne peut plus jouer on change de joueur
            tmp = R_CHEVRE;
            tigreRejoue = 0;
        }
    }
    else if(deplacementOk == 3)
    {//on change de joueur car le tigre doit sarreter de joueur
        tmp = R_CHEVRE;
        tigreRejoue = 0;
    }
    else if(deplacementOk == 4)
    {
        afficheSI = false;
        if(partie->GetJoueurEnCours() == R_TIGRE)
        {
            if(partie->GetTigreJoue() == 2)//il faut passer apres un saut
            {
                l_SI->setText(QString::fromUtf8("Les tigres passent leur tour."));
                tmp = R_CHEVRE;
                tigreRejoue = 0;
            }
            else
                l_SI->setText(QString::fromUtf8("Vous devez sauter avant de passer." ));
        }
        else
             l_SI->setText(QString::fromUtf8("Seuls les tigres peuvent passer leur tour."));
    }
    else if(deplacementOk == 5)
    {//on charge un plateau
        zoneGraphique->ModifierTableau();
    }
    else
    {
        afficheSI = false;
        switch (deplacementOk)
        {
            case 0:
                l_SI->setText(QString::fromUtf8("Déplacement non autorisé."));
                break;
            case -1:
                l_SI->setText(QString::fromUtf8("C'est aux chèvres de jouer."));
                break;
            case -2:
                l_SI->setText(QString::fromUtf8("C'est aux tigres de jouer."));
                break;
            case -3:
                l_SI->setText(QString::fromUtf8("Vous devez rejouer le même tigre."));
                break;
            case -4:
                l_SI->setText(QString::fromUtf8("Pour rejouer vous devez faire un saut."));
                break;
            default :
                break;
        }
    }

    if(afficheSI && deplacementOk != 5)
    {//si le tour se fait normalement et que ce n'est pas un chargement ou une nouvelle partie
        partie->SetNbTour(1);
        l_SI->setText(sl_SI->at(qrand()%nombreSI));
    }

    if(FinDePartie(partie))
    {
        int gagnant = 0;

        if(partie->GetNbPionsRestants(1) <= 6)//si c'est les tigres qui ont gagnés
            gagnant =  1;

        //definition de la page de fin
        pageFin = new FinPartie(gagnant);
        pageFin->setWindowModality(Qt::WindowModal);
        pageFin->show();

        int rejoue = QMessageBox::question(this, "Rejouer ?", "Voulez-vous rejouer ?", "Oui", "Non");

        pageFin->close();

        if(!rejoue)
        {
            pageNP->show();
            zoneGraphique->ModifierTableau();
        }
        else
            qApp->quit();
    }

    //mis à jour des joueurs
    partie->SetJoueurEnCours(tmp);
    partie->SetTigreJoue(tigreRejoue);

    if(deplacementOk == 2 ||deplacementOk == 3) //si il y a un saut de chevre
        partie->DecrementeNbChevre();

    //mise a jour des scores
    partie->SetScore(0,100 - partie->GetNbPionsRestants(1)*5);
    partie->SetScore(1,partie->GetNbPionsRestants(1)*5);

    l_scoreJoueurChevre->setText(QString::number(partie->GetScore(1)));
    l_scoreJoueurTigre->setText(QString::number(partie->GetScore(0)));

    //affichage du tour :
    if(partie->GetJoueurEnCours() == R_CHEVRE)
        l_tour->setText(QString("Tour ") +  QString::number(partie->GetNbTours() + 1) + QString::fromUtf8(" : Chèvres"));
    else
        l_tour->setText(QString("Tour ") +  QString::number(partie->GetNbTours() + 1) + QString::fromUtf8(" : Tigres"));

    //affichage du nombre de chevres
    l_nbChevres->setText(QString::fromUtf8("Nb chèvres : ") + QString::number(partie->GetNbPionsRestants(1)));
}

void PageDeJeu::SauvegarderNiveau()
{
    //écriture dans le fichier de sauvegarde
    if (fichierSauvegarde->open(QFile::WriteOnly))
    {
        QTextStream out(fichierSauvegarde);
        for(short i = 0; i < 5; i ++)
        {
            for(short j = 0; j < 5; j ++)
                out << QString::number(partie->GetCase(i,j) + 1) << ",";
        }
        out << partie->GetNomJoueur(0) + "," + partie->GetNomJoueur(1) +  ",";
        out << QString::number(partie->GetNbPionsRestants(1)) + "," + QString::number(partie->GetNbTours()) + ",";

        if(partie->GetJoueurEnCours() == R_TIGRE)
            out << "1";
        else
            out << "0";

        fichierSauvegarde->close();
    }
}

void PageDeJeu::NouvellePartie(QString nomJoueur0, QString nomJoueur1)
{
    pageNP->close();

    //changement du nom des joueurs
    partie->SetJoueurNom(0,nomJoueur0);
    partie->SetJoueurNom(1,nomJoueur1);

    partie->SetNbPionsRestants(1,20);

    partie->SetNbTour(0,0);

    l_nomJoueurTigre->setText(partie->GetNomJoueur(0));
    l_nomJoueurChevre->setText(partie->GetNomJoueur(1));

    actionSauvegardeAuto->setChecked(false);

    partie->SetTigreJoue(0);
    partie->SetJoueurEnCours(R_CHEVRE);

    //reinitialisation des pions
    Initialiser((partie->GetPlateau()));
    AppelMAJ(5);
}

void PageDeJeu::ChargerNiveau()
{
     QString *parametresDeJeu = new QString;

    //mettre tout dans charger partie
    if (fichierSauvegarde->open(QFile::ReadOnly))
    {
        QTextStream in(fichierSauvegarde);
        *parametresDeJeu = in.readLine();
        fichierSauvegarde->close();
    }

    partie->ChargerPartie(parametresDeJeu);

    l_nomJoueurTigre->setText(partie->GetNomJoueur(0));
    l_nomJoueurChevre->setText(partie->GetNomJoueur(1));

    AppelMAJ(5);
}

PageDeJeu::~PageDeJeu()
{
    delete fichierSauvegarde;

    delete sl_SI;
    delete l_SI;
    delete l_tour;
    delete l_nbChevres;

    delete pageAide;
    delete pageNP;
    delete pageFin;
    delete APropos;

    delete widgetCentral;

    delete vueGraphique;
    delete zoneGraphique;

    delete l_scoreJoueurChevre;
    delete l_scoreJoueurTigre;
    delete l_nomJoueurTigre;
    delete l_nomJoueurChevre;
}
