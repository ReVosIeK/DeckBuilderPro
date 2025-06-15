#include "GameManager.h"
#include "Card.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QRandomGenerator>
#include <random>
#include <algorithm>
#include <chrono>

GameManager::GameManager(QObject *parent) : QObject(parent)
{
    m_currentPlayer = nullptr;
}

void GameManager::setupNewGame(int playerCount, int superVillainCount)
{
    qDebug() << "[GameManager] Rozpoczynam przygotowanie nowej gry dla" << playerCount << "graczy...";
    loadCardData();
    if (m_cardsById.isEmpty()) return;

    buildDecks(superVillainCount);
    createPlayers(playerCount);
    dealStartingHands();
    determineFirstPlayer();
    refillLineUp();

    qDebug() << "[GameManager] Przygotowanie gry zakończone. Gra gotowa do rozpoczęcia.";
}

void GameManager::playFullTurnForCurrentPlayer_Test()
{
    if (!m_currentPlayer) {
        qWarning() << "[GameManager] Błąd krytyczny: Brak aktywnego gracza.";
        return;
    }

    qDebug() << "\n====== TURA GRACZA" << m_players.indexOf(m_currentPlayer) + 1 << "(" << m_currentPlayer->heroId() << ") ======";

    QStringList lineUpNames;
    for(const auto& card : m_lineUp) if(card) lineUpNames.append(card->name("pl"));
    qDebug() << "Line-Up na początku tury:" << lineUpNames.join(", ");

    while(!m_currentPlayer->hand().isEmpty()) {
        Card* playedCard = m_currentPlayer->playCard(0);
        if(playedCard) {
            resolveCardEffect(playedCard);
        }
    }
}

void GameManager::buyCardFromLineUp(int lineUpIndex)
{
    if (!m_currentPlayer) return;

    if(lineUpIndex < 0 || lineUpIndex >= m_lineUp.size() || !m_lineUp[lineUpIndex]) {
        qWarning() << "[GameManager] Próba zakupu nieprawidłowej karty z Line-Up o indeksie:" << lineUpIndex;
        return;
    }

    Card* cardToBuy = m_lineUp[lineUpIndex];
    qDebug() << "[GameManager] Gracz próbuje kupić" << cardToBuy->name("pl") << "za koszt" << cardToBuy->cost() << "(posiada" << m_currentPlayer->currentPower() << "Mocy)";

    if (m_currentPlayer->currentPower() >= cardToBuy->cost()) {
        m_currentPlayer->spendPower(cardToBuy->cost());
        m_currentPlayer->gainCard(cardToBuy);

        m_lineUp[lineUpIndex] = nullptr;

        qDebug() << "  -> Sukces! Karta" << cardToBuy->name("pl") << "kupiona. Pozostało Mocy:" << m_currentPlayer->currentPower();
    } else {
        qDebug() << "  -> Porażka. Niewystarczająca ilość Mocy.";
    }
}

void GameManager::buyKick()
{
    if (!m_currentPlayer) return;
    if (m_kickStack.isEmpty()) {
        qDebug() << "[GameManager] Stos Kopniaków jest pusty. Nie można kupić.";
        return;
    }

    Card* kickCard = m_kickStack.first();
    qDebug() << "[GameManager] Gracz próbuje kupić Kopniaka za koszt" << kickCard->cost() << "(posiada" << m_currentPlayer->currentPower() << "Mocy)";

    if (m_currentPlayer->currentPower() >= kickCard->cost()) {
        m_currentPlayer->spendPower(kickCard->cost());
        m_currentPlayer->gainCard(m_kickStack.takeFirst());
        qDebug() << "  -> Sukces! Kopniak kupiony. Pozostało Mocy:" << m_currentPlayer->currentPower();
    } else {
        qDebug() << "  -> Porażka. Niewystarczająca ilość Mocy.";
    }
}

