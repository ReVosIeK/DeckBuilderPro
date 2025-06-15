#include "Card.h"
#include <QDebug>

Card::Card() : m_cost(0), m_vp(0), m_cardType(Unknown), m_power(0) {}

QString Card::id() const { return m_id; }
QString Card::name(const QString& lang) const { return m_names.value(lang, "NO_NAME_" + lang.toUpper()); }
int Card::cost() const { return m_cost; }
int Card::vp() const { return m_vp; }
Card::CardType Card::cardType() const { return m_cardType; }
QString Card::ability(const QString& lang) const { return m_abilities.value(lang, ""); }
QString Card::imagePath() const { return m_imagePath; }
int Card::power() const { return m_power; }
QList<QString> Card::effectTags() const { return m_effectTags; }

bool Card::is(CardType type) const { return m_cardType == type; }

void Card::setId(const QString &id) { m_id = id; }
void Card::addName(const QString& lang, const QString& name) { m_names[lang] = name; }
void Card::setCost(int cost) { m_cost = cost; }
void Card::setVp(int vp) { m_vp = vp; }
void Card::setCardType(const QString &typeStr) {
    QString typeLower = typeStr.toLower();
    if (typeLower == "starter") m_cardType = Starter;
    else if (typeLower == "hero") m_cardType = Hero;
    else if (typeLower == "villain") m_cardType = Villain;
    else if (typeLower == "super-villain") m_cardType = SuperVillain;
    else if (typeLower == "equipment") m_cardType = Equipment;
    else if (typeLower == "super power") m_cardType = SuperPower;
    else if (typeLower == "location") m_cardType = Location;
    else if (typeLower == "kick") m_cardType = Kick;
    else if (typeLower == "weakness") m_cardType = Weakness;
    else {
        m_cardType = Unknown;
        qWarning() << "Unknown card type:" << typeStr;
    }
}
void Card::addAbility(const QString& lang, const QString& ability) { m_abilities[lang] = ability; }
void Card::setImagePath(const QString &imagePath) { m_imagePath = imagePath; }
void Card::setPower(int power) { m_power = power; }
void Card::setEffectTags(const QList<QString> &effectTags) { m_effectTags = effectTags; }
