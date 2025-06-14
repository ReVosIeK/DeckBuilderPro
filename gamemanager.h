#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include "CardLoader.h"
#include "Player.h"

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = nullptr);

    void setupNewGame(int playerCount);

private:
    void loadAndPrepareCards();
    void prepareDecks(); // <-- Nowa metoda do sortowania kart
    void createPlayers(int count);
    void dealStartingHands();

    CardLoader m_cardLoader;

    QMap<QString, Card*> m_cardsById;

    QList<Card*> m_mainDeck;
    QList<Card*> m_kickStack;
    QList<Card*> m_weaknessStack;
    QList<Card*> m_superVillainStack;

    QList<Player*> m_players;
    Player* m_currentPlayer;
};

#endif // GAMEMANAGER_H
