#ifndef CONTROLEURS_H
#define CONTROLEURS_H

#include "modele.h"

int Vabs(int val);
int DeplacementOk(Partie *partie, int *positionBase, int *positionFinale); //vérifie si le déplacement est possible
int RechercheDeplacement(int *positionIn, int *positionFin);
int SautOK(Partie *partie, int *positionBase, int *positionFinale);
bool FinDePartie(Partie *partie);
int TestDeplacementTigre(Partie *partie, int tigre = -1);

//convertisseurs
T_Contenu ToContenu(int val);
int ToInt(T_Contenu val);

//Initialisation du plateau
void Initialiser(T_CasePlateau **plateau);
void TestPositionCase(int *p_deplacements, int lign, int col);
void Init_characters(T_Contenu *p_contenu, int lign, int col, int croix, bool etoile);

#endif // CONTROLEURS_H
