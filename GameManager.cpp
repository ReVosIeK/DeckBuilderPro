#include "GameManager.h"
#include "Card.h"
#include <QDebug>
#include <random>
#include <algorithm>

GameManager::GameManager(QObject *parent) : QObject(parent), m_currentPlayerIndex(-1)
{
    if (!m_cardLoader.loadCards(":/cards.json", ":/deck_composition.json")) {
        qCritical("Failed to load card data.");
    }
}

void GameManager::prepareGame(int numberOfPlayers) {
    qDeleteAll(m_players);
    m_players.clear();

    const auto& startingDeck = m_cardLoader.getStartingDeck();
    for (int i = 0; i < numberOfPlayers; ++i) {
        m_players.push_back(new Player("Gracz " + QString::number(i + 1), startingDeck, this));
    }

    m_mainDeck = m_cardLoader.getMainDeck();
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_mainDeck.begin(), m_mainDeck.end(), g);
    emit mainDeckChanged();

    qDeleteAll(m_kickStack);
    m_kickStack.clear();
    std::shared_ptr<Card> kickPrototype = m_cardLoader.getCardById("kick");
    if (kickPrototype) {
        for (int i=0; i < 16; ++i) {
            Card* newCard = new Card(this);
            newCard->m_id = kickPrototype->m_id;
            newCard->m_names = kickPrototype->m_names;
            newCard->m_type = kickPrototype->m_type;
            newCard->m_subtype = kickPrototype->m_subtype;
            newCard->m_cost = kickPrototype->m_cost;
            newCard->m_power = kickPrototype->m_power;
            newCard->m_texts = kickPrototype->m_texts;
            newCard->m_isSpecial = kickPrototype->m_isSpecial;
            m_kickStack.push_back(newCard);
        }
    }
    emit kickStackChanged();

    qDeleteAll(m_superVillainStack);
    m_superVillainStack.clear();
    std::vector<std::shared_ptr<Card>> svPrototypes = m_cardLoader.getSuperVillains();
    std::shuffle(svPrototypes.begin(), svPrototypes.end(), g);
    std::shared_ptr<Card> rasAlGhulPrototype = nullptr;
    auto it = std::remove_if(svPrototypes.begin(), svPrototypes.end(), [&](const std::shared_ptr<Card>& card){
        if (card->m_id == "ras_al_ghul") {
            rasAlGhulPrototype = card;
            return true;
        }
        return false;
    });
    svPrototypes.erase(it, svPrototypes.end());
    const int standardGameSvCount = 7;
    if (svPrototypes.size() > standardGameSvCount) {
        svPrototypes.resize(standardGameSvCount);
    }
    for (const auto& svPrototype : svPrototypes) {
        Card* newCard = new Card(this);
        newCard->m_id = svPrototype->m_id;
        newCard->m_names = svPrototype->m_names;
        newCard->m_type = svPrototype->m_type;
        newCard->m_subtype = svPrototype->m_subtype;
        newCard->m_cost = svPrototype->m_cost;
        newCard->m_power = svPrototype->m_power;
        newCard->m_texts = svPrototype->m_texts;
        newCard->m_isSpecial = svPrototype->m_isSpecial;
        m_superVillainStack.push_back(newCard);
    }
    if (rasAlGhulPrototype) {
        Card* newCard = new Card(this);
        newCard->m_id = rasAlGhulPrototype->m_id;
        newCard->m_names = rasAlGhulPrototype->m_names;
        newCard->m_type = rasAlGhulPrototype->m_type;
        newCard->m_subtype = rasAlGhulPrototype->m_subtype;
        newCard->m_cost = rasAlGhulPrototype->m_cost;
        newCard->m_power = rasAlGhulPrototype->m_power;
        newCard->m_texts = rasAlGhulPrototype->m_texts;
        newCard->m_isSpecial = rasAlGhulPrototype->m_isSpecial;
        m_superVillainStack.push_back(newCard);
    }
    emit superVillainStackChanged();

    // Inicjalizacja stosu Słabości
    qDeleteAll(m_weaknessStack);
    m_weaknessStack.clear();
    std::shared_ptr<Card> weaknessPrototype = m_cardLoader.getCardById("weakness");
    if (weaknessPrototype) {
        for (int i = 0; i < 20; ++i) {
            Card* newCard = new Card(this);
            newCard->m_id = weaknessPrototype->m_id;
            newCard->m_names = weaknessPrototype->m_names;
            newCard->m_type = weaknessPrototype->m_type;
            newCard->m_subtype = weaknessPrototype->m_subtype;
            newCard->m_cost = weaknessPrototype->m_cost;
            newCard->m_power = weaknessPrototype->m_power;
            newCard->m_texts = weaknessPrototype->m_texts;
            newCard->m_isSpecial = weaknessPrototype->m_isSpecial;
            m_weaknessStack.push_back(newCard);
        }
    }
    emit weaknessStackChanged();

    qDeleteAll(m_lineUp);
    m_lineUp.clear();
    refillLineUp();

    m_currentPlayerIndex = 0;
    emit currentPlayerChanged();
    emit gameReady();
}

void GameManager::playCardFromHand(int index) {
    if (Player* p = currentPlayer()) {
        p->playCard(index);
    }
}

void GameManager::buyCardFromLineUp(int index)
{
    Player* player = currentPlayer();
    if (!player) { return; }
    if (index < 0 || index >= m_lineUp.size() || m_lineUp.at(index) == nullptr) { return; }

    Card* cardToBuy = m_lineUp.at(index);
    if (player->currentPower() < cardToBuy->cost()) {
        qWarning() << "Not enough power to buy" << cardToBuy->getName("pl_PL");
        return;
    }

    player->spendPower(cardToBuy->cost());
    qInfo() << player->name() << "buys" << cardToBuy->getName("pl_PL");

    cardToBuy->setParent(player);
    player->m_discardPile.push_back(cardToBuy);
    emit player->discardPileChanged();

    m_lineUp[index] = nullptr;

    emit lineUpChanged();
}

