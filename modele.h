#ifndef MODELE_H
#define MODELE_H

#include <QString>

typedef enum Contenu
{
    TIGRE = -1,
    RIEN = 0,
    CHEVRE_1 = 1,
    CHEVRE_2 = 2,
    CHEVRE_3 = 3,
    CHEVRE_4 = 4,
    CHEVRE_5 = 5,
}T_Contenu;

typedef struct CasePlateau
{
    T_Contenu contenu;
    int deplacements[8];// 0 = deplacement impossible, 1 = deplacement possible
} T_CasePlateau;

typedef enum Role
{
    R_CHEVRE,
    R_TIGRE,
}T_Role;



class Joueur
{
public:
    Joueur();//constructeur par défaut
    Joueur(QString nomCharge);

    QString GetNom();
    void SetNom(QString nomCharge);

    int GetNbPionsRestants();
    void SetNbPionsRestants(int inc = 0, int valBase = 0);

    void SetScore(int scoreCharge);
    int GetScore();

    ~Joueur();

private:
    QString nom;
    T_Role role; //chevre ou tigre
    int score;
    int nbPionsRestants;
};

class Partie
{
public:
    Partie();

    //get et set sur les differents parametres du jeu
    void SetJoueurNom(int joueur, QString nom);
    QString GetNomJoueur(int joueur);

    void SetCase(int ligne, int colonne, T_Contenu val);
    int GetCase(int ligne, int colonne);

    int GetNbTours();
    void SetNbTour(int inc = 0, int valBase = -1);

    int GetDeplacement(int ligne, int colonne, int depl);

    void SetJoueurEnCours(T_Role joueurEnCoursCharge);
    T_Role GetJoueurEnCours();

    void SetPosTigreEC(int *position); //EC : En Cours
    int GetPosTigreEC(int coor);
    int GetPosTigre(int tigre, int coor);

    void SetTigreEnCours(int *position);

    void SetTigreJoue(int ok);
    int GetTigreJoue();

    int GetNbPionsRestants(int joueur);
    void SetNbPionsRestants(int joueur, int val);

    void DecrementeNbChevre();

    void SetScore(int joueur, int score);
    int GetScore(int joueur);

    T_CasePlateau **GetPlateau();

    void ChargerPartie(QString *parametres);
    ~Partie();

private:
    T_Role joueurEnCours;
    int tigreEnCours;       //0 ou 1 en fonction du dernier tigre deplace
    int **posTigres;        //position des deux tigres
    Joueur *joueurs;
    T_CasePlateau **plateau;
    int nbTours;
    int tigreRejoue;
};



#endif // MODELE_H
