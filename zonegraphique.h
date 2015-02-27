#ifndef ZONEGRAPHIQUE_H
#define ZONEGRAPHIQUE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QPointF>
#include <QKeyEvent>
#include "modele.h"

class ZoneGraphique : public QGraphicsScene
{
    Q_OBJECT
public:
    ZoneGraphique(Partie **p_partie);
    ~ZoneGraphique();
    void ModifierTableau();
    T_Contenu TrouverPionCase(QPointF *pointPosition, int *p_position); //cherche le type de pion qui est sur la case
    void RecentrePos(QPointF *pos);

signals:
    void RetourZoneGraphique(int retour);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);

private:
    QGraphicsView *vue;
    QGraphicsPixmapItem *tmp;
    QGraphicsPixmapItem ***tableauPions;
    QPixmap *p_vide;
    QPixmap *p_chevre;
    QPixmap *p_chevre2;
    QPixmap *p_chevre3;
    QPixmap *p_chevre4;
    QPixmap *p_chevre5;
    QPixmap *p_tigre;
    Partie *partie;
    int *positionTmpBase;
    T_Contenu typePionTmp;
};

#endif // ZONEGRAPHIQUE_H
