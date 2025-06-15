#include "CardLoader.h"
#include "json.hpp"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <fstream>

using json = nlohmann::json;

CardLoader::CardLoader(QObject *parent) : QObject(parent) {}

QList<Card*> CardLoader::loadCardsFromFile(const QString &filePath)
{
    qDebug() << "[Loader] Próba wczytania kart z" << filePath;
    QList<Card*> cardList;

    std::ifstream file(filePath.toStdString());
    if (!file.is_open()) {
        qWarning() << "[Loader] Nie można otworzyć pliku:" << filePath;
        return cardList;
    }

    json data;
    try {
        data = json::parse(file);
    } catch (json::parse_error& e) {
        qWarning() << "[Loader] Błąd parsowania JSON:" << e.what();
        return cardList;
    }

    qDebug() << "[Loader] Znaleziono" << data.size() << "kart w pliku JSON.";

    for (const auto& item : data) {
        try {
            Card* newCard = new Card();
            newCard->setId(QString::fromStdString(item.value("id", "")));
            newCard->addName("en", QString::fromStdString(item.value("name_en", "")));
            newCard->addName("pl", QString::fromStdString(item.value("name_pl", "")));
            newCard->setCost(item.value("cost", 0));
            newCard->setVp(item.value("vp", 0));
            newCard->setCardType(QString::fromStdString(item.value("type", "Unknown")));
            newCard->addAbility("en", QString::fromStdString(item.value("ability_en", "")));
            newCard->addAbility("pl", QString::fromStdString(item.value("ability_pl", "")));
            newCard->setImagePath(QString::fromStdString(item.value("image_path", "")));
            newCard->setPower(item.value("power", 0));

            QList<QString> tags;
            if (item.contains("effect_tags") && item["effect_tags"].is_array()) {
                for (const auto& tag_item : item["effect_tags"]) {
                    tags.append(QString::fromStdString(tag_item.get<std::string>()));
                }
            }
            newCard->setEffectTags(tags);

            cardList.append(newCard);
        } catch (json::type_error& e) {
            qWarning() << "[Loader] Błąd typu w danych karty:" << e.what();
        }
    }
    qDebug() << "[Loader] Pomyślnie załadowano" << cardList.size() << "kart.";
    return cardList;
}
