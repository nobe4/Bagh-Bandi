#ifndef FINPARTIE_H
#define FINPARTIE_H

#include <QDialog>

class FinPartie : public QDialog
{
    Q_OBJECT
public:
    FinPartie(int gagnant);
    ~FinPartie();
    
};

#endif // FINPARTIE_H
