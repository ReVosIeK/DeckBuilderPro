#include <QCoreApplication>
#include <QDebug>
#include "GameManager.h" // <<< ZMIANA

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "====== URUCHAMIANIE APLIKACJI ======";

    GameManager game;
    game.setupNewGame(2); // Uruchamiamy setup dla 2 graczy

    qDebug() << "\n====== APLIKACJA ZAKOŃCZYŁA DZIAŁANIE ======";

    return 0;
}
