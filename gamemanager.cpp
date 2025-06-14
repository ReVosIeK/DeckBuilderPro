#include "GameManager.h"
#include <QDebug>

GameManager::GameManager(QObject *parent) : QObject(parent)
{
    // Na razie konstruktor jest pusty
}

void GameManager::setupNewGame(int playerCount)
{
    qDebug() << "[GameManager] Rozpoczynam przygotowanie nowej gry dla" << playerCount << "graczy...";

    // Na razie tylko wywołujemy jedną z naszych przyszłych funkcji
    loadAndPrepareCards();

    // Tutaj w przyszłości dodamy createPlayers(), prepareDecks() itd.

    qDebug() << "[GameManager] Przygotowanie gry zakończone.";
}

void GameManager::loadAndPrepareCards()
{
    // Używamy naszego CardLoadera do wczytania wszystkich kart
    m_allCards = m_cardLoader.loadCardsFromFile("cards.json");

    if (m_allCards.isEmpty()) {
        qWarning() << "[GameManager] BŁĄD: Nie udało się wczytać żadnych kart. Przerywam przygotowanie gry.";
        return;
    }

    // Tworzymy mapę dla szybkiego dostępu do kart po ich ID
    for (Card* card : m_allCards) {
        m_cardsById.insert(card->id(), card);
    }

    qDebug() << "[GameManager] Wszystkie" << m_allCards.count() << "karty wczytane i zmapowane.";
}

void GameManager::createPlayers(int count)
{
    // Do implementacji w przyszłości
}

void GameManager::dealStartingHands()
{
    // Do implementacji w przyszłości
}
