#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <u8glib.h>
#include <pgfx.h>
#include <MemoryFree.h>

// OLED Display
#define OLED_MOSI  1
#define OLED_CLK   2
#define OLED_DC    4
#define OLED_CS    3
#define OLED_RESET 5
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
unsigned int OLED_REFRESH = 200;
unsigned int LAST_OLED_REFRESH; 

// Gauges
pgfx_HBAR fbar = pgfx_HBAR( 0, 0,20,64,"FUEL",display);
pgfx_HBAR mbar = pgfx_HBAR(26, 0,20,64,"MONO",display);
pgfx_HBAR ebar = pgfx_HBAR(52, 0,20,64,"ELEC",display);
pgfx_HBAR gbar = pgfx_HBAR(78, 0,20,64,"G's",display);

//pins for LEDs
#define GLED 7
#define YLED 7
#define RLED 7
#define SASLED 13
#define RCSLED 12
#define CG1LED 11

//pins for input
#define SASPIN 6
#define RCSPIN 7
#define CG1PIN 8
#define THROTTLEPIN 0

#define THROTTLEDB 4 //Throttle axis deadband

//Input enums
#define SAS 7
#define RCS 6
#define LIGHTS 5
#define GEAR 4
#define BRAKES 3
#define PRECISION 2
#define ABORT 1
#define STAGE 0

//Action group statuses
#define AGSAS      0
#define AGRCS      1       
#define AGLight    2 
#define AGGear     3
#define AGBrakes   4 
#define AGAbort    5 
#define AGCustom01 6
#define AGCustom02 7 
#define AGCustom03 8 
#define AGCustom04 9 
#define AGCustom05 10
#define AGCustom06 11 
#define AGCustom07 12 
#define AGCustom08 13 
#define AGCustom09 14 
#define AGCustom10 15

//macro 
#define details(name) (uint8_t*)&name,sizeof(name)

//if no message received from KSP for more than 2s, go idle
#define IDLETIMER 2000
#define CONTROLREFRESH 25

//warnings
#define GWARN 9                  //9G Warning
#define GCAUTION 5               //5G Caution
#define FUELCAUTION 10.0         //10% Fuel Caution
#define FUELWARN 5.0             //5% Fuel warning

unsigned long deadtime, deadtimeOld, controlTime, controlTimeOld;
unsigned long now;

boolean Connected = false;

byte caution = 0, warning = 0, id;

struct VesselData
{
  byte id;                //1
  float AP;               //2
  float PE;               //3
  float SemiMajorAxis;    //4
  float SemiMinorAxis;    //5
  float VVI;              //6
  float e;                //7
  float inc;              //8
  float G;                //9
  long TAp;               //10
  long TPe;               //11
  float TrueAnomaly;      //12
  float Density;          //13
  long period;            //14
  float RAlt;             //15
  float Alt;              //16
  float Vsurf;            //17
  float Lat;              //18
  float Lon;              //19
  float LiquidFuelTot;    //20
  float LiquidFuel;       //21
  float OxidizerTot;      //22
  float Oxidizer;         //23
  float EChargeTot;       //24
  float ECharge;          //25
  float MonoPropTot;      //26
  float MonoProp;         //27
  float IntakeAirTot;     //28
  float IntakeAir;        //29
  float SolidFuelTot;     //30
  float SolidFuel;        //31
  float XenonGasTot;      //32
  float XenonGas;         //33
  float LiquidFuelTotS;   //34
  float LiquidFuelS;      //35
  float OxidizerTotS;     //36
  float OxidizerS;        //37
  uint32_t MissionTime;   //38
  float deltaTime;        //39
  float VOrbit;           //40
  uint32_t MNTime;        //41
  float MNDeltaV;         //42
  float Pitch;            //43  
  float Roll;             //44  
  float Heading;          //45
  uint16_t ActionGroups;  //46 status bit order:SAS, RCS, Light, Gear, Brakes, Abort, Custom01 - 10
};

struct HandShakePacket
{
  byte id;
  byte M1;
  byte M2;
  byte M3;
};

struct ControlPacket {
  byte id;
  byte MainControls;                  //SAS RCS Lights Gear Brakes Precision Abort Stage 
  byte Mode;                          //0 = stage, 1 = docking, 2 = map
  unsigned int ControlGroup;          //control groups 1-10 in 2 bytes
  byte AdditionalControlByte1;        //other stuff
  byte AdditionalControlByte2;
  int Pitch;                          //-1000 -> 1000
  int Roll;                           //-1000 -> 1000
  int Yaw;                            //-1000 -> 1000
  int TX;                             //-1000 -> 1000
  int TY;                             //-1000 -> 1000
  int TZ;                             //-1000 -> 1000
  int WheelSteer;                     //-1000 -> 1000
  int Throttle;                       //    0 -> 1000
  int WheelThrottle;                  //    0 -> 1000
};

HandShakePacket HPacket;
VesselData VData;
ControlPacket CPacket;

void setup(){
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,16);
  display.println("boot...");
  initLEDS();
  InitTxPackets();
  controlsInit();
  LEDSAllOff();
}

void loop()
{  
  input();
  output();
  
  now = millis();
  if ((now - LAST_OLED_REFRESH) > OLED_REFRESH) {
     updateHUD();
//     display.setCursor(96,5);
//     display.println(freeMemory());
//     display.setCursor(96,16);
//     display.println(OLED_REFRESH);
     display.display();
     LAST_OLED_REFRESH = millis(); // re-record now
  }
}


















