import QtQuick 2.9
import QtQuick.Window 2.2
import com.github.arpegiuswhooves.SerialSignalLines 1.0

Window {
    visible: true
    width: 640
    height: 480


    RSPinoutSignals{
       id:rs1
       portName: "/dev/ttyS0"
       interval: 50
       intCount: 2
       onPinStatusChanged: {
            lines.itemAt(pin).color = state ? "green" : "red";
       }
    }

    Column{
        Text{
            text: "Active pins:"
        }
        Repeater {
            id: lines
            model: rs1.pinList
            Text {
                text: index +":"+modelData
            }
        }
    }

    title: qsTr("Hello World")
}
