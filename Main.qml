import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import com.deckbuilderpro.game 1.0

Window {
    id: root
    width: 1280
    height: 720
    visible: true
    title: "DeckBuilderPro v0.5 - Widok Ręki Gracza"
    color: "#2c3e50"

    // 1. Definiujemy lokalną właściwość QML, która będzie przechowywać aktualnego gracza.
    //    Inicjujemy ją wartością z C++ - jeśli gameManager istnieje.
    property var activePlayer: gameManager ? gameManager.currentPlayer : null

    // 2. Używamy elementu Connections, aby jawnie reagować na sygnały z C++.
    Connections {
        // Nasłuchujemy na obiekcie 'gameManager'. Ignorujemy, jeśli jest chwilowo niedostępny.
        target: gameManager
        // Nazwa funkcji to "on" + nazwa sygnału z C++ z wielkiej litery.
        function onCurrentPlayerChanged() {
            console.log("QML: Otrzymano sygnał onCurrentPlayerChanged!")
            // Gdy sygnał nadejdzie, aktualizujemy naszą lokalną właściwość.
            activePlayer = gameManager.currentPlayer
        }
    }

    // Informacja o aktualnym graczu
    Text {
        id: playerInfo
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        font.pixelSize: 20
        color: "white"
        // 3. Wiążemy tekst do naszej lokalnej, bezpiecznej właściwości 'activePlayer'.
        text: activePlayer ? "Tura gracza: " + activePlayer.name : "Oczekiwanie na gracza..."
    }

    // Strefa ręki gracza
    Rectangle {
        id: handArea
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 250
        color: "#34495e"
        border.color: "#2c3e50"

        Flickable {
            id: handFlickable
            anchors.fill: parent
            anchors.margins: 10
            contentWidth: handRow.width
            contentHeight: height
            flickableDirection: Flickable.HorizontalFlick

            RowLayout {
                id: handRow
                height: parent.height

                Repeater {
                    // 4. Model repeatera również wiążemy do lokalnej właściwości 'activePlayer'.
                    model: activePlayer ? activePlayer.hand : []

                    delegate: Rectangle {
                        id: cardVisual
                        width: 140
                        height: 200
                        Layout.alignment: Qt.AlignVCenter
                        color: "#ecf0f1"
                        radius: 8
                        border.color: "#95a5a6"
                        border.width: 2

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.margins: 8
                            font.bold: true
                            font.pixelSize: 16
                            text: name
                            color: "#2c3e50"
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.WordWrap
                        }

                        Text {
                            anchors.centerIn: parent
                            font.pixelSize: 14
                            text: "Moc: " + power
                            color: "#2980b9"
                        }
                         Text {
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottomMargin: 8
                            font.pixelSize: 14
                            text: "Koszt: " + cost
                            color: "#c0392b"
                        }
                    }
                }
            }
        }
    }
}
