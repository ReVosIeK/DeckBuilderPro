#ifndef CARD_H
#define CARD_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>

class Card : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QString subtype READ subtype CONSTANT)
    Q_PROPERTY(int cost READ cost CONSTANT)
    Q_PROPERTY(int power READ power CONSTANT)
    Q_PROPERTY(bool isSpecial READ isSpecial CONSTANT)
    Q_PROPERTY(QString imagePath READ imagePath CONSTANT) // Nowa właściwość
    Q_PROPERTY(QString name READ name NOTIFY languageChanged)
    Q_PROPERTY(QString text READ text NOTIFY languageChanged)

public:
    explicit Card(QObject *parent = nullptr);

    QString id() const;
    QString type() const;
    QString subtype() const;
    int cost() const;
    int power() const;
    bool isSpecial() const;
    QString imagePath() const; // Nowy getter

    Q_INVOKABLE QString getName(const QString& lang) const;
    Q_INVOKABLE QString getText(const QString& lang) const;

    QString m_id;
    QMap<QString, QString> m_names;
    QString m_type;
    QString m_subtype;
    int m_cost;
    int m_power;
    QMap<QString, QString> m_texts;
    bool m_isSpecial;
    QList<QString> m_effectTags;
    QString m_imagePath; // Nowe pole

    static QString currentLanguage;

signals:
    void languageChanged();

private:
    QString name() const;
    QString text() const;
};

#endif // CARD_H
