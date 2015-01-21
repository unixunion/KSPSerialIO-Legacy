# KSPSerialIO
Kerbal Space Program Serial Port IO plugin

## Issues

scanning for the KSPboard is not currently working, please set config.xml up with the NoDetect bool to force communications with a specified port.

### GameData/KSPSerialIO/PluginData/KSPSerialIO/config.xml
```
  <?xml version="1.0" encoding="utf-8"?>
  <config>
      <string name="DefaultPort">/dev/tty.usbmodem411</string>
      <int name="BaudRate">115200</int>
      <int name="HandshakeDelay">0</int>
      <bool name="NoDetect">1</bool>
  </config>
```
