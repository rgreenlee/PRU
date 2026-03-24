import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import PRU 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: "PRU PWM Control"

    PruController {
        id: pru
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        Label {
            text: "PRU PWM Control Panel"
            font.pixelSize: 28
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#666666"
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 20

            Label {
                text: "Enable Output"
                font.pixelSize: 18
            }

            Switch {
                checked: pru.enabled
                onToggled: pru.enabled = checked
            }

            Label {
                text: pru.running ? "PRU Running" : "PRU Stopped"
                color: pru.running ? "limegreen" : "tomato"
                font.pixelSize: 18
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Label {
                text: "Frequency: " + pru.frequencyHz + " Hz"
                font.pixelSize: 18
            }

            Slider {
                Layout.fillWidth: true
                from: 1
                to: 5000
                value: pru.frequencyHz
                onMoved: pru.frequencyHz = Math.round(value)
                onValueChanged: pru.frequencyHz = Math.round(value)
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Label {
                text: "Duty Cycle: " + pru.dutyPercent + " %"
                font.pixelSize: 18
            }

            Slider {
                Layout.fillWidth: true
                from: 1
                to: 99
                value: pru.dutyPercent
                onMoved: pru.dutyPercent = Math.round(value)
                onValueChanged: pru.dutyPercent = Math.round(value)
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            Button {
                text: "Apply"
                onClicked: pru.applySettings()
            }

            Button {
                text: "Start PRU"
                onClicked: pru.startFirmware()
            }

            Button {
                text: "Stop PRU"
                onClicked: pru.stopFirmware()
            }

            Button {
                text: "Refresh"
                onClicked: pru.refreshState()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#666666"
        }

        Label {
            text: pru.statusText
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        Item {
            Layout.fillHeight: true
        }

        Label {
            text: "Note: run this app as root on the BBB so it can access /dev/mem and remoteproc sysfs."
            font.pixelSize: 14
            color: "lightgray"
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }
    }
}