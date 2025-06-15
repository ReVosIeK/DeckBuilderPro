#include "GameManager.h"
#include <QDebug>

GameManager::GameManager(QObject *parent) : QObject(parent), m_currentPlayerIndex(-1)
{
    if (!m_cardLoader.loadCards(":/cards.json", ":/deck_composition.json")) {
        qCritical("Failed to load card data.");
        // W prawdziwej aplikacji tutaj obsłużylibyśmy błąd
    }
}

void GameManager::prepareGame(int numberOfPlayers) {
    // Upewnij się, że starzy gracze są poprawnie usunięci
    qDeleteAll(m_players);
    m_players.clear();

    const auto& startingDeck = m_cardLoader.getStartingDeck();
    for (int i = 0; i < numberOfPlayers; ++i) {
        m_players.push_back(new Player("Gracz " + QString::number(i + 1), startingDeck, this));
    }

    m_mainDeck = m_cardLoader.getMainDeck();
    // Tutaj logika tasowania i przygotowania Line-Up

    m_currentPlayerIndex = 0;
    emit currentPlayerChanged();
    emit gameReady();
}

Player* GameManager::currentPlayer() const
{
    if (m_players.empty() || m_currentPlayerIndex < 0 || m_currentPlayerIndex >= m_players.size()) {
        return nullptr;
    }
    return m_players[m_currentPlayerIndex];
}
