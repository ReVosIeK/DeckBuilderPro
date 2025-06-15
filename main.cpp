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
    game.setupNewGame(2);

    // Symulacja fazy zagrywania kart
    game.playFullTurnForCurrentPlayer_Test();

    // Symulacja fazy zakupów
    qDebug() << "\n====== FAZA ZAKUPÓW ======";
    game.buyCardFromLineUp(0);
    game.buyKick();

    // Próba pokonania Super-Złoczyńcy
    game.buySuperVillain();

    // Symulacja końca tury
    if(game.currentPlayer()) {
        game.endTurnActions();
    }

    qDebug() << "\n====== APLIKACJA ZAKOŃCZYŁA DZIAŁANIE ======";

    return 0;
}
