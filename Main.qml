import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

Window {
    id: root
    width: 1280
    height: 768
    visible: true
    title: "DeckBuilderPro v0.6 - Wybór Gracza (Finalna Poprawka)"
    color: "#2c3e50"

    // --- Właściwości lokalne (bufor dla QML) ---
    property var activePlayer: gameManager ? gameManager.currentPlayer : null
    property var lineUpCards: gameManager ? gameManager.lineUp : []
    property var topKick: gameManager ? gameManager.topOfKickStack : null
    property int kickCount: gameManager ? gameManager.kickStackSize : 0
    property var topSV: gameManager ? gameManager.topOfSuperVillainStack : null
    property int svCount: gameManager ? gameManager.superVillainStackSize : 0
    property int mainDeckCount: gameManager ? gameManager.mainDeckSize : 0
    property int weaknessCount: gameManager ? gameManager.weaknessStackSize : 0
    property var selectedIndices: []
    // Nowe, lokalne właściwości do sterowania stanem wyboru
    property bool isChoosing: gameManager ? gameManager.isPlayerChoosing : false
    property string choicePromptText: gameManager ? gameManager.choicePrompt : ""
    property int cardsToSelectCount: gameManager ? gameManager.cardsToDiscard : 0


    // --- Połączenia sygnałów ---
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
        function onGameStateChanged() {
            // Aktualizujemy wszystkie nasze lokalne właściwości na podstawie danych z C++
            isChoosing = gameManager.isPlayerChoosing
            choicePromptText = gameManager.choicePrompt
            cardsToSelectCount = gameManager.cardsToDiscard
            selectedIndices = []
        }
    }

    // --- Panel informacyjny gracza ---
    RowLayout {
        anchors.top: parent.top; anchors.horizontalCenter: parent.horizontalCenter; anchors.topMargin: 15; spacing: 20
        Text { font.pixelSize: 20; color: "white"; text: activePlayer ? "Tura gracza: " + activePlayer.name : "..." }
        Text { font.pixelSize: 20; color: "#f1c40f"; font.bold: true; text: activePlayer ? "Moc: " + activePlayer.currentPower : "Moc: 0" }
        Text { font.pixelSize: 16; color: "#bdc3c7"; text: activePlayer ? "Talia: " + activePlayer.deckSize : "" }
        Text { font.pixelSize: 16; color: "#bdc3c7"; text: activePlayer ? "Odrzucone: " + activePlayer.discardPileSize : "" }
        Button { text: "Zakończ Turę"; onClicked: { gameManager.endTurn() } }
    }

    // --- Centralna strefa stołu ---
    RowLayout {
        id: centerAreaLayout
        y: 80; anchors.horizontalCenter: parent.horizontalCenter; spacing: 20
        Rectangle {
            width: 140; height: 220; color: "#2c3e50"; border.color: "#3498db"; border.width: 2; radius: 8
            Text { text: "TALIA\nGŁÓWNA"; anchors.centerIn: parent; color: "white"; font.pixelSize: 20; horizontalAlignment: Text.AlignHCenter }
            Text { text: mainDeckCount; anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottomMargin: 10; font.pixelSize: 22; font.bold: true; color: "white" }
        }
        Rectangle {
            id: lineUpArea
            width: 780; height: 220; color: "transparent"
            RowLayout {
                anchors.fill: parent; spacing: 15
                Repeater {
                    model: lineUpCards
                    delegate: Item {
                        width: 140; height: 200; Layout.alignment: Qt.AlignVCenter
                        visible: modelData !== null
                        Image {
                            anchors.fill: parent
                            source: modelData ? appDirUrl.toString() + "/" + imagePath : ""
                            fillMode: Image.PreserveAspectFit
                            Rectangle { visible: parent.status !== Image.Ready; anchors.fill: parent; color: "#cccccc"; radius: 8; border.color: "#95a5a6"; border.width: 2 }
                        }
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

    // --- Strefa stosów specjalnych ---
    ColumnLayout {
        anchors.top: centerAreaLayout.top; anchors.left: centerAreaLayout.right; anchors.leftMargin: 20; spacing: 20
        Rectangle {
            id: kickStackContainer; width: 140; height: 220; color: "transparent"
            Image { id: kickImage; anchors.fill: parent; source: topKick ? appDirUrl.toString() + "/" + topKick.imagePath : ""; fillMode: Image.PreserveAspectFit
                Rectangle { visible: parent.status !== Image.Ready; anchors.fill: parent; color: "#2c3e50"; radius: 8; border.color: topKick ? "#95a5a6" : "#34495e"; border.width: 2 }
                Text { anchors.fill: parent; text: "Kopniaki\nwyczerpane"; color: "white"; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; visible: topKick === null }
            }
            Rectangle {
                anchors.bottom: kickImage.bottom; anchors.horizontalCenter: kickImage.horizontalCenter; anchors.bottomMargin: 5
                color: "#00000088"; radius: 3;
                width: kickCountText.implicitWidth + 10; height: kickCountText.implicitHeight + 4
                Text { id: kickCountText; anchors.centerIn: parent; font.bold: true; color: "white"; text: "Pozostało: " + kickCount }
            }
            MouseArea { anchors.fill: parent; enabled: topKick !== null; cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor; onClicked: { gameManager.buyKick() } }
        }
        Rectangle {
            id: svStackContainer; width: 140; height: 220; color: "transparent"
            Image { id: svImage; anchors.fill: parent; source: topSV ? appDirUrl.toString() + "/" + topSV.imagePath : ""; fillMode: Image.PreserveAspectFit
                Rectangle { visible: parent.status !== Image.Ready; anchors.fill: parent; color: "#2c3e50"; radius: 8; border.color: topSV ? "#c0392b" : "#34495e"; border.width: 2 }
                Text { text: "Wszyscy pokonani!"; anchors.centerIn: parent; visible: topSV === null; color: "white" }
            }
            Rectangle {
                anchors.bottom: svImage.bottom; anchors.horizontalCenter: svImage.horizontalCenter; anchors.bottomMargin: 5
                color: "#00000088"; radius: 3;
                width: svCountText.implicitWidth + 10; height: svCountText.implicitHeight + 4
                Text { id: svCountText; anchors.centerIn: parent; font.bold: true; color: "white"; text: "Pozostało: " + svCount }
            }
            MouseArea { anchors.fill: parent; enabled: topSV !== null; cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor; onClicked: { gameManager.defeatSuperVillain() } }
        }
        Rectangle {
            id: weaknessStack; width: 140; height: 220; color: "#2c3e50"; radius: 8; border.color: weaknessCount > 0 ? "#7f8c8d" : "#34495e"; border.width: 2
            Text { text: "SŁABOŚĆ"; anchors.centerIn: parent; color: "white"; font.pixelSize: 20 }
            Text { anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottomMargin: 8; font.bold: true; color: "white"; text: "Pozostało: " + weaknessCount }
        }
    }

    // --- Strefa zagranych kart ---
    Rectangle {
        id: playedArea; width: centerAreaLayout.width; anchors.horizontalCenter: centerAreaLayout.horizontalCenter; height: 150; anchors.top: centerAreaLayout.bottom; anchors.topMargin: 40; color: "transparent"; clip: true
        Text { text: "ZAGRANE KARTY"; anchors.top: parent.top; anchors.horizontalCenter: parent.horizontalCenter; anchors.topMargin: -20; font.pixelSize: 16; color: "white" }
        Flickable {
            anchors.fill: parent; contentWidth: playedRow.width; flickableDirection: Flickable.HorizontalFlick
            RowLayout {
                id: playedRow; height: parent.height; spacing: 5
                Repeater {
                    model: activePlayer ? activePlayer.playedCards : []
                    delegate: Item {
                        width: 90; height: 128; Layout.alignment: Qt.AlignVCenter
                        Image { anchors.fill: parent; source: appDirUrl.toString() + "/" + imagePath; fillMode: Image.PreserveAspectFit
                            Rectangle { visible: parent.status !== Image.Ready; anchors.fill: parent; color: "#cccccc"; radius: 8; border.color: "#95a5a6"; border.width: 2 }
                        }
                    }
                }
            }
        }
    }

    // --- Strefa Ręki Gracza ---
    Rectangle {
        id: handArea; anchors.bottom: parent.bottom; anchors.left: parent.left; anchors.right: parent.right; height: 250; color: "#34495e"; border.color: "#2c3e50"
        Flickable {
            id: handFlickable; anchors.fill: parent; anchors.margins: 10; contentWidth: handRow.width; contentHeight: height; flickableDirection: Flickable.HorizontalFlick
            RowLayout {
                id: handRow; height: parent.height; spacing: 10
                Repeater {
                    id: handRepeater
                    model: activePlayer ? activePlayer.hand : []
                    delegate: Item {
                        id: handCardItem
                        width: 140; height: 200; Layout.alignment: Qt.AlignVCenter
                        property bool isSelectedForDiscard: selectedIndices.indexOf(index) !== -1
                        Image {
                            anchors.fill: parent; source: appDirUrl.toString() + "/" + imagePath; fillMode: Image.PreserveAspectFit
                        }
                        Rectangle {
                            anchors.fill: parent; color: "transparent"; border.color: handCardItem.isSelectedForDiscard ? "gold" : "transparent"; border.width: 4; radius: 8
                        }
                        MouseArea {
                            anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (isChoosing) {
                                    var idx = selectedIndices.indexOf(index);
                                    if (idx === -1) {
                                        if (selectedIndices.length < cardsToSelectCount) {
                                            selectedIndices.push(index)
                                        }
                                    } else {
                                        selectedIndices.splice(idx, 1)
                                    }
                                    selectedIndices = selectedIndices.slice();
                                } else {
                                    gameManager.playCardFromHand(index)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // --- Nakładka do podejmowania decyzji ---
    Rectangle {
        id: choiceOverlay
        anchors.fill: parent; color: "#000000AA"; visible: isChoosing; z: 10
        ColumnLayout {
            anchors.centerIn: parent; spacing: 20
            Text {
                color: "white"; font.pixelSize: 24
                // POPRAWKA: Bindowanie do bezpiecznej, lokalnej właściwości
                text: choicePromptText
            }
            Button {
                text: "Potwierdź"
                Layout.alignment: Qt.AlignHCenter
                // POPRAWKA: Bindowanie do bezpiecznej, lokalnej właściwości
                enabled: selectedIndices.length === cardsToSelectCount
                onClicked: {
                    gameManager.resolveDiscard(selectedIndices)
                }
            }
        }
    }
}
