# Speaking Backpack Rabbit
A old children`s backpack transformed into an animatronic mascot.
The project uses an ESP32 and additional hardware to randomly play pre-recorded audio tracks while moving two servos for expressive actions like waving and dancing.

## Key Features

- ESP32 controller using the Arduino framework
- Audio playback with DFPlayer Mini and microSD-stored tracks
- Two independent servos for animated motion
- Randomized behavior: speech only, motion only, or both together
- PlatformIO project setup for easy build and upload

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

The ESP32 code periodically selects one of three modes:

1. Play a random audio track from the DFPlayer Mini
2. Execute a randomized servo motion sequence
3. Perform both audio playback and servo motion together

Servo motion sequences include:

- Waving motion
- Dancing motion
- Static pose positions

Audio tracks are selected randomly from tracks 1 through 3.

## Build and Upload

1. Install PlatformIO in VS Code or use the PlatformIO CLI.
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

## Notes

This project currently includes DFPlayer Mini audio playback and simple random motion routines. IR remote support is included in the code imports but currently not active.
