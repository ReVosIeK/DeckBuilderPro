#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include "GameManager.h"
#include "Card.h"
#include "Player.h"

#include <iostream>
#include <string>
#include <locale>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#endif

void displayGameState(GameManager* game) {
    Player* player = game->currentPlayer();
    if (!player) return;

    qDebug().noquote() << "\n========================================================";
    qDebug().noquote() << QString("TURA GRACZA: %1 (%2)").arg(game->players().indexOf(player) + 1).arg(player->heroId());
    qDebug().noquote() << QString("MOC: %1").arg(player->currentPower());
    qDebug().noquote() << "------------------- RĘKA -------------------";

    if (player->hand().isEmpty()) {
        qDebug().noquote() << "(Pusta)";
    } else {
        for(int i = 0; i < player->hand().size(); ++i) {
            qDebug().noquote() << QString("  [%1] %2").arg(i).arg(player->hand()[i]->name("pl"));
        }
    }

    qDebug().noquote() << "------------------ LINE-UP -----------------";
    for(int i = 0; i < game->lineUp().size(); ++i) {
        if(game->lineUp()[i]) {
            qDebug().noquote() << QString("  [%1] %2 (Koszt: %3)").arg(i).arg(game->lineUp()[i]->name("pl")).arg(game->lineUp()[i]->cost());
        }
    }
    qDebug().noquote() << "------------------ AKCJE -------------------";
    qDebug().noquote() << "Dostępne komendy: play [nr], buy [nr], kick, sv, end, quit";
    qDebug().noquote() << "========================================================";
}

int main(int argc, char *argv[])
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    setlocale(LC_ALL, ".UTF-8");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    QCoreApplication app(argc, argv);
    qRegisterMetaType<Card::CardType>("Card::CardType");

    qDebug() << "====== WITAJ W DECK BUILDER PRO (wersja konsolowa) ======";

    GameManager game;
    game.setupNewGame(2);

    while(true) {
        displayGameState(&game);

        std::cout << "\nTwoja komenda: " << std::flush;
        std::string std_line;
        if (!std::getline(std::cin, std_line)) {
            break;
        }
        QString line = QString::fromStdString(std_line);

        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.isEmpty()) continue;

        QString command = parts[0].toLower();

        bool ok;
        int index = (parts.size() > 1) ? parts[1].toInt(&ok) : -1;
        if (parts.size() > 1 && !ok) {
            qDebug() << "Nieprawidłowy numer. Podaj komendę i numer, np. 'play 0'.";
            continue;
        }

        if (command == "quit") {
            qDebug() << "Do zobaczenia!";
            break;
        } else if (command == "play" && index != -1) {
            game.playCardForCurrentPlayer(index);
        } else if (command == "buy" && index != -1) {
            game.buyCardFromLineUp(index);
        } else if (command == "kick") {
            game.buyKick();
        } else if (command == "sv") {
            game.buySuperVillain();
        } else if (command == "end") {
            game.nextTurn();
        } else {
            qDebug() << "Nieznana komenda.";
        }
    }

    qDebug() << "\n====== APLIKACJA ZAKOŃCZYŁA DZIAŁANIE ======";
    return 0;
}
