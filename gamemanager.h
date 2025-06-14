#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QList>
#include "CardLoader.h"
#include "Player.h"

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = nullptr);

    // Główna publiczna metoda do inicjalizacji nowej gry
    void setupNewGame(int playerCount);

private:
    // Prywatne metody pomocnicze, które będą wywoływane przez setupNewGame
    void loadAndPrepareCards();
    void createPlayers(int count);
    void dealStartingHands();

    // Komponenty gry
    CardLoader m_cardLoader;

    // Przechowujemy tutaj wskaźniki na wszystkie unikalne karty w grze
    QList<Card*> m_allCards;
    QMap<QString, Card*> m_cardsById; // Dla szybkiego dostępu po ID

    // Stosy i talie gry
    QList<Card*> m_mainDeck;
    QList<Card*> m_kickStack;
    QList<Card*> m_weaknessStack;
    QList<Card*> m_superVillainStack;

    // Gracze
    QList<Player*> m_players;
    Player* m_currentPlayer;
};

#endif // GAMEMANAGER_H
