#include "zonegraphique.h"
#include "controleurs.h"
#include <QMessageBox>
#include <QString>

ZoneGraphique::ZoneGraphique(Partie **p_partie)
{
    //ajout de l'image de fond
    this->addPixmap(QPixmap("plateau.png"));

    //initialisation des images dans des objets:
    p_vide = new QPixmap("vide.png");
    p_tigre = new QPixmap("tigre.png");
    p_chevre = new QPixmap("chevre.png");
    p_chevre2 = new QPixmap("chevre2.png");
    p_chevre3 = new QPixmap("chevre3.png");
    p_chevre4 = new QPixmap("chevre4.png");
    p_chevre5 = new QPixmap("chevre5.png");

    //initialisation du tableau affiché
    tableauPions = new QGraphicsPixmapItem**[5];

    for(int i = 0; i < 5; i++)
    {
        tableauPions[i] = new QGraphicsPixmapItem*[5];
        for(int j = 0; j < 5; j ++)
        {
            tableauPions[i][j] = new QGraphicsPixmapItem;
            tableauPions[i][j]->setPixmap(*p_vide);
            tableauPions[i][j]->setPos(10 + 92*j,10 + 92*i);
            this->addItem(tableauPions[i][j]);
        }
    }

    //initialisation du tableau de gestion de l'affichage
    partie = (*p_partie);

    //objet qui servira à modifier le tableau et sa position de base
    tmp =  new QGraphicsPixmapItem;
    tmp->setPos(10,10);

    positionTmpBase = new int[2];
    typePionTmp = RIEN;

    ModifierTableau();

    this->addItem(tmp);
}

void ZoneGraphique::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos(mouseEvent->lastScenePos());

    //vérification de la position du curseur
    RecentrePos(&pos);

    tmp->setPos(pos - QPointF(50,50));

    int position[2];
    T_Contenu typePion;
    typePion = TrouverPionCase(&pos,position);

    int emi = 0;

    /*
      RETOUR ZONEGRAPHIQUE
      -1 : C'est aux chevres de jouer
      -2 : C'est aux tigres de jouer
      -3 : mauvais tigre choisi (2e deplacement)
      -4 : un second deplacement n'est possible qu'avec un saut

      GET TIGREJOUE
      0 : pas au tigre de jouer
      1 : tour normal
      2 : tigre rejoue
     */

    if(typePion == TIGRE)
    {
        if(partie->GetJoueurEnCours() == R_TIGRE)
        {
            if((partie->GetTigreJoue() == 2 && positionTmpBase[0] == position[0] && positionTmpBase[1] == position[1]) || partie->GetTigreJoue() == 1)
            {//si le pion choisis pour le nouveau deplacement est le même ou que le tigre commence a jouer
                tmp->setPixmap(*p_tigre);
                typePionTmp = TIGRE;
                partie->SetCase(position[0],position[1],ToContenu((partie->GetCase(position[0],position[1]) + 1)));
            }
            else
            {
                emi = -3;
            }
        }
        else
        {
            emi = -1;
        }
    }
    else if(typePion >= CHEVRE_1)
    {
        if(partie->GetJoueurEnCours() == R_CHEVRE)
        {
            tmp->setPixmap(*p_chevre);
            partie->SetCase(position[0],position[1],ToContenu((partie->GetCase(position[0],position[1]) - 1)));
            typePionTmp = CHEVRE_1;
        }
        else
        {
            emi = -2;
        }
    }

    if(emi == 0)//si le pion choisis est "bon"
    {
       positionTmpBase[0] = position[0];
       positionTmpBase[1] = position[1];
    }
    else
        emit RetourZoneGraphique(emi);

    ModifierTableau();
}

void ZoneGraphique::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    QPointF pos(mouseEvent->lastScenePos());

    //vérification de la position du curseur et deplacement de l'image
    RecentrePos(&pos);

    tmp->setPos(pos - QPointF(50,50));
}

