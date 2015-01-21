# KSPSerialIO
Modification of KSPSerialIO for Mac, *nix and Windows

## About
This is a rewire and patch of zitron-git's KSPSerialIO to use my PsimaxSerial dll for hopefully cross-platform goodness. 

## Status
So far I have only tested this on Mac, on which it works quite well.

## Config
I have added some config defaults to the code base, Pleae create a config file as below to override.

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
