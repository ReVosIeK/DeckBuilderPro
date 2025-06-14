#include "Card.h"

Card::Card(const QString &id, const QVariantMap &name, CardType type, int cost, int power, int victoryPoints, const QVariantMap &effectText, const QString &imageUrl, QObject *parent)
    : QObject(parent), m_id(id), m_name(name), m_cardType(type), m_cost(cost), m_power(power), m_victoryPoints(victoryPoints), m_effectText(effectText), m_imageUrl(imageUrl) {}

QString Card::id() const { return m_id; }
Card::CardType Card::cardType() const { return m_cardType; }
int Card::cost() const { return m_cost; }
int Card::power() const { return m_power; }
int Card::victoryPoints() const { return m_victoryPoints; }
QString Card::imageUrl() const { return m_imageUrl; }

QString Card::name(const QString& locale) const { return m_name.value(locale, m_name.value("en")).toString(); }
QString Card::effectText(const QString& locale) const { return m_effectText.value(locale, m_effectText.value("en")).toString(); }

bool Card::is(CardType type) const {
    if (m_cardType == type) return true;
    if (m_cardType == SuperVillain && type == Villain) return true;
    return false;
}