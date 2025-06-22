#include "GameManager.h"
#include "Card.h"
#include <QDebug>
#include <random>
#include <algorithm>
#include <QList>

GameManager::GameManager(QObject *parent)
    : QObject(parent),
    m_gameState(GameState::Normal),
    m_cardsToDiscard(0),
    m_currentPlayerIndex(-1)
{
    if (!m_cardLoader.loadCards(":/cards.json", ":/deck_composition.json")) {
        qCritical("Failed to load card data.");
    }
}

void GameManager::prepareGame(int numberOfPlayers) {
    m_gameState = GameState::Normal;
    emit gameStateChanged();

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
            newCard->m_effectTags = kickPrototype->m_effectTags;
            newCard->m_imagePath = kickPrototype->m_imagePath;
            m_kickStack.push_back(newCard);
        }
    }
    emit kickStackChanged();

    qDeleteAll(m_superVillainStack);
    m_superVillainStack.clear();
    std::vector<std::shared_ptr<Card>> svPrototypes = m_cardLoader.getSuperVillains();
    std::shared_ptr<Card> rasAlGhulPrototype = nullptr;
    auto it = std::remove_if(svPrototypes.begin(), svPrototypes.end(), [&](const std::shared_ptr<Card>& card){
        if (card->m_id == "ras_al_ghul") {
            rasAlGhulPrototype = card;
            return true;
        }
        return false;
    });
    svPrototypes.erase(it, svPrototypes.end());
    std::shuffle(svPrototypes.begin(), svPrototypes.end(), g);
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
        newCard->m_effectTags = svPrototype->m_effectTags;
        newCard->m_imagePath = svPrototype->m_imagePath;
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
        newCard->m_effectTags = rasAlGhulPrototype->m_effectTags;
        newCard->m_imagePath = rasAlGhulPrototype->m_imagePath;
        m_superVillainStack.push_back(newCard);
    }
    emit superVillainStackChanged();

    qDeleteAll(m_lineUp);
    m_lineUp.clear();
    refillLineUp();

    m_currentPlayerIndex = 0;
    emit currentPlayerChanged();
    emit gameReady();
}

void GameManager::requestDiscard(int count)
{
    if (m_gameState == GameState::Normal) {
        m_cardsToDiscard = count;
        m_gameState = GameState::PlayerChoosingToDiscard;
        emit gameStateChanged();
        qInfo() << "Game state changed to PlayerChoosingToDiscard. Player must discard" << count << "cards.";
    }
}

void GameManager::resolveDiscard(const QList<int>& cardIndices)
{
    if (m_gameState != GameState::PlayerChoosingToDiscard) return;

    if (cardIndices.size() != m_cardsToDiscard) {
        qWarning() << "Player selected" << cardIndices.size() << "cards, but should have selected" << m_cardsToDiscard;
        return;
    }

    if (Player* p = currentPlayer()) {
        p->discardCardsFromHand(cardIndices);
    }

    m_cardsToDiscard = 0;
    m_gameState = GameState::Normal;
    emit gameStateChanged();
    qInfo() << "Player finished discarding. Game state back to Normal.";
}

void GameManager::playCardFromHand(int index) {
    if (m_gameState != GameState::Normal) {
        qWarning() << "Cannot play card while not in Normal game state.";
        return;
    }
    if (Player* p = currentPlayer()) {
        p->playCard(index);
    }
}

