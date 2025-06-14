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

    void prepareStartingDeck(Card* punchCard, Card* vulnerabilityCard);
    void drawCards(int count);

private:
    QString m_heroId;
    QList<Card*> m_deck;
    QList<Card*> m_hand;
    QList<Card*> m_discardPile;
    QList<Card*> m_activeLocations;
};

#endif // PLAYER_H