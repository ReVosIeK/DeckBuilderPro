#ifndef CARDLOADER_H
#define CARDLOADER_H

#include <QObject>
#include <QList>
#include <QString>
#include "json.hpp" // Dołączamy nową bibliotekę

// Używamy aliasu dla wygody
using json = nlohmann::json;

class Card;

class CardLoader : public QObject
{
    Q_OBJECT
public:
    explicit CardLoader(QObject *parent = nullptr);
    QList<Card*> loadCardsFromFile(const QString& filePath);

private:
    Card* parseCardObject(const json& cardObject);
};

#endif // CARDLOADER_H
