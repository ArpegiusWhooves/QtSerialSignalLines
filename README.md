# QtSerialSignalLines
Plugin for QML, to watch RS232 signal lines. 

# Install

Simply type:
```sh
qmake && sudo make install
```
You can also open pro file from QtCreator and add make deploy with argument `install`. 
# Usage
    
 The import statement:
```qml
import com.github.arpegiuswhooves.SerialSignalLines 1.0
```
And you can use the RSPinoutSignals object as follow:
```qml
RSPinoutSignals {
    id: rsport
    interval: 20 // how often check for status change in miliseconds
    
    intCount: 2  // how much positive reads make the pin stable
                 // 0 is disabled, 2 means three positive reads
                 
    portName: "/dev/ttyS0"   // device name, when set the port is automaticly opened 
    
    onOpenedChanged: {  // on the opened property change
        if( rsport.opened )  // you can check if port is opened 
           rsport.dataTerminalReady = true; // so you can change properties iherited from QSerialPort 
    }

    onPinStatusChanged: {
        console.log("PIN_"+pin,   // pins from 0 to 8
                    rsport.pinList[pin],  // array of pin names as in enum QSerialPort::PinoutSignal
                    "STATE CHANGED TO:",status); // status 0 or 1
    }
}
```
