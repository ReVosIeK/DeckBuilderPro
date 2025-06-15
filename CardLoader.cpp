#include "CardLoader.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <algorithm> // Dodajemy dla std::shuffle

bool CardLoader::loadCards(const std::string& cardsPath, const std::string& deckCompositionPath) {
    m_cardPrototypes.clear();
    m_superVillains.clear();
    m_startingDeck.clear();
    m_mainDeck.clear();

    QFile cardsFile(QString::fromStdString(cardsPath));
    if (!cardsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open cards file: %s", cardsPath.c_str());
        return false;
    }
    QByteArray cardsData = cardsFile.readAll();
    json cardsJson = json::parse(cardsData.toStdString(), nullptr, false);

    if (cardsJson.is_discarded() || !cardsJson.is_array()) {
        qWarning("Invalid format in cards.json: root must be an array.");
        return false;
    }

    for (const auto& cardDef : cardsJson) {
        if (!cardDef.is_object()) continue;

        auto card = std::make_shared<Card>();
        card->m_id = QString::fromStdString(cardDef.value("id", ""));
        card->m_names["pl_PL"] = QString::fromStdString(cardDef.value("name_pl", ""));
        card->m_names["en_US"] = QString::fromStdString(cardDef.value("name_en", ""));
        card->m_texts["pl_PL"] = QString::fromStdString(cardDef.value("ability_pl", ""));
        card->m_texts["en_US"] = QString::fromStdString(cardDef.value("ability_en", ""));
        card->m_type = QString::fromStdString(cardDef.value("type", ""));
        card->m_subtype = QString::fromStdString(cardDef.value("subtype", ""));
        card->m_cost = cardDef.value("cost", 0);
        card->m_power = cardDef.value("power", 0);
        card->m_isSpecial = false;

        m_cardPrototypes[card->m_id.toStdString()] = card;

        if (card->m_type == "Super-Villain") {
            m_superVillains.push_back(card);
        }
    }
    qInfo() << "Loaded" << m_cardPrototypes.size() << "card prototypes, including" << m_superVillains.size() << "Super-Villains.";

    auto punchProto = getCardById("punch");
    auto vulnerabilityProto = getCardById("vulnerability");
    if(punchProto && vulnerabilityProto) {
        for(int i=0; i<7; ++i) m_startingDeck.push_back(punchProto);
        for(int i=0; i<3; ++i) m_startingDeck.push_back(vulnerabilityProto);
        qInfo() << "Generated starting deck with" << m_startingDeck.size() << "cards.";
    } else {
        qWarning("Could not generate starting deck, missing 'punch' or 'vulnerability' prototype.");
    }

    QFile deckFile(QString::fromStdString(deckCompositionPath));
    if (!deckFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open deck composition file: %s. Main deck will be empty.", deckCompositionPath.c_str());
        return true;
    }
    QByteArray deckData = deckFile.readAll();
    json deckJson = json::parse(deckData.toStdString(), nullptr, false);

    if (deckJson.is_discarded() || !deckJson.is_array()) {
        qWarning("Could not parse deck_composition.json or it's not an array. Main deck will be empty.");
        return true;
    }

    createDeck(deckJson, m_mainDeck);
    qInfo() << "Generated main deck with" << m_mainDeck.size() << "cards.";

    return true;
}

void CardLoader::createDeck(const json& composition, std::vector<std::shared_ptr<Card>>& deck) {
    for (const auto& item : composition) {
        if (!item.is_object()) continue;

        std::string id = item.value("id", "");
        int count = item.value("count", 0);
        auto it = m_cardPrototypes.find(id);
        if (it != m_cardPrototypes.end()) {
            for (int i = 0; i < count; ++i) {
                deck.push_back(it->second);
            }
        } else {
            qWarning("Card with id '%s' from deck_composition not found in card prototypes.", id.c_str());
        }
    }
}

const std::vector<std::shared_ptr<Card>>& CardLoader::getStartingDeck() const {
    return m_startingDeck;
}

const std::vector<std::shared_ptr<Card>>& CardLoader::getMainDeck() const {
    return m_mainDeck;
}

const std::vector<std::shared_ptr<Card>>& CardLoader::getSuperVillains() const
{
    return m_superVillains;
}

std::shared_ptr<Card> CardLoader::getCardById(const std::string& id) const {
    auto it = m_cardPrototypes.find(id);
    if (it != m_cardPrototypes.end()) {
        return it->second;
    }
    return nullptr;
}
