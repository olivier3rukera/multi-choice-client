#include "display.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Display winMain;
    QFile fichier("D:/Projets/MultiChoice/MultiChoiceClient/MultiChoiceClient/styleClient.css");
    fichier.open(QIODevice::ReadOnly);
    QString style(fichier.readAll());
    winMain.setStyleSheet(style);
    fichier.close();
    winMain.show();
    return app.exec();
}
