#include "Player.h"
#include "GameManager.h" // Dołączamy, aby mieć dostęp do GameManager
#include <random>
#include <algorithm>
#include <QDebug>

Player::Player(QString name, const std::vector<std::shared_ptr<Card>>& startingDeck, GameManager* gameManager)
    : QObject(gameManager), m_name(name), m_currentPower(0), m_gameManager(gameManager)
{
    for (const auto& cardPrototype : startingDeck) {
        Card* newCard = new Card(this);
        newCard->m_id = cardPrototype->m_id;
        newCard->m_names = cardPrototype->m_names;
        newCard->m_type = cardPrototype->m_type;
        newCard->m_subtype = cardPrototype->m_subtype;
        newCard->m_cost = cardPrototype->m_cost;
        newCard->m_power = cardPrototype->m_power;
        newCard->m_texts = cardPrototype->m_texts;
        newCard->m_isSpecial = cardPrototype->m_isSpecial;
        newCard->m_effectTags = cardPrototype->m_effectTags;
        newCard->m_imagePath = cardPrototype->m_imagePath;
        m_deck.push_back(newCard);
    }
    shuffleDeck();
    drawHand();
}

QString Player::name() const { return m_name; }
int Player::currentPower() const { return m_currentPower; }
int Player::discardPileSize() const { return m_discardPile.size(); }
int Player::deckSize() const { return m_deck.size(); }

void Player::playCard(int index)
{
    if (index < 0 || index >= m_hand.size()) {
        qWarning() << "Invalid card index to play:" << index;
        return;
    }

    Card* card = m_hand.at(index);
    m_hand.erase(m_hand.begin() + index);
    m_playedCards.push_back(card);

    m_currentPower += card->power();
    qInfo() << name() << "played" << card->getName(Card::currentLanguage) << "for" << card->power() << "power. Total power:" << m_currentPower;

    emit handChanged(); // Emitujemy od razu, żeby karta zniknęła z ręki
    emit playedCardsChanged();
    emit currentPowerChanged();

    for (const QString& tag : card->m_effectTags) {
        if (tag.startsWith("draw:")) {
            bool ok;
            int amountToDraw = tag.mid(5).toInt(&ok);
            if (ok && amountToDraw > 0) {
                qInfo() << "  -> Effect triggered: draw" << amountToDraw;
                drawCards(amountToDraw);
            }
        }
        else if (tag.startsWith("then_discard_card:")) {
            QString subTag = tag.mid(18); // "count=2_from_hand_choice"
            if (subTag.startsWith("count=")) {
                bool ok;
                int amountToDiscard = subTag.mid(6).split("_").first().toInt(&ok);
                if (ok && amountToDiscard > 0) {
                    qInfo() << "  -> Effect triggered: must discard" << amountToDiscard;
                    m_gameManager->requestDiscard(amountToDiscard);
                }
            }
        }
    }
}

void Player::spendPower(int amount)
{
    if (amount > m_currentPower) {
        qWarning() << "Attempted to spend more power than available!";
        return;
    }
    m_currentPower -= amount;
    emit currentPowerChanged();
}

void Player::drawCards(int count)
{
    for (int i = 0; i < count; ++i) {
        if (m_deck.empty()) {
            if (m_discardPile.empty()) {
                qInfo() << name() << "has no more cards to draw from deck or discard pile.";
                break;
            }
            qInfo() << name() << "is reshuffling their discard pile into a new deck.";
            m_deck = m_discardPile;
            m_discardPile.clear();
            shuffleDeck();
            emit discardPileChanged();
        }
        m_hand.push_back(m_deck.back());
        m_deck.pop_back();
    }
    qInfo() << name() << "drew" << count << "card(s). Hand size is now" << m_hand.size();
    emit handChanged();
    emit deckChanged();
}

void Player::discardCardsFromHand(const QList<int>& indices)
{
    // Sortujemy indeksy malejąco, aby uniknąć problemów
    // z przesuwaniem się elementów po usunięciu
    QList<int> sortedIndices = indices;
    std::sort(sortedIndices.rbegin(), sortedIndices.rend());

    for (int index : sortedIndices) {
        if (index < 0 || index >= m_hand.size()) continue;

        qInfo() << name() << "discards" << m_hand[index]->getName(Card::currentLanguage);
        m_discardPile.push_back(m_hand[index]);
        m_hand.erase(m_hand.begin() + index);
    }

    emit handChanged();
    emit discardPileChanged();
}

QQmlListProperty<Card> Player::hand()
{
    return QQmlListProperty<Card>(this, this,
                                  [](QQmlListProperty<Card>* prop) -> qsizetype {
                                      return reinterpret_cast<Player*>(prop->data)->m_hand.size();
                                  },
                                  [](QQmlListProperty<Card>* prop, qsizetype index) -> Card* {
                                      return reinterpret_cast<Player*>(prop->data)->m_hand.at(index);
                                  }
                                  );
}

QQmlListProperty<Card> Player::playedCards()
{
    return QQmlListProperty<Card>(this, this,
                                  [](QQmlListProperty<Card>* prop) -> qsizetype {
                                      return reinterpret_cast<Player*>(prop->data)->m_playedCards.size();
                                  },
                                  [](QQmlListProperty<Card>* prop, qsizetype index) -> Card* {
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
    drawCards(5);
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
