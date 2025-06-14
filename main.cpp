#include <QCoreApplication>
#include <QDebug>
#include "GameManager.h"
#include "Card.h" // Potrzebne do qRegisterMetaType

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Rejestracja typu, aby qDebug potrafił go obsłużyć
    qRegisterMetaType<Card::CardType>("Card::CardType");


    qDebug() << "====== URUCHAMIANIE APLIKACJI ======";

    GameManager game;
    game.setupNewGame(2);

    qDebug() << "\n====== APLIKACJA ZAKOŃCZYŁA DZIAŁANIE ======";

    return 0;
}
