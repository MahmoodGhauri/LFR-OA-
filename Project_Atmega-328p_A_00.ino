#include <SPI.h>
#include <SD.h>
File myFile;

#define trigPin A2
#define echoPin A3

//Line Tracking IO define
#define LT_R !digitalRead(A0)
#define LT_M !digitalRead(A1)
#define LT_L !digitalRead(A4)
 
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 10
 
#define carSpeed_R 80
#define carSpeed_L 80
 
float duration, distance;

void forward(){
  analogWrite(ENA, carSpeed_R);
  analogWrite(ENB, carSpeed_L);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go forward!");
}
 
void left(){
  analogWrite(ENA, carSpeed_R);
  analogWrite(ENB, carSpeed_L);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("go left!");
}
 
void right(){
  analogWrite(ENA, carSpeed_R);
  analogWrite(ENB, carSpeed_L);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 
  Serial.println("go right!");
} 
 
void stop(){
   analogWrite(ENA, carSpeed_R);
   analogWrite(ENB, carSpeed_L);
   digitalWrite(IN1, LOW);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, LOW);
   Serial.println("Stop!");
} 

void get_distance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0343;
}

void isr() //interrupt service routine
{
  stop();
  delay(3000);
}

void setup(){
  Serial.begin(9600);
  pinMode(LT_R,INPUT);
  pinMode(LT_M,INPUT);
  pinMode(LT_L,INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  while (!Serial) {;}
  Serial.print("Initializing SD card...");
  if (!SD.begin(2)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  attachInterrupt(1,isr,RISING);  //attaching the interrupt
}
 
void loop() {
  get_distance();
  if(distance<=6){
    stop();

    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
      Serial.print("Writing to test.txt...");
      myFile.println("S");
      Serial.println("done.");
    } 
    else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
    }

  }
  else if(LT_M){
    forward();

    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
      Serial.print("Writing to test.txt...");
      myFile.println("F");
      Serial.println("done.");
    } 
    else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
    }

  }
  else if(LT_R) { 
    right();

    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
      Serial.print("Writing to test.txt...");
      myFile.println("R");
      Serial.println("done.");
    } 
    else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
    }

    while(LT_R);                             
  }   
  else if(LT_L) {
    left();

    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
      Serial.print("Writing to test.txt...");
      myFile.println("L");
      Serial.println("done.");
    } 
    else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
    }

    while(LT_L);  
  }
  else{
    stop();
    
    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
      Serial.print("Writing to test.txt...");
      myFile.println("S");
      Serial.println("done.");
    } 
    else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
    }

  }
  myFile.close();
}