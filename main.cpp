#include <QCoreApplication>
#include <QDebug>
#include <QMetaEnum>
#include "CardLoader.h"
#include "Card.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "====== URUCHAMIANIE TESTU ŁADOWANIA KART ======";

    CardLoader loader;

    // ZMIANA: Poprawiono nazwę pliku z "card.json" na "cards.json"
    QList<Card*> allCards = loader.loadCardsFromFile("cards.json");

    if(allCards.isEmpty()) {
        qWarning() << "====== WYNIKI TESTU ======";
        qWarning() << "Test NIEUDANY. Nie załadowano żadnych kart. Sprawdź, czy plik 'cards.json' istnieje i jest w odpowiednim miejscu.";
    } else {
        qDebug() << "\n====== WYNIKI TESTU ======";
        qDebug() << "Test ZAKOŃCZONY SUKCESEM.";
        qDebug() << "Przykład - pierwsza załadowana karta:";

        Card* firstCard = allCards.first();
        qDebug() << "ID:" << firstCard->id();
        qDebug() << "Nazwa (PL):" << firstCard->name("pl");
        qDebug() << "Koszt:" << firstCard->cost();

        const QMetaObject* metaObject = firstCard->metaObject();
        int typeIndex = metaObject->indexOfEnumerator("CardType");
        QMetaEnum metaEnum = metaObject->enumerator(typeIndex);
        qDebug() << "Typ:" << metaEnum.valueToKey(firstCard->cardType());
    }

    qDebug() << "\n====== TEST ZAKOŃCZONY ======";

    return 0;
}
