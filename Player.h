#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include <vector>
#include <memory>
#include <QQmlListProperty>
#include "Card.h"

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(int currentPower READ currentPower NOTIFY currentPowerChanged)
    Q_PROPERTY(QQmlListProperty<Card> hand READ hand NOTIFY handChanged)
    Q_PROPERTY(QQmlListProperty<Card> playedCards READ playedCards NOTIFY playedCardsChanged)
    Q_PROPERTY(int discardPileSize READ discardPileSize NOTIFY discardPileChanged)
    Q_PROPERTY(int deckSize READ deckSize NOTIFY deckChanged)

public:
    explicit Player(QString name, const std::vector<std::shared_ptr<Card>>& startingDeck, QObject *parent = nullptr);

    QString name() const;
    int currentPower() const;

    QQmlListProperty<Card> hand();
    QQmlListProperty<Card> playedCards();
    int discardPileSize() const;
    int deckSize() const;

    void drawHand();
    void endTurn();

    Q_INVOKABLE void playCard(int index);
    void spendPower(int amount); // Deklaracja metody do wydawania mocy

signals:
    void currentPowerChanged();
    void handChanged();
    void playedCardsChanged();
    void discardPileChanged();
    void deckChanged();

public:
    QString m_name;
    int m_currentPower;

    std::vector<Card*> m_deck;
    std::vector<Card*> m_hand;
    std::vector<Card*> m_playedCards;
    std::vector<Card*> m_discardPile;

private:
    void shuffleDeck();
};

#endif // PLAYER_H
