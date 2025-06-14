#include "CardLoader.h"
#include "Card.h"
#include <fstream>
#include <QDebug>

CardLoader::CardLoader(QObject *parent) : QObject(parent) {}

QList<Card*> CardLoader::loadCardsFromFile(const QString &filePath) {
    QList<Card*> loadedCards;

    std::ifstream file(filePath.toStdString());
    if (!file.is_open()) {
        qWarning() << "Nie można otworzyć pliku z kartami:" << filePath;
        return loadedCards;
    }

    json data;
    try {
        data = json::parse(file);
    } catch (const json::parse_error& e) {
        qWarning() << "Błąd parsowania pliku JSON:" << e.what();
        return loadedCards;
    }

    if (!data.is_array()) {
        qWarning() << "Plik JSON nie zawiera tablicy na najwyższym poziomie.";
        return loadedCards;
    }

    qDebug() << "[Loader] Znaleziono" << data.size() << "kart w pliku JSON.";

    for (const auto& cardObject : data) {
        Card* newCard = parseCardObject(cardObject);
        if (newCard) {
            newCard->setParent(this);
            loadedCards.append(newCard);
        }
    }

    qDebug() << "[Loader] Pomyślnie załadowano" << loadedCards.count() << "kart.";
    return loadedCards;
}

Card* CardLoader::parseCardObject(const json &cardObject) {
    try {
        QVariantMap nameMap;
        nameMap.insert("en", QString::fromStdString(cardObject.at("name_en").get<std::string>()));
        nameMap.insert("pl", QString::fromStdString(cardObject.at("name_pl").get<std::string>()));

        QVariantMap abilityMap;
        abilityMap.insert("en", QString::fromStdString(cardObject.at("ability_en").get<std::string>()));
        abilityMap.insert("pl", QString::fromStdString(cardObject.at("ability_pl").get<std::string>()));

        QString typeString = QString::fromStdString(cardObject.at("type").get<std::string>());
        Card::CardType type;

        if (typeString == "Starter") type = Card::Starter;
        else if (typeString == "Super Power") type = Card::Superpower;
        else if (typeString == "Weakness") type = Card::Weakness;
        else if (typeString == "Equipment") type = Card::Equipment;
        else if (typeString == "Hero") type = Card::Hero;
        else if (typeString == "Villain") type = Card::Villain;
        else if (typeString == "Location") type = Card::Location;
        else if (typeString == "Super-Villain") type = Card::SuperVillain;
        else if (typeString == "Kick") type = Card::Kick;
        else {
            if (typeString != "Super-Hero") {
                qWarning() << "Nieznany typ karty:" << typeString << "dla ID:" << QString::fromStdString(cardObject.at("id").get<std::string>());
            }
            return nullptr;
        }

        return new Card(
            QString::fromStdString(cardObject.at("id").get<std::string>()),
            nameMap,
            type,
            cardObject.at("cost").get<int>(),
            cardObject.at("power").get<int>(),
            cardObject.at("vp").get<int>(),
            abilityMap,
            QString::fromStdString(cardObject.at("image_path").get<std::string>())
            );
    } catch (const json::exception& e) {
        qWarning() << "Błąd parsowania obiektu JSON:" << e.what();
        return nullptr;
    }
}
