#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QMap>

class Card
{
public:
    enum CardType {
        Starter,
        Hero,
        Villain,
        SuperVillain,
        Equipment,
        SuperPower,
        Location,
        Kick,
        Weakness,
        Unknown
    };

    Card();

    QString id() const;
    QString name(const QString& lang) const;
    int cost() const;
    int vp() const;
    CardType cardType() const;
    QString ability(const QString& lang) const;
    QString imagePath() const;
    int power() const;
    QList<QString> effectTags() const;

    bool is(CardType type) const;

    void setId(const QString &id);
    void addName(const QString& lang, const QString& name);
    void setCost(int cost);
    void setVp(int vp);
    void setCardType(const QString &typeStr);
    void addAbility(const QString& lang, const QString& ability);
    void setImagePath(const QString &imagePath);
    void setPower(int power);
    void setEffectTags(const QList<QString> &effectTags);

private:
    QString m_id;
    QMap<QString, QString> m_names;
    int m_cost;
    int m_vp;
    CardType m_cardType;
    QMap<QString, QString> m_abilities;
    QString m_imagePath;
    int m_power;
    QList<QString> m_effectTags;
};

#endif // CARD_H
