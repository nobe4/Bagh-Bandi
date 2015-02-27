    #include "controleurs.h"

int Vabs(int val)
{
    if(val < 0)
        return -val;
    else
        return val;
}

int DeplacementOk(Partie *partie, int *positionBase, int *positionFinale)
{
    //  0 : Déplacement pas possible
    //  1 : Déplacement possible
    //  2 : Saut d'une chevre
    //  3 : Saut d'une tour de chevres
    // -1 : Pas de déplacement

    int out = 0;


    if((positionBase[0] == positionFinale[0]) && (positionBase[1] == positionFinale[1])) //pas de deplacement
    {
        out = -1;
    }
    else
    {
        if(ToContenu(partie->GetCase(positionFinale[0],positionFinale[1])) != RIEN)
        {
            //si la case n'est pas vide ou si pas de déplacement
            out = 0;
        }
        else if(Vabs(positionBase[0] - positionFinale[0]) < 2 && Vabs(positionBase[1] - positionFinale[1]) < 2)
        {//deplacement d'une case
            //on retourne 1 si le déplacement depuis la case de base ver la direction recherchee est bonne
            out = partie->GetDeplacement(positionBase[0],positionBase[1],RechercheDeplacement(positionBase, positionFinale));
        }
        else if(Vabs(positionBase[0] - positionFinale[0]) < 3 && Vabs(positionBase[1] - positionFinale[1]) < 3)
        {//saut au dessus d'un pion
            if(partie->GetJoueurEnCours() == R_TIGRE)
                out = SautOK(partie,positionBase,positionFinale);
            else
                out = 0; //si c'est une chevre qui saute
        }
        else //si le saut est plus long que 3 cases
        {
            out = 0;
        }
    }

    //si deplacement est bon et que le joueur en cours est un tigre
    if(out > 0 && partie->GetJoueurEnCours() == R_TIGRE)
    {//maj de la pos du tigre en cours
        partie->SetTigreEnCours(positionBase);
        partie->SetPosTigreEC(positionFinale);
    }

    return out;
}

int RechercheDeplacement(int *positionBase, int *positionFinale) // Avec [0] = x = ligne et [1] = y = colonne
{
    int out;

    if((positionFinale[0] - positionBase[0]) == -1)          // Vers le haut
    {
        if((positionFinale[1] - positionBase[1]) == 0)       // Deplacement VERTICAL vers le HAUT
        {
            out = 0;
        }
        else if((positionFinale[1] - positionBase[1]) == 1)  // Deplacement HAUT Diagonale DROITE
        {
            out = 1;
        }
        else                                                 // Deplacement HAUT Diagonale GAUCHE
        {
            out = 7;
        }
    }
    else if((positionFinale[0] - positionBase[0]) == 1)      // Vers le bas
    {
        if((positionFinale[1] - positionBase[1]) == 0)       // Deplacement VERTICAL vers le BAS
        {
            out = 4;
        }
        else if((positionFinale[1] - positionBase[1]) == 1)  // Deplacement BAS Diagonale DROITE
        {
            out = 3;
        }
        else                                                 // Deplacement BAS Diagonale GAUCHE
        {
            out = 5;
        }
    }
    else                                                    // Deplacement horizontal
    {
        if((positionFinale[1] - positionBase[1]) == 1)      // Deplacement DROITE
        {
            out = 2;
        }
        else                                                // Deplacement GAUCHE
        {
            out = 6;
        }
    }
    return out;
}

int SautOK(Partie *partie, int *positionBase, int *positionFinale)
{
    // 0 : Pas possible
    // 2 : bon par dessus 1 chevre
    // 3 : bon par dessus une tour de chevres

    int ok = 0;
    //on verifie que le saut est bien de deux cases
    if(((positionBase[0] + positionFinale[0]) % 2 != 0) || (((positionBase[1] + positionFinale[1]) % 2 != 0)))
    {//pas un trajet suivant les lignes
        //QMessageBox::information(NULL,"","pas droit");
        ok = 0;
    }
    else
    {
        //on sauvegarde le pion central
        int positionMid[2] = {(positionBase[0] + positionFinale[0]) / 2.0, (positionBase[1] + positionFinale[1]) / 2.0};
        //on verifie si le pre-saut est possible car l'autre suivra et pas de selection hors du plateau
        int direction = RechercheDeplacement(positionBase,positionMid);

        if(partie->GetDeplacement(positionBase[0],positionBase[1],direction) == 1)//vérifier le deplacement
        {
            T_Contenu conTmp = ToContenu(partie->GetCase(positionMid[0],positionMid[1]));

            if(conTmp == CHEVRE_1)
            {
               ok = 2;
            }
            else if(conTmp > CHEVRE_1)
                ok = 3;
            else
                ok = 0;
        }
    }
    return ok;
}

bool FinDePartie(Partie *partie)
{
   /*
     0 : pas finie
     1 : fin de partie
     */
    bool result = true;

    if(partie->GetJoueurEnCours() == R_TIGRE)//Fin du tour du tigre
    {
        if(partie->GetNbPionsRestants(1) <= 6)
        {
             // Si il n'y a plus assez de chevres : fin de partie
        }
        else
        {
            result = false;
        }
    }
    else // Fin du tour des chèvres
    {
        if(TestDeplacementTigre(partie, 0) + TestDeplacementTigre(partie,1) > 0)
            result = false;
    }
    return result;
}

