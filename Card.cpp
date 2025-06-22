#include "Card.h"

// Inicjalizacja statycznego pola
QString Card::currentLanguage = "pl_PL";

Card::Card(QObject *parent)
    : QObject(parent), m_cost(0), m_power(0), m_isSpecial(false)
{}

QString Card::id() const { return m_id; }
QString Card::type() const { return m_type; }
QString Card::subtype() const { return m_subtype; }
int Card::cost() const { return m_cost; }
int Card::power() const { return m_power; }
bool Card::isSpecial() const { return m_isSpecial; }
QString Card::imagePath() const { return m_imagePath; } // Implementacja gettera

QString Card::name() const
{
    return getName(currentLanguage);
}

QString Card::text() const
{
    return getText(currentLanguage);
}

QString Card::getName(const QString& lang) const
{
    return m_names.value(lang, m_names.value("en_US", "NO_NAME"));
}

QString Card::getText(const QString& lang) const
{
    return m_texts.value(lang, m_texts.value("en_US", ""));
}
