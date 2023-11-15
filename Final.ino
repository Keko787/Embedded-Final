#include <Servo.h>
#include "ultrasonic.h"
#include "motor.h"

// Global constants

const byte ULTRASONIC_ECHO = A0;
const byte ULTRASONIC_TRIGGER = A1;
const byte SERVO_PIN = A2;
const byte IR_PIN = 12;

const byte MOTOR_RIGHT_ENABLE = 5;
const byte MOTOR_RIGHT_FORWARD = 4;
const byte MOTOR_RIGHT_BACKWARD = 2;

const byte MOTOR_LEFT_ENABLE = 6;
const byte MOTOR_LEFT_FORWARD = 8;
const byte MOTOR_LEFT_BACKWARD = 7;

class Ultrasonic {
    private:
        byte _triggerPin;
        byte _echoPin;
        const unsigned long _maxDuration = 12 * 74 * 2;

    public:
        Ultrasonic();
        Ultrasonic(byte);
        Ultrasonic(byte, byte);
        unsigned long measureTime();
        double measureInch();
        double measureCm();
}; // end ultrasonic

void setup() {

    Serial.begin(9600);
    pinMode(SIG_PIN, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(SIG_PIN, LOW);
    digitalWrite(TRIG_PIN, LOW);

    TCNT1H = 0xC2;
    TCNT1L = 0xF7;
    TCCR1A = 0x00;
    TCCR1B = 0x05;
    TIMSK1 = (1 << TOIE1);

    interrupts();
}

void loop() {

}

Ultrasonic::Ultrasonic() {
    _triggerPin = 0;
    _echoPin = 0;
}

Ultrasonic::Ultrasonic(byte pinNum) {
    _triggerPin = pinNum;
    _echoPin = pinNum;
}

Ultrasonic::Ultrasonic(byte sigPin, byte trigPin) {
    _triggerPin = sigPin;
    _echoPin = trigPin;
}

unsigned long Ultrasonic::measureTime() {
    noInterrupts();

    unsigned long duration = 0;

    pinMode(_echoPin, OUTPUT);
    pinMode(_triggerPin, OUTPUT);

    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(5);
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);

    pinMode(_echoPin, INPUT);
    duration = pulseIn(_echoPin, maxDuration);

    interrupts();

    if (duration > 0) {
        duration /= 2;
    }
    return duration;
}

double Ultrasonic::measureInch() {
    double inches = 0;
    unsigned long duration = measureTime();
    if (duration > 0) {
        inches = duration / 74;
    }
    interrupted = false;
    return inches;
}

double Ultrasonic::measureCm() {
    double centimeters = 0;
    unsigned long duration = measureTime();
    if (duration > 0) {
        centimeters = duration / 29.1;
    }
    interrupted = false;
    return centimeters;
}

ISR(TIMER_OCF_vect) {
    interrupted = true;
}
