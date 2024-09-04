#include <Arduino.h>
#include <IRremote.h>
#include <Servo.h>
#include <NewPing.h>

#define leftIR 2
#define rightIR 3

#define ir 12

#define echo 16
#define trigger 17

Servo rightServo;
Servo leftServo;

NewPing sonar(trigger, echo, 100);

void setup() {
    Serial.begin(9600);

    pinMode(leftIR, INPUT);
    pinMode(rightIR, INPUT);
    pinMode(ir, INPUT);

    rightServo.attach(5);
    leftServo.attach(6);

    IrReceiver.begin(ir, ENABLE_LED_FEEDBACK);

}

bool combatMode = false;
bool infosensores = false;
int distance;
void loop() {
    if(IrReceiver.decode()) {
        switch (IrReceiver.decodedIRData.decodedRawData) {
            case 0xFD020707: //power
                combatMode = true;
                Serial.println("CombatMode ON");
                break;

            case 0xFE010707: //source button
                combatMode = false;
                Serial.println("CombatMode OFF");
                break;

            case 0x9A650707: //left arrow
                Serial.println("Probando servo izquierdo adelante");
                leftServo.write(180);
                break;

            case 0xF7080707: //number 4
                Serial.println("Probando servo izquierdo atrás");
                leftServo.write(0);
                break;

            case 0x9D620707: //right arrow
                Serial.println("Probando servo derecho adelante");
                rightServo.write(0);
                break;

            case 0xF50A0707: //number 6
                Serial.println("Probando servo derecho atrás");
                rightServo.write(180);
                break;

            case 0x936C0707: //A rojo
                Serial.println("Todos los motores adelante");
                rightServo.write(0);
                leftServo.write(180);
                break;

            case 0xEB140707: //B verde
                Serial.println("Todos los motores en reversa");
                rightServo.write(180);
                leftServo.write(0);
                break;

            case 0xEA150707: //c amarillo
                Serial.println("Girando a la izquierda");
                rightServo.write(0);
                leftServo.write(0);
                break;

            case 0xE9160707: //d azul
                Serial.println("Girando a la derecha");
                rightServo.write(180);
                leftServo.write(180);
                break;

            case 0xD22D0707: //exit
                Serial.println("PARANDO TODOS LOS MOTORES y apagando modo combate");
                leftServo.write(90);
                rightServo.write(90);
                combatMode = false;
                infosensores = false;
                break;

            case 0xE01F0707: //info
                //infosensores
                infosensores=true;

            default:
                Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
                break;
        }
    IrReceiver.resume();

    }

    if(combatMode) {
        if(!digitalRead(rightIR) || !digitalRead(leftIR)) {
            //estoy pisando algo blanco, voy en reversa
            leftServo.write(0);
            rightServo.write(180);
            Serial.println("pisando blanco");
        } else {
            //estoy pisando negro, estoy seguro, sigo girando para buscar y luego atacar
            distance = sonar.ping_cm();
            Serial.print("Distance: ");
            Serial.print(distance);
            Serial.println(" cm");

            if (distance == 0) {
                // no se encunetra objeto sigo girando
                Serial.println("Girando a la izquierda");
                rightServo.write(0);
                leftServo.write(0);
            } else {
                //tengo algo alfrente, entonces ataco
                rightServo.write(0);
                leftServo.write(180);
            }
        }
    } else {
        leftServo.write(90);
        rightServo.write(90);
    }

    if(infosensores) {
        distance = sonar.ping_cm();
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");


        Serial.print("IRright: ");
        if(digitalRead(rightIR)) {
            Serial.println("negro");
        } else {
            Serial.println("blanco");
        }
        
        Serial.print("IRleft: ");
        if(digitalRead(leftIR)) {
            Serial.println("negro");
        } else {
            Serial.println("blanco");
        }

        Serial.println("--------------------");
        delay(200);
    }
}


// if ir == false --> white

// if ir == true --> black
