#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4);

#define VOLTAGE_IN_PIN A1       //Voltage Sensor
#define CURRENT_IN_PIN A0       //Current Sensor

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 8
#define S1 11
#define S2 12
#define S3 13
#define sensorOut 10

// Stores frequency read by the photodiodes
int red = 0;
int green = 0;
int blue = 0;

//For Voltage Sensor
float adc_voltage = 0.0;
float in_voltage = 0.0;
float R1 = 30000.0;             // For resistor values in divider (in ohms)
float R2 = 7500.0; 
float ref_voltage = 5.0;        // For Reference Voltage
int adc_value = 0;              // Integer for ADC value

//For Current Sensor
double Vout = 0;                // Variables for Measured Voltage and Calculated Current
double Current = 0;
const double scale_factor = 0.1;// 20A scaling factor
const double vRef = 5.00;       // Constants for A/D converter resolution
const double resConvert = 1024; // Arduino has 10-bit ADC, so 1024 possible values
double resADC = vRef/resConvert;// Reference voltage is 5V if not using AREF external reference
double zeroPoint = vRef/2;      // Zero point is half of Reference Voltage

void setup(){
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);

  // Setup Serial Monitor
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  Serial.println("DC Voltage & Current Test & Colour Detection");
}
 
void loop(){
  adc_value = analogRead(VOLTAGE_IN_PIN);                            // Read the Analog Voltage Input
  adc_voltage  = (adc_value * ref_voltage) / 1024.0;                // Determine voltage at ADC input
  in_voltage = adc_voltage / (R2/(R1+R2));                          // Calculate voltage at divider input

  for(int i = 0; i < 1000; i++) {                                   // Vout is read 1000 Times for precision
    Vout = (Vout + (resADC * analogRead(CURRENT_IN_PIN)));   
    delay(1);
  }
  Vout = Vout /1000;                                                // Get Vout in mv
  Current = (Vout - zeroPoint)/ scale_factor;                       // Convert Vout into Current using Scale Factor

  color();
  lcd.clear();

  if(red<blue && red<green && red<25){
    Serial.print("Input Voltage = ");                                 // Print results of Voltage Sensor in Serial Monitor
    Serial.println(in_voltage, 2);                   
    Serial.print("\t Current = ");                                    // Print results of Current Sensor in Serial Monitor
    Serial.print(Current,2);
    Serial.println(" Amps");
    Serial.print("Color detected : RED\n\tFrequency: ");
    Serial.println(red);

    lcd.setCursor(0, 0);
    lcd.print("V:");
    lcd.print(in_voltage);
    lcd.print("v ");
    lcd.print("C:");
    lcd.print(Current);
    lcd.print("A");
    lcd.setCursor(0, 1);
    lcd.print("Color:RED");
    delay(100);
  }
  else if(blue < red && blue < green && blue<25) {
    Serial.print("Input Voltage = ");                                 // Print results of Voltage Sensor in Serial Monitor
    Serial.println(in_voltage, 2);                   
    Serial.print("\t Current = ");                                    // Print results of Current Sensor in Serial Monitor
    Serial.print(Current,2);
    Serial.println(" Amps");
    Serial.print("Color detected : RED\n\tFrequency: ");
    Serial.println(red);
    Serial.print("Color detected : BLUE\n\tFrequency: ");
    Serial.println(blue);

    lcd.setCursor(0, 0);
    lcd.print("V:");
    lcd.print(in_voltage);
    lcd.print("v ");
    lcd.print("C:");
    lcd.print(Current);
    lcd.print("A");
    lcd.setCursor(0, 1);
    lcd.print("Color:BLUE");
    delay(100);
  }
  else if (green < red && green < blue && green<25) {
    Serial.print("Input Voltage = ");                                 // Print results of Voltage Sensor in Serial Monitor
    Serial.println(in_voltage, 2);                   
    Serial.print("\t Current = ");                                    // Print results of Current Sensor in Serial Monitor
    Serial.print(Current,2);
    Serial.println(" Amps");
    Serial.print("Color detected : RED\n\tFrequency: ");
    Serial.println(red);
    Serial.print("Color detected : GREEN\n\tFrequency: ");
    Serial.println(green);

    lcd.setCursor(0, 0);
    lcd.print("V:");
    lcd.print(in_voltage);
    lcd.print("v ");
    lcd.print("C:");
    lcd.print(Current);
    lcd.print("A");
    lcd.setCursor(0, 1);
    lcd.print("Color:GREEN");
    delay(100);
  }  
  else {
    lcd.setCursor(0, 0);
    lcd.print("V:");
    lcd.print(in_voltage);
    lcd.print("v ");
    lcd.print("C:");
    lcd.print(Current);
    lcd.print("A");
    lcd.setCursor(0, 1);
    lcd.print("NO Color");

    Serial.print("Input Voltage = ");                                 // Print results of Voltage Sensor in Serial Monitor
    Serial.println(in_voltage, 2);                   
    Serial.print("\t Current = ");                                    // Print results of Current Sensor in Serial Monitor
    Serial.print(Current,2);
    Serial.println(" Amps");
    Serial.print("Color detected : RED\n\tFrequency: ");
    Serial.println(red);
    Serial.print("NO COLOR DETECTED\n");
    Serial.println(red);
    Serial.println(blue);
    Serial.println(green);
    delay(10);
  }

  delay(100);
}

void color() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  red = pulseIn(sensorOut, LOW);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  green = pulseIn(sensorOut, LOW);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  blue = pulseIn(sensorOut, LOW);
  delay(100);
}