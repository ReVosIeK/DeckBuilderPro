#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include <QList>
#include "Card.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(const QString &heroId, QObject *parent = nullptr);

    // Metody publiczne
    QString heroId() const;
    void prepareStartingDeck(Card* punchCard, Card* vulnerabilityCard);
    void drawCards(int count);
    const QList<Card*>& hand() const;
    int currentPower() const;

    void addPower(int amount);
    void spendPower(int amount);
    void gainCard(Card* card);

    Card* playCard(int cardIndex);
    void endTurn();

private:
    void shuffleDiscardIntoDeck();

    QString m_heroId;
    int m_currentPower;

    QList<Card*> m_deck;
    QList<Card*> m_hand;
    QList<Card*> m_discardPile;
    QList<Card*> m_activeLocations;
    QList<Card*> m_playedCards;
};

#endif // PLAYER_H
