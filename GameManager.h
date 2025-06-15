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

    void setupNewGame(int playerCount, int superVillainCount = 8);
    void playFullTurnForCurrentPlayer_Test();

    void buyCardFromLineUp(int lineUpIndex);
    void buyKick();
    void buySuperVillain();

    void endTurnActions();

    Player* currentPlayer() const;

private:
    void loadCardData();
    void buildDecks(int superVillainCount);
    void prepareSuperVillainStack(int count);
    void createPlayers(int count);
    void dealStartingHands();
    void determineFirstPlayer();
    void resolveCardEffect(Card* card);
    void refillLineUp();

    CardLoader m_cardLoader;

    QMap<QString, Card*> m_cardsById;

    QList<Card*> m_mainDeck;
    QList<Card*> m_kickStack;
    QList<Card*> m_weaknessStack;
    QList<Card*> m_superVillainStack;
    QList<Card*> m_lineUp;

    QList<Player*> m_players;
    Player* m_currentPlayer;
};

#endif // GAMEMANAGER_H
