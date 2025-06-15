#ifndef CARDLOADER_H
#define CARDLOADER_H

#include <QObject>
#include <QList>
#include <QString>
#include "Card.h"
#include "json.hpp"

using json = nlohmann::json;

class CardLoader : public QObject
{
    Q_OBJECT
public:
    explicit CardLoader(QObject *parent = nullptr);

    QList<Card*> loadCardsFromFile(const QString& filePath);
};

#endif // CARDLOADER_H


