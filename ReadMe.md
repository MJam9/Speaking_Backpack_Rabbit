# Speaking Backpack Rabbit
A old children`s backpack transformed into an animatronic mascot.
The project uses an ESP32 and additional hardware to randomly play pre-recorded audio tracks while moving two servos for expressive actions like waving and dancing. All actions are triggered via an IR remote control.

<img width="227" height="404" alt="20260609_085228" src="https://github.com/user-attachments/assets/e17b48d0-239a-40b4-b637-ff839f8ef61f" />


## Key Features

- **ESP32 Core:** Powered by the Arduino framework for ESP32.
- **Audio Playback:** High-quality sound using the DFPlayer Mini module.
- **Animatronic Motion:** Two independent servos for lifelike movements.
- **IR Control:** Handheld control with three distinct modes (Speech, Motion, or Combined).
- **PlatformIO Integrated:** Easy configuration and deployment.

## Hardware Requirements

- ESP32 development board (e.g. esp32dev)
- DFPlayer Mini MP3 module
- Speaker connected to the DFPlayer Mini
- Two hobby servos
- Power supply for ESP32 and servos

## Software Components

- `src/main.cpp` - Main application code
- `platformio.ini` - PlatformIO build environment configuration
- `ReadMe.md` - Project documentation

## Libraries Used

- `ESP32Servo`
- `DFRobotDFPlayerMini`
- `espsoftwareserial`
- `IRremoteESP8266`

## How It Works

Über die IR-Fernbedienung wird der Modus gewählt der ausgewählt wird:

1. Play a random audio track from the DFPlayer Mini
2. Execute a randomized servo motion sequence
3. Perform both audio playback and servo motion together

Innerhalb der Moduse ist es zufällig welcher Sound oder bewegung durchgeführt wird

Servo motion sequences include:

- Waving motion
- Dancing motion
- Static pose positions

Audio tracks are selected randomly from tracks 1 through 3.

## Build and Upload

1. Install PlatformIO in VS Code.
2. Connect the ESP32 board to your computer.
3. Update the upload port in `platformio.ini` if needed (`COM3` by default).
4. Build and upload using:

```bash
pio run
pio run -t upload
```

## Audio Setup

Prepare a microSD card with audio files named `0001.mp3`, `0002.mp3`, and `0003.mp3` to match DFPlayer Mini conventions.

## Customization

- Change servo pins in `src/main.cpp`
- Modify motion sequences or delay timing
- Add additional audio files to the DFPlayer Mini

