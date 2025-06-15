#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <vector>
#include <string>
#include <memory>
#include "Player.h"
#include "CardLoader.h"

class GameManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Player* currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)

public:
    explicit GameManager(QObject *parent = nullptr);
    void prepareGame(int numberOfPlayers);

    Player* currentPlayer() const;

signals:
    void gameReady();
    void currentPlayerChanged();

private:
    CardLoader m_cardLoader;
    std::vector<std::shared_ptr<Card>> m_mainDeck;
    std::vector<Player*> m_players;
    int m_currentPlayerIndex;
};

#endif // GAMEMANAGER_H
