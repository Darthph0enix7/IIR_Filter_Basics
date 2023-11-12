#include <SD.h>
#include <SPI.h>

const int chipSelect = 10; // SD card chip select pin
const char *filename = "1.mp3"; // Audio file name

// Simple Low-Pass Filter Variables
const float alphaLPF = 0.1; // Low-pass filter coefficient (between 0 and 1)
float filteredValueLPF = 0;

// Simple High-Pass Filter Variables
const float alphaHPF = 0.1; // High-pass filter coefficient (between 0 and 1)
float filteredValueHPF = 0;
float previousInputHPF = 0;
float previousOutputHPF = 0;

void setup() {
  Serial.begin(9600);

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed.");
    return;
  }

  File audioFile = SD.open(filename);
  if (!audioFile) {
    Serial.println("Failed to open audio file.");
    return;
  }

  // Assuming 8-bit mono audio file
  while (audioFile.available()) {
    int sample = audioFile.read(); // Read one 8-bit sample
    
    // Apply low-pass filter
    filteredValueLPF = lowPassFilter(sample);

    // Apply high-pass filter
    filteredValueHPF = highPassFilter(sample);

    // Output the filtered values - for demonstration purposes, via Serial
    Serial.print("LPF: ");
    Serial.print(filteredValueLPF);
    Serial.print("\tHPF: ");
    Serial.println(filteredValueHPF);
  }

  audioFile.close();
}

void loop() {
  // No implementation required for this example
}

float lowPassFilter(int sample) {
  filteredValueLPF = alphaLPF * sample + (1 - alphaLPF) * filteredValueLPF;
  return filteredValueLPF;
}

float highPassFilter(int sample) {
  filteredValueHPF = alphaHPF * (previousOutputHPF + sample - previousInputHPF);
  previousInputHPF = sample;
  previousOutputHPF = filteredValueHPF;
  return filteredValueHPF;
}
