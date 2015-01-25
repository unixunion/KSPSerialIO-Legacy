# KSPSerialIO
Modification of KSPSerialIO for Mac, *nix and Windows based off 0.17 of Zitron's github https://github.com/zitron-git/KSPSerialIO

This is a slight modification of Zitron's code base and *should* be cross-platform friendly. 

## About
This is a rewire and patch of zitron-git's KSPSerialIO to use my PsimaxSerial dll for hopefully cross-platform goodness. 

## Status
So far I have only tested this on Mac, on which it works quite well.

## Binaries
There are binaries in release subdir compiled on Mac, I have not tested if they work on any other platform.

## Baud Rates
Due to issues with .NET and Unity regarding Event callbacks, this version updates the Arduino via MonoBehavious->Update() which runs at about your screens refresh rate. Therefore please run your arduino at a hight speed like 115200 to avoid dragging down your system. 

## Config
I have added some config defaults to the code base, Please create a config file as below to override.

### NoDetect
NoDetect disable scanning of all connected serial devices for the KSPSerialIO Board. This forces attempted connection to the serial port mentioned in the config.

#### Example Config
GameData/KSPSerialIO/PluginData/KSPSerialIO/config.xml
```
  <?xml version="1.0" encoding="utf-8"?>
  <config>
      <string name="DefaultPort">/dev/tty.usbmodem411</string>
      <int name="BaudRate">115200</int>
      <int name="HandshakeDelay">400</int>
      <bool name="NoDetect">0</bool>
  </config>
```
