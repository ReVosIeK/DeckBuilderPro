#include "Player.h"
#include <random>
#include <algorithm>
#include <chrono>
#include <QDebug>

Player::Player(const QString &heroId, QObject *parent)
    : QObject(parent), m_heroId(heroId), m_currentPower(0)
{
}

QString Player::heroId() const
{
    return m_heroId;
}

int Player::currentPower() const
{
    return m_currentPower;
}

void Player::addPower(int amount)
{
    m_currentPower += amount;
}

void Player::prepareStartingDeck(Card* punchCard, Card* vulnerabilityCard)
{
    m_deck.clear();
    m_hand.clear();
    m_discardPile.clear();
    m_activeLocations.clear();
    m_playedCards.clear();
    for (int i = 0; i < 7; ++i) { m_deck.append(punchCard); }
    for (int i = 0; i < 3; ++i) { m_deck.append(vulnerabilityCard); }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_deck.begin(), m_deck.end(), std::default_random_engine(seed));
}

void Player::drawCards(int count)
{
    for (int i = 0; i < count; ++i) {
        if (m_deck.isEmpty()) {
            shuffleDiscardIntoDeck();
        }
        if (m_deck.isEmpty()) {
            qWarning() << "    -> Gracz" << m_heroId << "nie ma więcej kart do dobrania.";
            break;
        }
        m_hand.append(m_deck.takeFirst());
    }
}

Card* Player::playCard(int cardIndex)
{
    if (cardIndex < 0 || cardIndex >= m_hand.size()) {
        qWarning() << "Próba zagrania nieistniejącej karty o indeksie:" << cardIndex;
        return nullptr;
    }

    Card* card = m_hand.takeAt(cardIndex);
    m_playedCards.append(card);

    // Zwracamy kartę, aby GameManager mógł ją przetworzyć
    return card;
}

void Player::endTurn()
{
    qDebug() << "  -> Gracz" << heroId() << "kończy turę z" << m_currentPower << "Mocy.";

    m_discardPile.append(m_hand);
    m_hand.clear();

    m_discardPile.append(m_playedCards);
    m_playedCards.clear();

    m_currentPower = 0;

    drawCards(5);
}

const QList<Card*>& Player::hand() const
{
    return m_hand;
}

void Player::shuffleDiscardIntoDeck()
{
    qDebug() << "    -> Talia gracza"
