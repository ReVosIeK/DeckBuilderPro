#include "Player.h"
#include <random>
#include <algorithm>
#include <chrono>

Player::Player(const QString &heroId, QObject *parent) : QObject(parent), m_heroId(heroId) {}

void Player::prepareStartingDeck(Card* punchCard, Card* vulnerabilityCard) {
    m_deck.clear();
    m_hand.clear();
    m_discardPile.clear();
    m_activeLocations.clear();
    for (int i = 0; i < 7; ++i) { m_deck.append(punchCard); }
    for (int i = 0; i < 3; ++i) { m_deck.append(vulnerabilityCard); }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_deck.begin(), m_deck.end(), std::default_random_engine(seed));
}

void Player::drawCards(int count) { /* Logika do implementacji w przyszłości */ }