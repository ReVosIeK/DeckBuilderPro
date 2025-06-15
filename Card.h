#ifndef CARD_H
#define CARD_H

#include <QObject>
#include <QString>
#include <QMap>

class Card : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QString subtype READ subtype CONSTANT)
    Q_PROPERTY(int cost READ cost CONSTANT)
    Q_PROPERTY(int power READ power CONSTANT)
    Q_PROPERTY(bool isSpecial READ isSpecial CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY languageChanged) // Właściwość do odczytu w QML
    Q_PROPERTY(QString text READ text NOTIFY languageChanged) // Właściwość do odczytu w QML

public:
    explicit Card(QObject *parent = nullptr);

    // Getters dla właściwości
    QString id() const;
    QString type() const;
    QString subtype() const;
    int cost() const;
    int power() const;
    bool isSpecial() const;

    // Metody Q_INVOKABLE do jawnego pobierania danych w danym języku
    Q_INVOKABLE QString getName(const QString& lang) const;
    Q_INVOKABLE QString getText(const QString& lang) const;

    // Pola publiczne do łatwego wypełniania w CardLoader
    QString m_id;
    QMap<QString, QString> m_names;
    QString m_type;
    QString m_subtype;
    int m_cost;
    int m_power;
    QMap<QString, QString> m_texts;
    bool m_isSpecial;

    // Język, który ma być używany przez właściwości Q_PROPERTY
    static QString currentLanguage;

signals:
    void languageChanged();

private:
    // Prywatne implementacje getterów dla właściwości Q_PROPERTY
    QString name() const;
    QString text() const;
};

#endif // CARD_H
