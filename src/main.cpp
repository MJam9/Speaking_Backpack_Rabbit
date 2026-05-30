#include <Arduino.h>
#include <ESP32Servo.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"//Download hier zu finden: https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

// Servo-Objekt
Servo servoRechts;
Servo servoLinks;

// DFPlayer Mini
HardwareSerial mySerial(2);
DFRobotDFPlayerMini myDFPlayer;

const int servorechtsPin = 27;  // Signal-Pin für das Servo (GPIO 18 alt)
const int servolinksPin = 26;  // Signal-Pin für das Servo (GPIO 19 alt)

//const uint16_t kRecvPin = 27;  // Dein Signalpin
//IRrecv irrecv(kRecvPin);
//decode_results results;

// Zeitsteuerung
unsigned long interval = 5 * 1000; // 5 Sekunden in ms
unsigned long lastActionTime = 0;

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
  //Serial.print("Spiele Track Nummer: ");
  //Serial.println(trackNumber);
  myDFPlayer.play(trackNumber);
  delay(5000);
}

void bewegeServosZufaellig() {
 //Serial.println("Bewege Servos...");

  int index = random(0, anzahlBewegungen);
  Bewegung b = bewegungen[index];

  //Serial.printf("Starte Bewegung #%d mit %d Schritten\n", index, b.laenge);

  for (int i = 0; i < b.laenge; i++) {
    int rechts = b.sequenz[i][0];
    int links  = b.sequenz[i][1];

    //Serial.printf("  -> Servo rechts = %d, links = %d\n", rechts, links);

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

  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // RX, TX
  delay(1000);

  if (!myDFPlayer.begin(mySerial)) {
    //Serial.println("DFPlayer nicht verbunden. Schlafe endlos.");
    while (true);  // Blockieren statt Sleep
  }

  //Serial.println("DFPlayer erfolgreich verbunden.");
  myDFPlayer.volume(30);  // Lautstärke

  servoRechts.attach(servorechtsPin);
  servoLinks.attach(servolinksPin);

  lastActionTime = millis();
}

void loop() {
  if (millis() - lastActionTime >= interval) {
    lastActionTime = millis();

    // Zufallsmodus: 0 = Sprechen, 1 = Bewegen, 2 = Beides
    int mode = random(0, 3);
    // mode = 2;  // Zum Testen gezielt nur "beides"

    //Serial.print("Modus gewählt: ");
    //Serial.println(mode);

    if (mode == 0 || mode == 2) {
      spieleZufallsTrack();
    }

    if (mode == 1 || mode == 2) {
      //servoAn();
      bewegeServosZufaellig();
      delay(500);  // kurz stabilisieren
      //servoAus();
    }

    // Warten vor Sleep, damit DFPlayer etc. fertig sind
    delay(5000); 

    /*// Light Sleep starten
    Serial.println("Gehe in Light Sleep...");
    esp_sleep_enable_timer_wakeup(interval * 1000);  // Mikrosekunden
    esp_light_sleep_start();
    Serial.println("Aufgewacht aus Light Sleep.");*/
  }
}
