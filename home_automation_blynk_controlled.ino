// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID       "TMPL3PPZSsdhm"
#define BLYNK_TEMPLATE_NAME     "Home Automation"
#define BLYNK_AUTH_TOKEN        "ZVhA4XhXsmkBBkXCbBufk17Emki3UAEU"

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "temperature_system.h"
#include "ldr.h"
#include "serial_tank.h"

char auth[] = BLYNK_AUTH_TOKEN;
bool heater_sw,inlet_sw,outlet_sw, cooler_sw;
unsigned int tank_volume;

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//To turn ON and OFF cooler based virtual PIN value
BLYNK_WRITE(COOLER_V_PIN)
{ 
  cooler_sw = param.asInt();
  if(cooler_sw)
  {
    cooler_control(ON);
    lcd.setCursor(8, 0);
    lcd.print("CLR_ON ");
  }
  else
  {
    cooler_control(OFF);
    lcd.setCursor(8, 0);
    lcd.print("CLR_OFF");
  }
}

//To turn ON and OFF heater based virtual PIN value
BLYNK_WRITE(HEATER_V_PIN )
{ 
  heater_sw = param.asInt();
  if(heater_sw)
  {
    heater_control(ON);
    lcd.setCursor(8, 0);
    lcd.print("HTR_ON ");
  }
  else
  {
    heater_control(OFF);
    lcd.setCursor(8, 0);
    lcd.print("HTR_OFF ");
  }
}

//To turn ON and OFF inlet valve based virtual PIN value
BLYNK_WRITE(INLET_V_PIN)
{
  inlet_sw = param.asInt();
  if(inlet_sw)
  {
    enable_inlet();
    lcd.setCursor(8, 1);
    lcd.print("IN_FL_ON ");
  }
  else
  {
    disable_inlet();
    lcd.setCursor(8, 1);
    lcd.print("IN_FL_OFF");
  }
}

//To turn ON and OFF outlet valve based virtual PIN value
BLYNK_WRITE(OUTLET_V_PIN)
{
  outlet_sw = param.asInt();
  if(outlet_sw)
  {
    enable_outlet();
    lcd.setCursor(8, 1);
    lcd.print("OUT_FL_ON ");
  }
  else
  {
    disable_outlet();
    lcd.setCursor(8, 1);
    lcd.print("OUT_FL_OFF");
  }
}

// To display temperature as gauge on the Blynk App
void update_temperature_reading()
{
  Blynk.virtualWrite(TEMPERATURE_GAUGE, read_temperature());
}

void update_water_reading()
{
  Blynk.virtualWrite(WATER_VOL_GAUGE, tank_volume);
}

//To turn off the heater if the temperature raises above 35 degree Celsius
void handle_temp(void)
{
  if(read_temperature() > (float) 35 && heater_sw)
  {
    heater_sw = 0;
    heater_control(OFF);
    Blynk.virtualWrite(HEATER_V_PIN, 0);
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Temperature is more than 35 degree Celsius\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning off the Heater\n");
    lcd.setCursor(8, 0);
    lcd.print("HTR_OFF");
  }
}

//To control water volume above 2000 ltrs
void handle_tank(void)
{
  if(tank_volume < 2000 && (inlet_sw==0))
  {
    enable_inlet();
    inlet_sw = 1;
    Blynk.virtualWrite(INLET_V_PIN, 1);
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Water Volume is less than 2000 liters\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning on the Inlet Valve\n");
    lcd.setCursor(8, 1);
    lcd.print("IN_FL_ON ");
  }

  if(tank_volume == 3000 && (inlet_sw==1))
  {
    disable_inlet();
    inlet_sw = 0;
    Blynk.virtualWrite(INLET_V_PIN, 0);
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Water Volume is 3000 liters\n");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning off the Inlet Valve\n");
    lcd.setCursor(8, 1);
    lcd.print("IN_FL_OFF");
  }
}


void setup(void)
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.home();

  lcd.setCursor(0,0);
  lcd.print("Home Automation");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T=");
  lcd.setCursor(0,1);
  lcd.print("V=");
  
  Blynk.begin(BLYNK_AUTH_TOKEN);
  init_ldr();
  init_temperature_system(); 
  init_serial_tank(); 
  
  timer.setInterval(1000, update_temperature_reading);
  timer.setInterval(1000, update_water_reading);

}

String temp;

void loop(void) 
{
  brightness_control(); 
  temp = String(read_temperature(),2 ); 
  lcd.setCursor(2,0);
  lcd.print(temp); 

  tank_volume = volume();
  lcd.setCursor(2,1);
  lcd.print(tank_volume);

  Blynk.run();
  timer.run();
  
  handle_temp();
  handle_tank();
}
