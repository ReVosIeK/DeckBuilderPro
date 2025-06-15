import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

Window {
    id: root
    width: 1280
    height: 768
    visible: true
    title: "DeckBuilderPro v0.5 - Kompletny Stół"
    color: "#2c3e50"

    // --- Właściwości lokalne do bindowania z C++ ---
    property var activePlayer: gameManager ? gameManager.currentPlayer : null
    property var lineUpCards: gameManager ? gameManager.lineUp : []
    property var topKick: gameManager ? gameManager.topOfKickStack : null
    property int kickCount: gameManager ? gameManager.kickStackSize : 0
    property var topSV: gameManager ? gameManager.topOfSuperVillainStack : null
    property int svCount: gameManager ? gameManager.superVillainStackSize : 0
    property int mainDeckCount: gameManager ? gameManager.mainDeckSize : 0
    property int weaknessCount: gameManager ? gameManager.weaknessStackSize : 0

    // --- Połączenia sygnałów z C++ do QML ---
    Connections {
        target: gameManager
        function onCurrentPlayerChanged() { activePlayer = gameManager.currentPlayer }
        function onLineUpChanged() { lineUpCards = gameManager.lineUp }
        function onKickStackChanged() {
            topKick = gameManager.topOfKickStack
            kickCount = gameManager.kickStackSize
        }
        function onSuperVillainStackChanged() {
            topSV = gameManager.topOfSuperVillainStack
            svCount = gameManager.superVillainStackSize
        }
        function onMainDeckChanged() { mainDeckCount = gameManager.mainDeckSize }
        function onWeaknessStackChanged() { weaknessCount = gameManager.weaknessStackSize }
    }

    // --- Panel informacyjny gracza ---
    RowLayout {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 15
        spacing: 20

        Text {
            font.pixelSize: 20; color: "white"
            text: activePlayer ? "Tura gracza: " + activePlayer.name : "..."
        }
        Text {
            font.pixelSize: 20; color: "#f1c40f"; font.bold: true
            text: activePlayer ? "Moc: " + activePlayer.currentPower : "Moc: 0"
        }
        Text {
            font.pixelSize: 16; color: "#bdc3c7"
            text: activePlayer ? "Talia: " + activePlayer.deckSize : ""
        }
        Text {
            font.pixelSize: 16; color: "#bdc3c7"
            text: activePlayer ? "Odrzucone: " + activePlayer.discardPileSize : ""
        }
        Button {
            text: "Zakończ Turę"
            onClicked: { gameManager.endTurn() }
        }
    }

    // --- Centralna strefa stołu (Talia Główna i Line-Up) ---
    RowLayout {
        id: centerAreaLayout
        y: 80
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        // TALIA GŁÓWNA
        Rectangle {
            width: 140
            height: 220 // Wysokość dopasowana do Line-Up
            color: "#2c3e50"
            border.color: "#3498db"
            border.width: 2
            radius: 8

            Text { text: "TALIA\nGŁÓWNA"; anchors.centerIn: parent; color: "white"; font.pixelSize: 20; horizontalAlignment: Text.AlignHCenter }
            Text { text: mainDeckCount; anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottomMargin: 10; font.pixelSize: 22; font.bold: true; color: "white" }
        }

        // LINE-UP
        Rectangle {
            id: lineUpArea
            width: 780 // 5 * 140px (karta) + 4 * 15px (odstęp)
            height: 220
            color: "#16a085"
            border.color: "#1abc9c"
            radius: 5

            RowLayout {
                anchors.centerIn: parent
                spacing: 15
                Repeater {
                    model: lineUpCards
                    delegate: Rectangle {
                        width: 140; height: 200; Layout.alignment: Qt.AlignVCenter;
                        color: modelData ? "#ecf0f1" : "transparent"
                        border.color: modelData ? "#95a5a6" : "transparent"
                        radius: 8; border.width: 2
                        visible: modelData !== null
                        Text { anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right; anchors.margins: 8; font.bold: true; font.pixelSize: 16; text: name; color: "#2c3e50"; horizontalAlignment: Text.AlignHCenter; wrapMode: Text.WordWrap }
                        Text { anchors.centerIn: parent; font.pixelSize: 14; text: "Moc: " + power; color: "#2980b9" }
                        Text { anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottomMargin: 8; font.pixelSize: 14; text: "Koszt: " + cost; color: "#c0392b" }
                        MouseArea {
                            anchors.fill: parent; enabled: modelData !== null
                            cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                            onClicked: { gameManager.buyCardFromLineUp(index) }
                        }
                    }
                }
            }
        }
    }


    // --- Strefa stosów specjalnych (Kick, SV, Weakness) ---
    ColumnLayout {
        anchors.top: centerAreaLayout.top
        anchors.left: centerAreaLayout.right
        anchors.leftMargin: 20
        spacing: 20

        Rectangle {
            id: kickStack
            width: 140
            height: 220
            color: "#2c3e50"
            radius: 8
            border.color: topKick ? "#95a5a6" : "#34495e"
            border.width: 2

            Rectangle {
                anchors.centerIn: parent
                width: 140
                height: 200
                color: "#ecf0f1"
                visible: topKick !== null
                radius: 8

                Text { anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right; anchors.margins: 8; font.bold: true; font.pixelSize: 16; text: topKick ? topKick.name : ""; color: "#2c3e50"; horizontalAlignment: Text.AlignHCenter; wrapMode: Text.WordWrap }
                Text { anchors.centerIn: parent; font.pixelSize: 14; text: "Moc: " + (topKick ? topKick.power : ""); color: "#2980b9" }
                Text { anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottomMargin: 30; font.pixelSize: 14; text: "Koszt: " + (topKick ? topKick.cost : ""); color: "#c0392b" }

                MouseArea {
                    anchors.fill: parent; enabled: topKick !== null
                    cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                    onClicked: { gameManager.buyKick() }
                }
            }
            Text { anchors.fill: parent; text: "Kopniaki\nwyczerpane"; color: "white"; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; visible: topKick === null }
            Text {
                anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 8; font.bold: true; color: "white"
                text: "Pozostało: " + kickCount
            }
        }

        Rectangle {
            id: svStack
            width: 140
            height: 220
            color: "#2c3e50"
            radius: 8
            border.color: topSV ? "#c0392b" : "#34495e"
            border.width: 2

            Rectangle {
                anchors.centerIn: parent
                width: 140
                height: 200
                color: "#ecf0f1"
                visible: topSV !== null
                radius: 8

                Text { anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right; anchors.margins: 8; font.bold: true; font.pixelSize: 16; text: topSV ? topSV.name : ""; color: "#c0392b"; horizontalAlignment: Text.AlignHCenter; wrapMode: Text.WordWrap }
                Text { anchors.centerIn: parent; font.pixelSize: 14; text: "Moc: " + (topSV ? topSV.power : ""); color: "#2980b9" }
                Text { anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottomMargin: 30; font.pixelSize: 14; text: "Koszt: " + (topSV ? topSV.cost : ""); color: "black" }

                MouseArea {
                    anchors.fill: parent; enabled: topSV !== null
                    cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                    onClicked: { gameManager.defeatSuperVillain() }
                }
            }
            Text { text: "Wszyscy pokonani!"; anchors.centerIn: parent; visible: topSV === null; color: "white" }
            Text {
                anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 8; font.bold: true; color: "white"
                text: "Pozostało: " + svCount
            }
        }

        Rectangle {
            id: weaknessStack
            width: 140
            height: 220
            color: "#2c3e50"
            radius: 8
            border.color: weaknessCount > 0 ? "#7f8c8d" : "#34495e"
            border.width: 2

            Text { text: "SŁABOŚĆ"; anchors.centerIn: parent; color: "white"; font.pixelSize: 20 }
             Text {
                anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 8; font.bold: true; color: "white"
                text: "Pozostało: " + weaknessCount
            }
        }
    }

    // --- Strefa zagranych kart ---
    Rectangle {
        id: playedArea
        width: centerAreaLayout.width
        anchors.horizontalCenter: centerAreaLayout.horizontalCenter
        height: 150; anchors.top: centerAreaLayout.bottom; anchors.topMargin: 20; color: "#2980b9"
        clip: true

        Text { text: "ZAGRANE KARTY"; anchors.top: parent.top; anchors.horizontalCenter: parent.horizontalCenter; anchors.topMargin: -20; font.pixelSize: 16; color: "white" }
        Flickable {
            anchors.fill: parent; contentWidth: playedRow.width; flickableDirection: Flickable.HorizontalFlick
            RowLayout {
                id: playedRow; height: parent.height
                Repeater {
                    model: activePlayer ? activePlayer.playedCards : []
                    delegate: Rectangle {
                        width: 90; height: 128; Layout.alignment: Qt.AlignVCenter; color: "#ecf0f1"; radius: 8; border.color: "#95a5a6"; border.width: 2
                        Text { anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right; anchors.margins: 4; font.bold: true; font.pixelSize: 12; text: name; color: "#2c3e50"; horizontalAlignment: Text.AlignHCenter; wrapMode: Text.WordWrap }
                        Text { anchors.centerIn: parent; font.pixelSize: 11; text: "Moc: " + power; color: "#2980b9" }
                        Text { anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottomMargin: 4; font.pixelSize: 11; text: "Koszt: " + cost; color: "#c0392b" }
                    }
                }
            }
        }
    }

    // --- Strefa Ręki Gracza ---
    Rectangle {
        id: handArea
        anchors.bottom: parent.bottom; anchors.left: parent.left; anchors.right: parent.right; height: 250; color: "#34495e"; border.color: "#2c3e50"
        Flickable {
            id: handFlickable; anchors.fill: parent; anchors.margins: 10; contentWidth: handRow.width; contentHeight: height; flickableDirection: Flickable.HorizontalFlick
            RowLayout {
                id: handRow; height: parent.height
                Repeater {
                    model: activePlayer ? activePlayer.hand : []
                    delegate: Rectangle {
                        width: 140; height: 200; Layout.alignment: Qt.AlignVCenter; color: "#ecf0f1"; radius: 8; border.color: "#95a5a6"; border.width: 2
                        Text { anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right; anchors.margins: 8; font.bold: true; font.pixelSize: 16; text: name; color: "#2c3e50"; horizontalAlignment: Text.AlignHCenter; wrapMode: Text.WordWrap }
                        Text { anchors.centerIn: parent; font.pixelSize: 14; text: "Moc: " + power; color: "#2980b9" }
                        Text { anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottomMargin: 8; font.pixelSize: 14; text: "Koszt: " + cost; color: "#c0392b" }
                        MouseArea {
                            anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                            onClicked: { gameManager.playCardFromHand(index) }
                        }
                    }
                }
            }
        }
    }
}
