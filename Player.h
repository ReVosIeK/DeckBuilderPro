#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include <vector>
#include <memory>
#include <QQmlListProperty> // <-- DODANY NAGŁÓWEK
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

signals:
    void currentPowerChanged();
    void handChanged();
    void playedCardsChanged();
    void discardPileChanged();
    void deckChanged();

private:
    QString m_name;
    int m_currentPower;

    std::vector<Card*> m_deck;
    std::vector<Card*> m_hand;
    std::vector<Card*> m_playedCards;
    std::vector<Card*> m_discardPile;

    void shuffleDeck();
};

#endif // PLAYER_H
