#include "GameManager.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <random>
#include <algorithm>
#include <chrono>

GameManager::GameManager(QObject *parent) : QObject(parent)
{
    // Konstruktor
}

void GameManager::setupNewGame(int playerCount, int superVillainCount)
{
    qDebug() << "[GameManager] Rozpoczynam przygotowanie nowej gry dla" << playerCount << "graczy...";
    loadCardData();
    if (m_cardsById.isEmpty()) return;

    buildDecks(superVillainCount);

    createPlayers(playerCount);

    // TODO: dealStartingHands();

    qDebug() << "[GameManager] Przygotowanie gry zakończone.";
}

void GameManager::loadCardData()
{
    QList<Card*> allCards = m_cardLoader.loadCardsFromFile("cards.json");
    if (allCards.isEmpty()) {
        qWarning() << "[GameManager] BŁĄD: Nie udało się wczytać definicji kart.";
        return;
    }
    for (Card* card : allCards) {
        m_cardsById.insert(card->id(), card);
    }
    qDebug() << "[GameManager] Wszystkie definicje kart wczytane i zmapowane.";
}

void GameManager::buildDecks(int superVillainCount)
{
    qDebug() << "[GameManager] Rozpoczynam budowanie talii...";
    m_mainDeck.clear();
    m_kickStack.clear();
    m_weaknessStack.clear();
    m_superVillainStack.clear();

    for(int i = 0; i < 16; ++i) m_kickStack.append(m_cardsById.value("kick"));
    for(int i = 0; i < 20; ++i) m_weaknessStack.append(m_cardsById.value("weakness"));

    QFile compositionFile("deck_composition.json");
    if (!compositionFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Nie można otworzyć pliku deck_composition.json";
        return;
    }
    QJsonArray compositionArray = QJsonDocument::fromJson(compositionFile.readAll()).array();
    for (const QJsonValue& value : compositionArray) {
        QJsonObject obj = value.toObject();
        QString cardId = obj["id"].toString();
        int count = obj["count"].toInt();

        Card* cardTemplate = m_cardsById.value(cardId);
        if (cardTemplate) {
            for (int i = 0; i < count; ++i) {
                m_mainDeck.append(cardTemplate);
            }
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_mainDeck.begin(), m_mainDeck.end(), std::default_random_engine(seed));

    prepareSuperVillainStack(superVillainCount);

    qDebug() << "[GameManager] Talie przygotowane. Rozmiary:";
    qDebug() << "  - Talia Główna:" << m_mainDeck.count();
    qDebug() << "  - Stos Kopniaków:" << m_kickStack.count();
    qDebug() << "  - Stos Słabości:" << m_weaknessStack.count();
    qDebug() << "  - Stos Super-Złoczyńców:" << m_superVillainStack.count();
}

void GameManager::prepareSuperVillainStack(int count)
{
    QList<Card*> allSuperVillains;
    for(Card* card : m_cardsById.values()) {
        if(card->cardType() == Card::SuperVillain) {
            allSuperVillains.append(card);
        }
    }

    Card* rasAlGhul = m_cardsById.value("ras_al_ghul");
    allSuperVillains.removeOne(rasAlGhul);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(allSuperVillains.begin(), allSuperVillains.end(), std::default_random_engine(seed));

    m_superVillainStack.append(rasAlGhul);
    for(int i = 0; i < count - 1; ++i) {
        if(i < allSuperVillains.size()) {
            m_superVillainStack.append(allSuperVillains[i]);
        }
    }
}

void GameManager::createPlayers(int count)
{
    qDebug() << "[GameManager] Tworzenie graczy...";
    qDeleteAll(m_players);
    m_players.clear();

    Card* punchCard = m_cardsById.value("punch");
    Card* vulnerabilityCard = m_cardsById.value("vulnerability");

    if (!punchCard || !vulnerabilityCard) {
        qWarning() << "Nie można znaleźć kart startowych 'punch' lub 'vulnerability' w załadowanych danych!";
        return;
    }

    for (int i = 0; i < count; ++i) {
        QString heroId = QString("Player%1_Hero").arg(i + 1);
        Player* newPlayer = new Player(heroId, this);

        newPlayer->prepareStartingDeck(punchCard, vulnerabilityCard);
        m_players.append(newPlayer);
        qDebug() << "  - Stworzono gracza" << (i + 1) << "i przygotowano jego talię startową.";
    }
}

void GameManager::dealStartingHands() { /* Do implementacji w przyszłości */ }
