#include "Player.h"
#include <random>
#include <algorithm>
#include <QDebug>

Player::Player(QString name, const std::vector<std::shared_ptr<Card>>& startingDeck, QObject *parent)
    : QObject(parent), m_name(name), m_currentPower(0)
{
    for (const auto& cardPrototype : startingDeck) {
        // 1. Tworzymy nową, pustą instancję karty, ustawiając gracza jako rodzica
        Card* newCard = new Card(this);

        // 2. Kopiujemy wszystkie dane z prototypu do nowej instancji
        newCard->m_id = cardPrototype->m_id;
        newCard->m_names = cardPrototype->m_names;
        newCard->m_type = cardPrototype->m_type;
        newCard->m_subtype = cardPrototype->m_subtype;
        newCard->m_cost = cardPrototype->m_cost;
        newCard->m_power = cardPrototype->m_power;
        newCard->m_texts = cardPrototype->m_texts;
        newCard->m_isSpecial = cardPrototype->m_isSpecial;

        // 3. Dodajemy nową, w pełni skonfigurowaną kartę do talii gracza
        m_deck.push_back(newCard);
    }
    shuffleDeck();
    drawHand();
}

QString Player::name() const { return m_name; }
int Player::currentPower() const { return m_currentPower; }
int Player::discardPileSize() const { return m_discardPile.size(); }
int Player::deckSize() const { return m_deck.size(); }


QQmlListProperty<Card> Player::hand()
{
    // POPRAWKA: Przekazujemy 'this' jako wskaźnik do danych, a nie nullptr
    return QQmlListProperty<Card>(this, this,
                                  [](QQmlListProperty<Card>* prop) -> qsizetype {
                                      // Zwraca rozmiar wektora m_hand
                                      return reinterpret_cast<Player*>(prop->data)->m_hand.size();
                                  },
                                  [](QQmlListProperty<Card>* prop, qsizetype index) -> Card* {
                                      // Zwraca wskaźnik do karty na podanym indeksie
                                      return reinterpret_cast<Player*>(prop->data)->m_hand.at(index);
                                  }
                                  );
}

QQmlListProperty<Card> Player::playedCards()
{
    // POPRAWKA: Przekazujemy 'this' jako wskaźnik do danych, a nie nullptr
    return QQmlListProperty<Card>(this, this,
                                  [](QQmlListProperty<Card>* prop) -> qsizetype {
                                      // Zwraca rozmiar wektora m_playedCards
                                      return reinterpret_cast<Player*>(prop->data)->m_playedCards.size();
                                  },
                                  [](QQmlListProperty<Card>* prop, qsizetype index) -> Card* {
                                      // Zwraca wskaźnik do karty na podanym indeksie
                                      return reinterpret_cast<Player*>(prop->data)->m_playedCards.at(index);
                                  }
                                  );
}


void Player::shuffleDeck() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_deck.begin(), m_deck.end(), g);
}

void Player::drawHand() {
    for (int i = 0; i < 5; ++i) {
        if (m_deck.empty()) {
            if (m_discardPile.empty()) {
                break; // No more cards to draw
            }
            m_deck = m_discardPile;
            m_discardPile.clear();
            shuffleDeck();
            emit discardPileChanged();
        }
        m_hand.push_back(m_deck.back());
        m_deck.pop_back();
    }
    emit handChanged();
    emit deckChanged();
}

void Player::endTurn() {
    m_currentPower = 0;
    emit currentPowerChanged();

    m_discardPile.insert(m_discardPile.end(), m_hand.begin(), m_hand.end());
    m_hand.clear();

    m_discardPile.insert(m_discardPile.end(), m_playedCards.begin(), m_playedCards.end());
    m_playedCards.clear();

    emit handChanged();
    emit playedCardsChanged();
    emit discardPileChanged();

    drawHand();
}