void GameManager::buyKick()
{
    Player* player = currentPlayer();
    if (!player) return;
    if (m_kickStack.empty()) {
        qWarning() << "Kick stack is empty!";
        return;
    }

    Card* kickToBuy = topOfKickStack();
    if (player->currentPower() < kickToBuy->cost()) {
        qWarning() << "Not enough power to buy Kick";
        return;
    }

    player->spendPower(kickToBuy->cost());
    qInfo() << player->name() << "buys a Kick";

    m_kickStack.back()->setParent(player);
    player->m_discardPile.push_back(m_kickStack.back());
    m_kickStack.pop_back();

    emit player->discardPileChanged();
    emit kickStackChanged();
}

void GameManager::defeatSuperVillain()
{
    Player* player = currentPlayer();
    if (!player) return;
    if (m_superVillainStack.empty()) {
        qWarning() << "Super-Villain stack is empty!";
        return;
    }

    Card* svToDefeat = topOfSuperVillainStack();
    if (player->currentPower() < svToDefeat->cost()) {
        qWarning() << "Not enough power to defeat" << svToDefeat->getName("pl_PL");
        return;
    }

    player->spendPower(svToDefeat->cost());
    qInfo() << player->name() << "defeats" << svToDefeat->getName("pl_PL") << "!!!";

    m_superVillainStack.back()->setParent(player);
    player->m_discardPile.push_back(m_superVillainStack.back());
    m_superVillainStack.pop_back();

    emit player->discardPileChanged();
    emit superVillainStackChanged();
}

void GameManager::endTurn()
{
    Player* player = currentPlayer();
    if (!player) return;

    qInfo() << "Ending turn for" << player->name();
    player->endTurn();
    refillLineUp();
    m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();
    emit currentPlayerChanged();
    qInfo() << "Starting turn for" << currentPlayer()->name();
}

void GameManager::refillLineUp()
{
    bool changed = false;
    for (size_t i = 0; i < m_lineUp.size(); ++i) {
        if (m_lineUp[i] == nullptr) {
            if (m_mainDeck.empty()) {
                qWarning() << "Main deck is empty! Cannot refill slot in Line-Up.";
                continue;
            }
            std::shared_ptr<Card> cardPrototype = m_mainDeck.back();
            m_mainDeck.pop_back();
            emit mainDeckChanged(); // Sygnał o zmianie rozmiaru talii głównej

            Card* newCard = new Card(this);
            newCard->m_id = cardPrototype->m_id;
            newCard->m_names = cardPrototype->m_names;
            newCard->m_type = cardPrototype->m_type;
            newCard->m_subtype = cardPrototype->m_subtype;
            newCard->m_cost = cardPrototype->m_cost;
            newCard->m_power = cardPrototype->m_power;
            newCard->m_texts = cardPrototype->m_texts;
            newCard->m_isSpecial = cardPrototype->m_isSpecial;

            m_lineUp[i] = newCard;
            changed = true;
        }
    }

    if (m_lineUp.size() < 5) {
        const int lineUpSize = 5;
        for (size_t i = m_lineUp.size(); i < lineUpSize && !m_mainDeck.empty(); ++i) {
            std::shared_ptr<Card> cardPrototype = m_mainDeck.back();
            m_mainDeck.pop_back();
            emit mainDeckChanged(); // Sygnał o zmianie rozmiaru talii głównej

            Card* newCard = new Card(this);
            newCard->m_id = cardPrototype->m_id;
            newCard->m_names = cardPrototype->m_names;
            newCard->m_type = cardPrototype->m_type;
            newCard->m_subtype = cardPrototype->m_subtype;
            newCard->m_cost = cardPrototype->m_cost;
            newCard->m_power = cardPrototype->m_power;
            newCard->m_texts = cardPrototype->m_texts;
            newCard->m_isSpecial = cardPrototype->m_isSpecial;
            m_lineUp.push_back(newCard);
        }
        changed = true;
    }

    if (changed) {
        emit lineUpChanged();
    }
}

Player* GameManager::currentPlayer() const
{
    if (m_players.empty() || m_currentPlayerIndex < 0 || m_currentPlayerIndex >= m_players.size()) {
        return nullptr;
    }
    return m_players[m_currentPlayerIndex];
}

QQmlListProperty<Card> GameManager::lineUp()
{
    return QQmlListProperty<Card>(this, this,
                                  [](QQmlListProperty<Card>* prop) -> qsizetype {
                                      return reinterpret_cast<GameManager*>(prop->data)->m_lineUp.size();
                                  },
                                  [](QQmlListProperty<Card>* prop, qsizetype index) -> Card* {
                                      return reinterpret_cast<GameManager*>(prop->data)->m_lineUp.at(index);
                                  }
                                  );
}

Card* GameManager::topOfKickStack() const
{
    if (m_kickStack.empty()) {
        return nullptr;
    }
    return m_kickStack.back();
}

int GameManager::kickStackSize() const
{
    return m_kickStack.size();
}

Card* GameManager::topOfSuperVillainStack() const
{
    if (m_superVillainStack.empty()) {
        return nullptr;
    }
    return m_superVillainStack.back();
}

int GameManager::superVillainStackSize() const
{
    return m_superVillainStack.size();
}

int GameManager::mainDeckSize() const
{
    return m_mainDeck.size();
}

int GameManager::weaknessStackSize() const
{
    return m_weaknessStack.size();
}