int TestDeplacementTigre(Partie *partie, int tigre)
{
    // 0 : pas de déplacement possible
    // 1 : deplacement possible
    // 2 : saut possible

    //renvoie true si deplacement ou un saut est encore possible
    int result = 0;
    int j = 0;
    int posBase[2] = {};


    if (tigre == -1)//on prend le tigre en cours
    {
        posBase[0] = partie->GetPosTigreEC(0);
        posBase[1] = partie->GetPosTigreEC(1);
    }
    else //on prend le tigre voulu
    {
        posBase[0] = partie->GetPosTigre(tigre,0);
        posBase[1] = partie->GetPosTigre(tigre,1);
    }

    int posMid[2];
    int posFinal[2];

    int ligInc[8] = {-1,-1,0,1,1,1,0,-1};
    int colInc[8] = {0,1,1,1,0,-1,-1,-1};

    do
    {
        if(partie->GetDeplacement(posBase[0],posBase[1],j) == 1)//si le deplacement est bon
        {
            //on met les positions de la case centrale
            posMid[0] = posBase[0] + ligInc[j];
            posMid[1] = posBase[1] + colInc[j];

           if(posMid[0] > -1 && posMid[1] > -1 && posMid[0] < 5 && posMid[1] < 5)//vérification que l'on ne sorte pas du cadre
            {
               if(ToContenu(partie->GetCase(posMid[0],posMid[1])) == RIEN)
                {
                    //encore un déplacement possible
                    result = 1;
                }
                else
                {
                   //on met la position finale dans posFinal
                    posFinal[0] = posBase[0] + 2*ligInc[j];
                    posFinal[1] = posBase[1] + 2*colInc[j];


                    if(posFinal[0] > -1 && posFinal[1] > -1 && posFinal[0] < 5 && posFinal[1] < 5)//vérification que l'on ne sorte pas du cadre
                    {
                        if((ToContenu(partie->GetCase(posMid[0],posMid[1])) > RIEN) && (ToContenu(partie->GetCase(posFinal[0],posFinal[1])) == RIEN))
                        {
                            result = 2;
                            //encore un saut possible
                        }
                    }
                }
            }
        }

        j++;
    }while (j < 8 && ((result == 0 && tigre != -1) || (result < 2 && tigre == -1)));

    /*
A : j < 8
B : res != 0
C : res != 2
D : test Tigre EC

Si  A et ((B et !D) ou (C et D))
     */

 return result;
}

// INITIALISATION
//
void Initialiser(T_CasePlateau **plateau)
{
    int croix = 0;
    int noeudEtoile[8] = {1,1,1,1,1,1,1,1};
    int noeudCroix[8] = {1,0,1,0,1,0,1,0};
    bool etoile = false;
    int tmp = 1;

    for(int lign = 0; lign < 5; lign++)
    {
        for (int col = 0; col < 5; col++)
        {
            plateau[lign][col].contenu = RIEN;
            etoile = false;


            if (tmp == 1) // Normalement XOR
            {
                for(int i = 0; i < 8; i++)
                {
                    plateau[lign][col].deplacements[i] = noeudEtoile[i];
                }
                etoile = true;
            }
            else
            {
                for(int i = 0; i < 8; i++)
                {
                    plateau[lign][col].deplacements[i] = noeudCroix[i];
                }
                croix++;
            }

            tmp *= -1;

            Init_characters(&(plateau[lign][col].contenu), lign, col, croix, etoile);
            TestPositionCase(plateau[lign][col].deplacements, lign, col);
        }
    }
}

//
// TEST POSITION
//
void TestPositionCase(int *deplacements, int lign, int col)
{
    if (lign - 1 < 0)                       // Boucle a voir, ou méthode plus propre, rapide --> PAS TRES BEAU !!!
    {
        deplacements[0] = 0;
        deplacements[1] = 0;
        deplacements[7] = 0;
    }
    if(lign + 1 >= 5)
    {
        deplacements[3] = 0;
        deplacements[4] = 0;
        deplacements[5] = 0;
    }
    if(col - 1 < 0)
    {
        deplacements[5] = 0;
        deplacements[6] = 0;
        deplacements[7] = 0;
    }

    if(col + 1 >= 5)
    {
        deplacements[1] = 0;
        deplacements[2] = 0;
        deplacements[3] = 0;
    }
}

//
// INITIALISATION CHARACTERES
//
void Init_characters(T_Contenu *p_contenu, int lign, int col, int croix, bool etoile) // Conditions à revoir, peuvent être mieux.
{
    if(lign %2 == 0 && croix < 8 && etoile == false)
    {
        *p_contenu = CHEVRE_5;
    }
    else if (col == 2 && lign % 2 == 1)
    {
        *p_contenu = TIGRE;
    }
    else
    {
        *p_contenu = RIEN;
    }
}

//fait la conversion entre un int et un T_Contenu
T_Contenu ToContenu(int val)
{
    T_Contenu retour = TIGRE;

    switch (val)
    {
        case 0:
            retour = RIEN;
            break;
        case 1:
            retour = CHEVRE_1;
            break;
        case 2:
            retour = CHEVRE_2;
            break;
        case 3:
            retour = CHEVRE_3;
            break;
        case 4:
            retour = CHEVRE_4;
            break;
        case 5:
            retour = CHEVRE_5;
            break;
        case -1:
            retour = TIGRE;
            break;
    }
    return retour;
}

int ToInt(T_Contenu val)
{
    int retour = 0;

    switch (val)
    {
        case RIEN:
            retour = 0;
            break;
        case CHEVRE_1:
            retour = 1;
            break;
        case CHEVRE_2:
            retour = 2;
            break;
        case CHEVRE_3:
            retour = 3;
            break;
        case CHEVRE_4:
            retour = 4;
            break;
        case CHEVRE_5:
            retour = 5;
            break;
        case TIGRE:
            retour = -1;
            break;
    }
    return retour;
}