void GameManager::buyCardFromLineUp(int index)
{
    if (m_gameState != GameState::Normal) {
        qWarning() << "Cannot buy card while not in Normal game state.";
        return;
    }
    Player* player = currentPlayer();
    if (!player) { return; }
    if (index < 0 || index >= m_lineUp.size() || m_lineUp.at(index) == nullptr) { return; }

    Card* cardToBuy = m_lineUp.at(index);
    if (player->currentPower() < cardToBuy->cost()) { return; }

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
    if (m_gameState != GameState::Normal) return;
    Player* player = currentPlayer();
    if (!player) return;
    if (m_kickStack.empty()) return;
    Card* kickToBuy = topOfKickStack();
    if (player->currentPower() < kickToBuy->cost()) return;

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
    if (m_gameState != GameState::Normal) return;
    Player* player = currentPlayer();
    if (!player) return;
    if (m_superVillainStack.empty()) return;

    Card* svToDefeat = topOfSuperVillainStack();
    if (player->currentPower() < svToDefeat->cost()) return;

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
    if (m_gameState != GameState::Normal) {
        qWarning() << "Cannot end turn while not in Normal game state.";
        return;
    }
    Player* player = currentPlayer();
    if (!player) return;

    qInfo() << "Ending turn for" << player->name();
    player->endTurn();
    refillLineUp();
    m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();
    emit currentPlayerChanged();
    qInfo() << "Starting turn for" << currentPlayer()->name();
}

void GameManager::refillLineUp() {
    bool changed = false;
    for (size_t i = 0; i < m_lineUp.size(); ++i) {
        if (m_lineUp[i] == nullptr) {
            if (m_mainDeck.empty()) {
                qWarning() << "Main deck is empty! Cannot refill slot in Line-Up.";
                continue;
            }
            std::shared_ptr<Card> cardPrototype = m_mainDeck.back();
            m_mainDeck.pop_back();
            emit mainDeckChanged();

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

            m_lineUp[i] = newCard;
            changed = true;
        }
    }

    if (m_lineUp.size() < 5) {
        const int lineUpSize = 5;
        for (size_t i = m_lineUp.size(); i < lineUpSize && !m_mainDeck.empty(); ++i) {
            std::shared_ptr<Card> cardPrototype = m_mainDeck.back();
            m_mainDeck.pop_back();
            emit mainDeckChanged();

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
            m_lineUp.push_back(newCard);
        }
        changed = true;
    }

    if (changed) {
        emit lineUpChanged();
    }
}

Player* GameManager::currentPlayer() const {
    if (m_players.empty() || m_currentPlayerIndex < 0 || m_currentPlayerIndex >= m_players.size()) {
        return nullptr;
    }
    return m_players[m_currentPlayerIndex];
}

QQmlListProperty<Card> GameManager::lineUp() {
    return QQmlListProperty<Card>(this, this,
                                  [](QQmlListProperty<Card>* prop) -> qsizetype {
                                      return reinterpret_cast<GameManager*>(prop->data)->m_lineUp.size();
                                  },
                                  [](QQmlListProperty<Card>* prop, qsizetype index) -> Card* {
                                      return reinterpret_cast<GameManager*>(prop->data)->m_lineUp.at(index);
                                  }
                                  );
}

Card* GameManager::topOfKickStack() const {
    if (m_kickStack.empty()) {
        return nullptr;
    }
    return m_kickStack.back();
}

int GameManager::kickStackSize() const {
    return m_kickStack.size();
}

Card* GameManager::topOfSuperVillainStack() const {
    if (m_superVillainStack.empty()) {
        return nullptr;
    }
    return m_superVillainStack.back();
}

int GameManager::superVillainStackSize() const {
    return m_superVillainStack.size();
}

int GameManager::mainDeckSize() const {
    return m_mainDeck.size();
}

int GameManager::weaknessStackSize() const {
    return m_weaknessStack.size();
}

bool GameManager::isPlayerChoosing() const
{
    return m_gameState == GameState::PlayerChoosingToDiscard;
}

QString GameManager::choicePrompt() const
{
    if (m_gameState == GameState::PlayerChoosingToDiscard) {
        return QString("Wybierz %1 kart(y) do odrzucenia.").arg(m_cardsToDiscard);
    }
    return "";
}

int GameManager::cardsToDiscard() const
{
    return m_cardsToDiscard;
}
