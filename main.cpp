#include <QtGui/QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QMessageBox>
#include "pagedejeu.h"
#include "modele.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Partie *partie = new Partie();

    QSplashScreen splash(QPixmap("splash.png"));
    splash.show();

    sleep(1.5);
    splash.finish(NULL);

    PageDeJeu *pageJeu = new PageDeJeu(&partie);
    pageJeu->show();

    return app.exec();
}
//verifier le delete pour chaque appel