void GameManager::buySuperVillain()
{
    if (!m_currentPlayer) return;
    if (m_superVillainStack.isEmpty()) {
        qDebug() << "[GameManager] Stos Super-Złoczyńców jest pusty.";
        return;
    }

    Card* svCard = m_superVillainStack.first();
    qDebug() << "[GameManager] Gracz próbuje pokonać Super-Złoczyńcę" << svCard->name("pl") << "za koszt" << svCard->cost() << "(posiada" << m_currentPlayer->currentPower() << "Mocy)";

    if (m_currentPlayer->currentPower() >= svCard->cost()) {
        m_currentPlayer->spendPower(svCard->cost());
        m_currentPlayer->gainCard(m_superVillainStack.takeFirst());
        qDebug() << "  -> Sukces! Super-Złoczyńca pokonany. Pozostało Mocy:" << m_currentPlayer->currentPower();
    } else {
        qDebug() << "  -> Porażka. Niewystarczająca ilość Mocy.";
    }
}

void GameManager::endTurnActions()
{
    if(!m_currentPlayer) return;
    m_currentPlayer->endTurn();
    refillLineUp();
}

Player* GameManager::currentPlayer() const
{
    return m_currentPlayer;
}

void GameManager::resolveCardEffect(Card* card)
{
    if (!card || !m_currentPlayer) return;
    qDebug() << "  -> Rozpatrywanie efektu karty:" << card->name("pl");
    m_currentPlayer->addPower(card->power());

    if (card->id() == "kid_flash" || card->id() == "super_speed") {
        qDebug() << "    -> Efekt: Dobierz kartę.";
        m_currentPlayer->drawCards(1);
    }
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
        if(card->is(Card::SuperVillain)) {
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

    QStringList heroPool = {"the_flash", "batman", "superman", "wonder_woman", "green_lantern", "aquaman", "cyborg"};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(heroPool.begin(), heroPool.end(), std::default_random_engine(seed));

    for (int i = 0; i < count; ++i) {
        QString heroId = heroPool.value(i, QString("Player%1_Default").arg(i + 1));
        Player* newPlayer = new Player(heroId, this);

        newPlayer->prepareStartingDeck(punchCard, vulnerabilityCard);
        m_players.append(newPlayer);
        qDebug() << "  - Stworzono gracza" << (i + 1) << "z bohaterem" << heroId << "i przygotowano jego talię startową.";
    }
}

void GameManager::dealStartingHands()
{
    qDebug() << "[GameManager] Rozdawanie kart startowych...";
    for (int i = 0; i < m_players.count(); ++i) {
        m_players[i]->drawCards(5);
    }

    qDebug() << "\n====== WERYFIKACJA STANU GRY ======";
    for (int i = 0; i < m_players.count(); ++i) {
        QStringList handContents;
        for(Card* card : m_players[i]->hand()) {
            handContents.append(card->name("pl"));
        }
        qDebug() << "Ręka Gracza" << (i + 1) << "(" << m_players[i]->heroId() << "):" << handContents.join(", ");
    }
}

void GameManager::determineFirstPlayer()
{
    qDebug() << "[GameManager] Wybieranie pierwszego gracza...";

    for(int i = 0; i < m_players.count(); ++i) {
        if(m_players[i]->heroId() == "the_flash") {
            m_currentPlayer = m_players[i];
            qDebug() << "  -> Gracz" << (i + 1) << "(" << m_currentPlayer->heroId() << ") gra jako Flash i zaczyna grę!";
            return;
        }
    }

    if (!m_players.isEmpty()) {
        int firstPlayerIndex = QRandomGenerator::global()->bounded(m_players.size());
        m_currentPlayer = m_players[firstPlayerIndex];
        qDebug() << "  -> Losowo wybrano gracza" << firstPlayerIndex + 1 << "(" << m_currentPlayer->heroId() << ")" << "do rozpoczęcia gry.";
    }
}

void GameManager::refillLineUp()
{
    qDebug() << "[GameManager] Uzupełnianie Line-Up...";
    for(int i = 0; i < 5; ++i) {
        if (i >= m_lineUp.size()) {
            // Jeśli slot nie istnieje, dodaj go
            if(!m_mainDeck.isEmpty()) {
                m_lineUp.append(m_mainDeck.takeFirst());
            }
        } else if (!m_lineUp[i]) {
            // Jeśli slot jest pusty (nullptr), uzupełnij go
            if(!m_mainDeck.isEmpty()) {
                m_lineUp[i] = m_mainDeck.takeFirst();
            }
        }
    }
}
