import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Dialogs 1.0

import MergeMagicDatabase 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Katie's Merge Magie Editor")

    MergeMagicDatabase { id: db }

    Connections {
        target: Qt.application
        onStateChanged: {
            // when the app comes back from the background, force a reload just in case....
            if (Qt.application.state === Qt.ApplicationActive) {
                    db.reload();
            }
        }
    }

    Column
    {
        spacing: 10
        padding: 10

        Label {
            text: db.lastError
            font.pixelSize: 22
            font.bold: true
            color: "red"
        }

        GridLayout
        {
            columns: 2

            Label { text: "Compasses" }
            SpinBox {
                property bool hasUnsavedChange: value != db.compasses
                id: compasses
                value: db.compasses
                editable: true
                from: 0
                to: 999999
            }

            Label { text: "Coins" }
            SpinBox {
                property bool hasUnsavedChange: value != db.coins
                id: coins
                value: db.coins
                editable: true
                from: 0
                to: 999999
            }

            Label { text: "Wood Logs" }
            SpinBox {
                property bool hasUnsavedChange: value != db.woodLogs
                id: woodLogs;
                value: db.woodLogs
                editable: true
                from: 0
                to: 999999
            }

            Label { text: "Magic Gems" }
            SpinBox {
                property bool hasUnsavedChange: value != db.magicGems
                id: magicGems
                value: db.magicGems
                editable: true
                from: 0
                to: 999999
            }
        }

/*
        CheckBox {
            property bool hasUnsavedChange: checked != db.premiumLandPurchased
            id: premiumLandPurchased
            text: "Mark Premium Lands as purchased"
            checked: db.premiumLandPurchased

            // if premium land has been marked as pruchased and the change has
            // already been applied, disallow reverting it because it may screw
            // up the gamefile (not sure what would happen, but why take a chance?)
            enabled: !checked || (checked && hasUnsavedChange)
        }
*/

        RowLayout {

            Button {
                text: "Reload"
                onClicked: db.reload();
            }

            Button {
                text: "Save"
                enabled:
                    compasses.hasUnsavedChanges ||
                    coins.hasUnsavedChange ||
                    woodLogs.hasUnsavedChange ||
                    magicGems.hasUnsavedChange /*||
                    premiumLandPurchased.hasUnsavedChange*/

                onClicked:
                {
                    db.compasses = compasses.value;
                    db.coins = coins.value;
                    db.woodLogs = woodLogs.value;
                    db.magicGems = magicGems.value;
                    //db.premiumLandPurchased = premiumLandPurchased.checked;
                    db.save();
                }
            }
        }
    }

    Label {
        anchors.bottom: parent.bottom
        text: "© 2020 Super Hubby - All Rights Reserved"
        color: "grey"
        font.pixelSize: 22
    }

}
