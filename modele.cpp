#include "modele.h"
#include <QStringList>
#include "controleurs.h"

//
//  JOUEUR
//
Joueur::Joueur()
{

}

Joueur::Joueur(QString nomCharge)
{
    nom = nomCharge;
    nbPionsRestants = 5;

}

void Joueur::SetNom(QString nomCharge)
{
    nom = nomCharge;
}

int Joueur::GetNbPionsRestants()
{
    return nbPionsRestants;
}

void Joueur::SetScore(int scoreCharge)
{
    score = scoreCharge;
}

int Joueur::GetScore()
{
    return score;
}

void Joueur::SetNbPionsRestants(int inc, int valBase)
{
    if(inc == 0)
        nbPionsRestants = valBase;
    else
        nbPionsRestants -= inc;
}

QString Joueur::GetNom()
{
    return nom;
}

Joueur::~Joueur()
{
}

//
//  PARTIE
//
Partie::Partie()
{
    joueurEnCours = R_CHEVRE;

    joueurs = new Joueur[2];

    joueurs[0] = Joueur();
    joueurs[0].SetNbPionsRestants(0,2);
    joueurs[0].SetNom("Gilmour");
    joueurs[0].SetScore(0);

    joueurs[1] = Joueur();
    joueurs[1].SetNbPionsRestants(0,20);
    joueurs[1].SetNom("Young");
    joueurs[1].SetScore(100);

    //faire la création dans le constructeur
    plateau = new T_CasePlateau*[5];

    for(int lign = 0; lign < 5; lign++)
    {
        plateau[lign] = new T_CasePlateau[5];
    }

    posTigres =  new int*[2];

    for(int i = 0; i < 2; i ++)
    {
        posTigres[i] = new int[2];
        for(int j  = 0; j < 2; j ++)
            posTigres[i][j] = 0;
    }

    //position de tigre de base

    posTigres[0][0] = 1;
    posTigres[0][1] = 2;
    posTigres[1][0] = 3;
    posTigres[1][1] = 2;

    tigreEnCours = 0;

    tigreRejoue = 0;

    Initialiser(plateau);

    nbTours = 0;
}

void Partie::SetJoueurNom(int joueur, QString nom)
{
    joueurs[joueur].SetNom(nom);
}

void Partie::SetCase(int ligne, int colonne, T_Contenu val)
{
   plateau[ligne][colonne].contenu = val;
}

int Partie::GetNbTours()
{
    return nbTours;
}

void Partie::SetNbTour(int inc, int valBase)
{
    if(valBase == -1)
        nbTours += inc;
    else
        nbTours = valBase;
}

QString Partie::GetNomJoueur(int joueur)
{
    return joueurs[joueur].GetNom();
}

int Partie::GetCase(int ligne, int colonne)
{
    return plateau[ligne][colonne].contenu;
}

int Partie::GetDeplacement(int ligne, int colonne, int depl)
{//retourne 1 si le deplacement est possible 0 sinon
    return plateau[ligne][colonne].deplacements[depl];
}

void Partie::SetJoueurEnCours(T_Role joueurEnCoursCharge)
{
    joueurEnCours = joueurEnCoursCharge;
}

T_Role Partie::GetJoueurEnCours()
{
    return joueurEnCours;
}

void Partie::SetPosTigreEC(int *position)
{        
    for(int i = 0; i < 2; i ++)
        posTigres[tigreEnCours][i] =  position[i];
}

int Partie::GetPosTigreEC(int coor)
{
    return posTigres[tigreEnCours][coor];
}

int Partie::GetPosTigre(int tigre, int coor)
{
    return posTigres[tigre][coor];
}

void Partie::SetTigreEnCours(int *position)
{
     if(position[0] == posTigres[0][0] && position[1]==posTigres[0][1])
        tigreEnCours = 0;
    else
        tigreEnCours = 1;
}

int Partie::GetNbPionsRestants(int joueur)
{
    return joueurs[joueur].GetNbPionsRestants();
}

void Partie::SetNbPionsRestants(int joueur, int val)
{
    joueurs[joueur].SetNbPionsRestants(0,val);
}

void Partie::DecrementeNbChevre()
{
    joueurs[1].SetNbPionsRestants(1);
}

void Partie::SetScore(int joueur, int score)
{
    joueurs[joueur].SetScore(score);
}

int Partie::GetScore(int joueur)
{
    return joueurs[joueur].GetScore();
}

T_CasePlateau **Partie::GetPlateau()
{
    return plateau;
}

void Partie::SetTigreJoue(int ok)
{
    tigreRejoue = ok;
}

int Partie::GetTigreJoue()
{
    return tigreRejoue;
}

void Partie::ChargerPartie(QString *parametres)
{
    QStringList *listeParametres = new QStringList(parametres->split(","));
    int nbTigres = 0;

    for(int i = 0; i < 25; i ++)
    {
        SetCase(i/5,i%5,ToContenu(listeParametres->at(i).toInt() - 1));
        /*pour éviter d'avoir à gérer le cas des nombres négatifs dans l'enregistrement du tableau on met chaque case à +1
          et on refait un -1 en le mettant dans le tableau*/

        //mise à jour des positions de tigres
        if(plateau[i/5][i%5].contenu == TIGRE)
        {
            posTigres[nbTigres][0] = i/5;
            posTigres[nbTigres][1] = i%5;
            nbTigres++;
        }
    }

    joueurs[0].SetNom(listeParametres->at(25));
    joueurs[1].SetNom(listeParametres->at(26));

    joueurs[1].SetNbPionsRestants(0,listeParametres->at(27).toInt());

    joueurs[0].SetScore(100 - joueurs[1].GetNbPionsRestants()*5);
    joueurs[1].SetScore(joueurs[1].GetNbPionsRestants()*5);

    nbTours = listeParametres->at(28).toInt();

    if(listeParametres->at(29) == "1")
    {
        tigreRejoue = 1;
        SetJoueurEnCours(R_TIGRE);
    }
    else
    {
        tigreRejoue = 0;
        SetJoueurEnCours(R_CHEVRE);
    }

    delete listeParametres;
}

Partie::~Partie()
{
    for(int i = 0; i < 5; i ++)
        delete [] plateau[i];

    delete [] plateau;

    for(int i  = 0; i < 2; i ++)
        delete [] posTigres[i];

    delete [] posTigres;
}
