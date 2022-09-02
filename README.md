# SpaceShipSimPitControl
Simpit controls connecing several arduino devices (through i2c), computer and arduinos (through serial and json).

The code consists of several independent part and are intended to be used with several arduinos, making each "sub-controller" work of one arduino that communicate with a central "data broker". The data broker reads data from the game (in this case Elite Dangerous, but that should be relatively easy to change), and communicates this to the sub-controllers that need the info. It also reads data, such as button presses, from the sub-controllers and forwards this to the computer in the form of game controller inputs.

The data from the game (Elite Dangerous) is interpreted using a Node-Red server listening to changes in the status-files, and then forwarding this info in the form of simplified json (it only reads key-value-pairs) to the arduino over Serial.

The project is intended as a way of sharing the specifics from a SimPit development process over on http://www.youtube.com/JonBck/