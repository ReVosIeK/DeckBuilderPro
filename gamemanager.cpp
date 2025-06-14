#include "GameManager.h"
#include <QDebug>
#include <random>
#include <algorithm>
#include <chrono>

GameManager::GameManager(QObject *parent) : QObject(parent)
{
    // Rejestrujemy nasz enum, aby można go było używać w qDebug
    qRegisterMetaType<Card::CardType>("Card::CardType");
}

void GameManager::setupNewGame(int playerCount)
{
    qDebug() << "[GameManager] Rozpoczynam przygotowanie nowej gry dla" << playerCount << "graczy...";

    loadAndPrepareCards();

    // Jeśli ładowanie kart się nie powiodło, przerwij setup.
    if (m_cardsById.isEmpty()) return;

    // Wywołujemy naszą nową metodę
    prepareDecks();

    qDebug() << "[GameManager] Przygotowanie gry zakończone.";
}

void GameManager::loadAndPrepareCards()
{
    // Ta metoda pozostaje bez zmian
    QList<Card*> allCards = m_cardLoader.loadCardsFromFile("cards.json");

    if (allCards.isEmpty()) {
        qWarning() << "[GameManager] BŁĄD: Nie udało się wczytać żadnych kart. Przerywam przygotowanie gry.";
        return;
    }

    for (Card* card : allCards) {
        m_cardsById.insert(card->id(), card);
    }

    qDebug() << "[GameManager] Wszystkie karty wczytane i zmapowane.";
}

// NOWA IMPLEMENTACJA
void GameManager::prepareDecks()
{
    qDebug() << "[GameManager] Rozpoczynam sortowanie kart do talii...";

    // Wyczyść talie na wypadek, gdyby to była nowa gra
    m_mainDeck.clear();
    m_kickStack.clear();
    m_weaknessStack.clear();
    m_superVillainStack.clear();

    // Przejdź przez wszystkie wczytane karty i przypisz je do odpowiednich stosów
    for (Card* card : m_cardsById.values()) {
        switch (card->cardType()) {
        case Card::Kick:
            m_kickStack.append(card);
            break;
        case Card::Weakness:
            m_weaknessStack.append(card);
            break;
        case Card::SuperVillain:
            m_superVillainStack.append(card);
            break;
        case Card::Starter:
            // Karty startowe (Punch, Vulnerability) nie trafiają do żadnej z tych talii
            break;
        default:
            // Wszystkie pozostałe (Hero, Villain, Equipment, Super Power, Location) trafiają do talii głównej
            m_mainDeck.append(card);
            break;
        }
    }

    // Potasuj talię główną
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(m_mainDeck.begin(), m_mainDeck.end(), std::default_random_engine(seed));

    // TODO: W przyszłości dodamy tutaj specjalną logikę dla stosu Super-Złoczyńców (Ra's al Ghul na wierzchu)

    qDebug() << "[GameManager] Talie przygotowane. Rozmiary talii:";
    qDebug() << "  - Talia Główna:" << m_mainDeck.count();
    qDebug() << "  - Stos Kopniaków:" << m_kickStack.count();
    qDebug() << "  - Stos Słabości:" << m_weaknessStack.count();
    qDebug() << "  - Stos Super-Złoczyńców:" << m_superVillainStack.count();
}


void GameManager::createPlayers(int count)
{
    // Do implementacji w przyszłości
}

void GameManager::dealStartingHands()
{
    // Do implementacji w przyszłości
}
