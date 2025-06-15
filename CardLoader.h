#ifndef CARDLOADER_H
#define CARDLOADER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Card.h"
#include "json.hpp"

using json = nlohmann::json;

class CardLoader {
public:
    bool loadCards(const std::string& cardsPath, const std::string& deckCompositionPath);
    const std::vector<std::shared_ptr<Card>>& getStartingDeck() const;
    const std::vector<std::shared_ptr<Card>>& getMainDeck() const;
    const std::vector<std::shared_ptr<Card>>& getSuperVillains() const;
    std::shared_ptr<Card> getCardById(const std::string& id) const;

private:
    void createDeck(const json& composition, std::vector<std::shared_ptr<Card>>& deck);

    std::map<std::string, std::shared_ptr<Card>> m_cardPrototypes;
    std::vector<std::shared_ptr<Card>> m_startingDeck;
    std::vector<std::shared_ptr<Card>> m_mainDeck;
    std::vector<std::shared_ptr<Card>> m_superVillains;
};

#endif // CARDLOADER_H
