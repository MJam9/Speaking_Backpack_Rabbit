#include <Arduino.h>
#include <ESP32Servo.h>
#include "DFRobotDFPlayerMini.h"//Download hier zu finden: https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <esp_sleep.h>
#include "driver/gpio.h"

// Enable IR debugging
#define DEBUG 1

// Servo-Objekt
Servo servoRechts;
Servo servoLinks;

// DFPlayer Mini
HardwareSerial mySerial(2);
DFRobotDFPlayerMini myDFPlayer;

const int servorechtsPin = 27;  // Signal-Pin für das Servo (GPIO 18 alt)
const int servolinksPin = 26;  // Signal-Pin für das Servo (GPIO 19 alt)

// IR Remote
const uint16_t kRecvPin = 4;  // Dein Signalpin
IRrecv irrecv(kRecvPin);
decode_results results;

// Bewegungs-Definitionen
struct Bewegung {
  const int (*sequenz)[2]; // Zeiger auf 2-Spalten-Array
  int laenge;              // Anzahl Positionen
  int delayMs;             // Zeit zwischen Schritten
};

// Sequenzen
int tanzen[11][2] = {
  {60, 60},
  {80, 80},
  {100, 100},
  {120, 120},
  {140, 140},
  {160, 160},
  {140, 140},
  {120, 120},
  {100, 100},
  {80, 80},
  {60, 60}
};

int winken[3][2] = {
  {60, 100},
  {160, 100},
  {60, 100}
};

// Einzel-Positionen (als 1-Schritt-Sequenz)
int pos1[1][2] = { {60, 30} };
int pos2[1][2] = { {100, 100} };

// Bewegungen-Liste
Bewegung bewegungen[] = {
  { pos1, 1, 500 },    // feste Position
  { pos2, 1, 500 },    // feste Position
  { winken, 3, 400 },  // Winken mit 300 ms
  { tanzen, 11, 200 }   // Tanzen mit 150 ms
};
const int anzahlBewegungen = sizeof(bewegungen) / sizeof(bewegungen[0]);


void spieleZufallsTrack() {
  int trackNumber = random(1, 4);  // Track 1 bis 3
  Serial.print("Spiele Track Nummer: ");
  Serial.println(trackNumber);
  myDFPlayer.play(trackNumber);
/*
  // Wait until the track is finished (or up to 10 seconds max)
  unsigned long waitStart = millis();
  while (millis() - waitStart < 10000UL) {
    if (myDFPlayer.available()) {
      uint8_t type = myDFPlayer.readType();
      if (type == DFPlayerPlayFinished) {
        Serial.println("Track finished.");
        break;
      }
      myDFPlayer.read();
    }
    delay(20);
  }*/
}

void bewegeServosZufaellig() {
 //Serial.println("Bewege Servos...");

  int index = random(0, anzahlBewegungen);
  Bewegung b = bewegungen[index];

  //Serial.printf("Starte Bewegung #%d mit %d Schritten\n", index, b.laenge);

  for (int i = 0; i < b.laenge; i++) {
    int rechts = b.sequenz[i][0];
    int links  = b.sequenz[i][1];

    Serial.printf("  -> Servo rechts = %d, links = %d\n", rechts, links);

    servoRechts.write(rechts);
    servoLinks.write(links);

    delay(b.delayMs);
  }
}

void servoAus() {
  servoRechts.detach();
  servoLinks.detach();
}

void servoAn() {
  servoRechts.attach(servorechtsPin);
  servoLinks.attach(servolinksPin);
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // Initialisiert den Zufallsgenerator

  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // RX, TX
  delay(1000);

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer nicht verbunden. Schlafe endlos.");
    while (true);  // Blockieren statt Sleep
  }

  Serial.println("DFPlayer erfolgreich verbunden.");
  myDFPlayer.volume(30);  // Lautstärke

  servoRechts.attach(servorechtsPin);
  servoLinks.attach(servolinksPin);

  // IR Remote starten
  pinMode(kRecvPin, INPUT);  // Explicitly set GPIO 4 as input
  irrecv.enableIRIn();  // Start the receiver
  Serial.println("IR receiver enabled on GPIO 4.");
  Serial.printf("GPIO 4 initialized, current state: %d\n", digitalRead(kRecvPin));
  Serial.println("Waiting for IR button presses (Sleep Mode Disabled)...");

  delay(500);
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long irValue = results.value;

    // Ignoriere den NEC-Repeat Code (0xFFFFFFFF) und ungültige Signale
    if (irValue == 0 || irValue == 0xFFFFFFFF) {
      irrecv.resume();
      return;
    }

    Serial.print("IR Code empfangen: 0x");
    Serial.println(irValue, HEX);

    if (irValue == 0xFF30CF) { // Taste 1
      Serial.println("Aktion: Nur Sprechen");
      spieleZufallsTrack();
    }
    else if (irValue == 0xFF18E7) { // Taste 2
      Serial.println("Aktion: Nur Bewegen");
      bewegeServosZufaellig();
    }
    else if (irValue == 0xFF7A85) { // Taste 3
      Serial.println("Aktion: Sprechen & Bewegen gleichzeitig");
      spieleZufallsTrack(); // Startet sofort (non-blocking)
      bewegeServosZufaellig(); // Startet danach (blocking für die Dauer der Bewegung)
    }

    irrecv.resume(); // Bereit für das nächste Signal
  }
  delay(50); // Entlastung für die CPU
}
