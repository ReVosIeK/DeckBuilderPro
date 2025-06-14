#include <QCoreApplication>
#include <QDebug>
#include "GameManager.h"
#include "Card.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qRegisterMetaType<Card::CardType>("Card::CardType");

    qDebug() << "====== URUCHAMIANIE APLIKACJI ======";

    GameManager game;
    game.setupNewGame(2); // Tworzymy grę dla 2 graczy

    qDebug() << "\n====== APLIKACJA ZAKOŃCZYŁA DZIAŁANIE ======";

    return 0;
}
