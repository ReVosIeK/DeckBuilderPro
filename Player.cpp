#include "Player.h"
#include <random>
#include <algorithm>
#include <chrono>
#include <QDebug>

Player::Player(const QString &heroId, QObject *parent) : QObject(parent), m_heroId(heroId) {}

// Nowa implementacja
QString Player::heroId() const
{
    return m_heroId;
}

// ... reszta pliku bez zmian ...
void Player::prepareStartingDeck(Card* punchCard, Card* vulnerabilityCard) { /* ... */ }
void Player::drawCards(int count) { /* ... */ }
const QList<Card*>& Player::hand() const { return m_hand; }
void Player::shuffleDiscardIntoDeck() { /* ... */ }