void ZoneGraphique::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{


    /*
      retourZoneGraphique
      0 : Deplacement non autorisé
      1 : Deplacement ok
      2 : Saut Ok
      3 : les tigres passent leur tour
      -1 : C'est aux chevres de jouer
      -2 : C'est aux tigres de jouer
      -3 : il faut deplacer le même tigre
      -4 : un second deplacement n'est possible qu'avec un saut
      */

    bool reinit = true; //si on change les cases du dernier pion
    QPointF pos(mouseEvent->lastScenePos());

    //vérification de la positionFinale du curseur et deplacement de l'image
    RecentrePos(&pos);

    int positionFinale[2];
    T_Contenu typePion;

    typePion = TrouverPionCase(&pos,positionFinale);

    int deplOk;

    if(typePionTmp != RIEN) //si aucun pion n'est selectionne
        deplOk = DeplacementOk(partie,positionTmpBase,positionFinale);
    else
        deplOk = -1;

    if(deplOk == 1)//déplacement simple
    {
        if(partie->GetTigreJoue() != 2)
        {
            partie->SetCase(positionFinale[0],positionFinale[1],ToContenu(partie->GetCase(positionFinale[0],positionFinale[1]) + typePionTmp));
            emit RetourZoneGraphique(1);
        }
        else
        {//si on doit faire un saut pour rejouer
            //reinitialisation de la position de base
            partie->SetCase(positionTmpBase[0],positionTmpBase[1],ToContenu((partie->GetCase(positionTmpBase[0],positionTmpBase[1])) + typePionTmp));
            reinit = false;
            emit RetourZoneGraphique(-4);
        }
    }
    else if(deplOk == 2 || deplOk == 3)//saut
    {
        //decremente la case centrale
        partie->SetCase((positionTmpBase[0] + positionFinale[0])/2,(positionTmpBase[1] + positionFinale[1])/2,ToContenu((partie->GetCase((positionTmpBase[0] + positionFinale[0])/2,(positionTmpBase[1] + positionFinale[1])/2)) - 1));
        partie->SetCase(positionFinale[0],positionFinale[1],ToContenu((partie->GetCase(positionFinale[0],positionFinale[1])) + typePionTmp));
        emit RetourZoneGraphique(deplOk);
    }
    else if(deplOk == -1)//pas de déplacement
    {
        reinit = false;
        partie->SetCase(positionFinale[0],positionFinale[1],ToContenu((partie->GetCase(positionFinale[0],positionFinale[1])) + typePionTmp));
    }
    else//mauvais déplacement
    {
        partie->SetCase(positionTmpBase[0],positionTmpBase[1],ToContenu((partie->GetCase(positionTmpBase[0],positionTmpBase[1])) + typePionTmp));
        reinit = false;
        emit RetourZoneGraphique(0);
    }

    //reinitialisation du tmp si il y a eu un deplacement
    if(reinit)
    {
      positionTmpBase[0] = positionFinale[0];
      positionTmpBase[1] = positionFinale[1];
    }

    typePionTmp = RIEN;

    tmp->setPixmap(*p_vide);

    ModifierTableau();
}

void ZoneGraphique::keyPressEvent(QKeyEvent *keyEvent)
{
    if(keyEvent->key() == Qt::Key_Space)
        emit RetourZoneGraphique(4);
}

T_Contenu ZoneGraphique::TrouverPionCase(QPointF *pointPosition, int *p_position)
{
    int *position = p_position;
    T_Contenu typePion;

    position[0] = (pointPosition->y()-10)/92;
    position[1] = (pointPosition->x()-10)/92;

    typePion = ToContenu(partie->GetCase(position[0],position[1]));

    return typePion;
}

void ZoneGraphique::RecentrePos(QPointF *pos)
{
    if(pos->x() > 430)
        pos->setX(430);
    else if(pos->x() < 60)
        pos->setX(60);

    if(pos->y() > 430)
        pos->setY(430);
    else if(pos->y() < 60)
        pos->setY(60);
}

void ZoneGraphique::ModifierTableau()
{
    for(int i = 0; i < 5; i ++)
    {
        for(int j = 0; j < 5; j ++)
        {
            switch (partie->GetCase(i,j))
            {
                case RIEN:
                    tableauPions[i][j]->setPixmap(*p_vide);
                    break;
                case CHEVRE_1:
                    tableauPions[i][j]->setPixmap(*p_chevre);
                    break;
                case CHEVRE_2:
                    tableauPions[i][j]->setPixmap(*p_chevre2);
                    break;
                case CHEVRE_3:
                    tableauPions[i][j]->setPixmap(*p_chevre3);
                    break;
                case CHEVRE_4:
                    tableauPions[i][j]->setPixmap(*p_chevre4);
                    break;
                case CHEVRE_5:
                    tableauPions[i][j]->setPixmap(*p_chevre5);
                    break;
                default:
                    tableauPions[i][j]->setPixmap(*p_tigre);
                    break;

            }
        }
    }
}


ZoneGraphique::~ZoneGraphique()
{
    for(short i = 0; i < 5; i++)
    {
       for(short j = 0; j < 5; j ++)
            delete tableauPions[i][j];

        delete [] tableauPions[i];
    }

    delete [] tableauPions;

    delete p_chevre;
    delete p_chevre2;
    delete p_chevre3;
    delete p_chevre4;
    delete p_chevre5;
    delete p_vide;
    delete p_tigre;

    delete vue;
    delete tmp;

    delete positionTmpBase;
}
