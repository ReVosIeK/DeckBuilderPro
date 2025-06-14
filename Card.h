#ifndef CARD_H
#define CARD_H

#include <QObject>
#include <QString>
#include <QVariantMap>

class Card : public QObject
{
    Q_OBJECT

public:
    enum CardType { Hero, Villain, Equipment, Superpower, Location, Starter, SuperVillain, Kick, Weakness };
    Q_ENUM(CardType)

    explicit Card(const QString &id, const QVariantMap &name, CardType type, int cost, int power, int victoryPoints, const QVariantMap &effectText, const QString &imageUrl, QObject *parent = nullptr);

    QString id() const;
    CardType cardType() const;
    int cost() const;
    int power() const;
    int victoryPoints() const;
    QString imageUrl() const;
    
    Q_INVOKABLE QString name(const QString& locale = "en") const;
    Q_INVOKABLE QString effectText(const QString& locale = "en") const;
    Q_INVOKABLE bool is(CardType type) const;

private:
    QString m_id;
    QVariantMap m_name;
    CardType m_cardType;
    int m_cost;
    int m_power;
    int m_victoryPoints;
    QVariantMap m_effectText;
    QString m_imageUrl;
};

#endif // CARD_H