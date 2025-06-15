#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <vector>
#include <string>
#include <memory>
#include <QQmlListProperty>
#include "Player.h"
#include "CardLoader.h"

class Card;

class GameManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Player* currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(QQmlListProperty<Card> lineUp READ lineUp NOTIFY lineUpChanged)
    Q_PROPERTY(Card* topOfKickStack READ topOfKickStack NOTIFY kickStackChanged)
    Q_PROPERTY(int kickStackSize READ kickStackSize NOTIFY kickStackChanged)
    Q_PROPERTY(Card* topOfSuperVillainStack READ topOfSuperVillainStack NOTIFY superVillainStackChanged)
    Q_PROPERTY(int superVillainStackSize READ superVillainStackSize NOTIFY superVillainStackChanged)
    // Nowe właściwości
    Q_PROPERTY(int mainDeckSize READ mainDeckSize NOTIFY mainDeckChanged)
    Q_PROPERTY(int weaknessStackSize READ weaknessStackSize NOTIFY weaknessStackChanged)

public:
    explicit GameManager(QObject *parent = nullptr);
    void prepareGame(int numberOfPlayers);

    Player* currentPlayer() const;
    QQmlListProperty<Card> lineUp();
    Card* topOfKickStack() const;
    int kickStackSize() const;
    Card* topOfSuperVillainStack() const;
    int superVillainStackSize() const;
    int mainDeckSize() const; // Nowy getter
    int weaknessStackSize() const; // Nowy getter

    Q_INVOKABLE void playCardFromHand(int index);
    Q_INVOKABLE void buyCardFromLineUp(int index);
    Q_INVOKABLE void endTurn();
    Q_INVOKABLE void buyKick();
    Q_INVOKABLE void defeatSuperVillain();

signals:
    void gameReady();
    void currentPlayerChanged();
    void lineUpChanged();
    void kickStackChanged();
    void superVillainStackChanged();
    void mainDeckChanged(); // Nowe sygnały
    void weaknessStackChanged(); // Nowe sygnały

private:
    void refillLineUp();

    CardLoader m_cardLoader;
    std::vector<std::shared_ptr<Card>> m_mainDeck;
    std::vector<Player*> m_players;
    std::vector<Card*> m_lineUp;
    std::vector<Card*> m_kickStack;
    std::vector<Card*> m_superVillainStack;
    std::vector<Card*> m_weaknessStack; // Nowy kontener
    int m_currentPlayerIndex;
};

#endif // GAMEMANAGER_H
