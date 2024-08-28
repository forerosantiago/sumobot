#include <Arduino.h>
#include <IRremote.h>
#include <Servo.h>
#include <NewPing.h>


int IrRight = 2;
int IrLeft = 3;

int ir = 4;
//int servoRight = 5;
//int servoLeft = 6;

int echo = 16;
int trigger = 17;
int maxDistance = 40;


Servo servoRight;
Servo servoLeft;
NewPing sonar(trigger, echo, maxDistance);


bool combatMode = false;

void Evasion();
void Parado();
void GirarDerecha();
void Adelante();

void setup() {
  pinMode(IrRight, INPUT); 
  pinMode(IrLeft, INPUT);
  pinMode(echo, INPUT);   
  pinMode(trigger, OUTPUT); 
  
  servoRight.attach(6);     
  servoLeft.attach(5); 
  
  Serial.begin(9600);

  IrReceiver.begin(ir, ENABLE_LED_FEEDBACK);
  Serial.println("Waiting for command...");
}

void loop() {
  if(IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    switch (IrReceiver.decodedIRData.decodedRawData) {
      case 0xFD020707: // power button
        combatMode = !combatMode; 
        if (combatMode) {
          Serial.println("CombatMode ON");
        } else if(!combatMode) {
          Serial.println("CombatMode OFF");
        }
      break;
    
      default:
      break;
    }
    IrReceiver.resume();
  }

  
  //uS = sonar.ping();
  //cm = sonar.convert_cm(uS);
  //Serial.println(cm);

  if(combatMode) {
    if(!digitalRead(IrRight) || !digitalRead(IrLeft)) {
      Evasion();
    }

    if(sonar.ping_cm() != 0) {//cambiar medida importante
      Adelante();
    } else {
      //girarderecha (buscar oponente)
      GirarDerecha();
    }
  } else {
    Parado();
  }
}





void GirarDerecha() {
  Serial.println("girando derecha y Buscando Enemigo...");
  servoRight.write(120);
  servoLeft.write(120);
  delay(50);
}
void GirarIzquierda() {
  servoRight.write(0);
  servoLeft.write(0);
  delay(50);
}
void Adelante() {
  Serial.println("ATACANDO");
  servoRight.write(0);
  servoLeft.write(180);
  delay(100);
}

void Atras() {
  servoRight.write(180);
  servoLeft.write(0);
  delay(50);
}

void Parado() {
  servoRight.write(90);
  servoLeft.write(90);
  delay(50);
}

void Evasion() {
  Parado();
  delay(100);
  Serial.println("Evasion");
  Atras();
  delay(1000);
}