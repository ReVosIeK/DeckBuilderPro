#include "Player.h"
#include <random>
#include <algorithm>
#include <chrono>
#include <QDebug>

Player::Player(const QString &heroId, QObject *parent) : QObject(parent), m_heroId(heroId)
{
}

void Player::prepareStartingDeck(Card* punchCard, Card* vulnerabilityCard)
{
    // ... (bez zmian)
    m_deck.clear();
    m_hand.clear();
    m_discardPile.clear();
    m_activeLocations.clear();
    for (int i = 0; i < 7; ++i) { m_deck.append(punchCard); }
    for (int i = 0; i < 3; ++i) { m_deck.append(vulnerabilityCard); }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_deck.begin(), m_deck.end(), std::default_random_engine(seed));
}

// NOWA IMPLEMENTACJA
void Player::drawCards(int count)
{
    for (int i = 0; i < count; ++i) {
        // Jeśli talia jest pusta, potasuj stos kart odrzuconych i uczyń go nową talią
        if (m_deck.isEmpty()) {
            qDebug() << "    -> Talia gracza" << m_heroId << "jest pusta. Tasowanie stosu odrzuconych...";
            shuffleDiscardIntoDeck();
        }

        // Jeśli po potasowaniu talia wciąż jest pusta (bo nie było nic do odrzucenia), przerwij dobieranie
        if (m_deck.isEmpty()) {
            qWarning() << "    -> Gracz" << m_heroId << "nie ma więcej kart do dobrania.";
            break;
        }

        // Przenieś jedną kartę z wierzchu talii do ręki
        m_hand.append(m_deck.takeFirst());
    }
}

const QList<Card*>& Player::hand() const
{
    return m_hand;
}

// NOWA IMPLEMENTACJA
void Player::shuffleDiscardIntoDeck()
{
    // Wszystkie karty ze stosu odrzuconych stają się nową talią
    m_deck = m_discardPile;
    // Czyścimy stos kart odrzuconych
    m_discardPile.clear();
    // Tasujemy nową talię
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_deck.begin(), m_deck.end(), std::default_random_engine(seed));
}
