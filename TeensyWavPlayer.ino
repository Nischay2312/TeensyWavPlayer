#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <String.h>
#include "SongList.h"
#include "Assets.h"

#define PIN_SERVOR 5
#define ext_pos 130
#define ret_pos 500
#define Freq 50


#define USE_I2S 1
// GUItool: begin automatically generated code
AudioPlaySdWav           playWav1;
#if USE_I2S == 0
AudioOutputPWM           pwm1;
AudioConnection          patchCord1(playWav1, 0, pwm1, 0);
#else           
AudioOutputI2S           audioOutput;
AudioConnection          patchCord1(playWav1, 0, audioOutput, 0);
#endif
// GUItool: end automatically generated code

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10

void Help();
void Stop();
bool StartLoop = false;
void TrackLoop(bool flag);
void ToggleLoop();

SongList MyList;
File LastDir;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    while (!Serial && millis () < 3000)
    ;

  AudioMemory(8);

  Serial.println("Starting sd setup");
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }    
  }
  LastDir = SD.open("/");
  if(USE_I2S){ 
    Serial.println("Configuring PWM Output");
    pinMode(PIN_SERVOR, OUTPUT);
    analogWriteFrequency(PIN_SERVOR, Freq);
    analogWriteResolution(12);
  }
  else{
    Serial.println("USING PWM SOUND OUTPUT.\nAMP_IN is PIN 3");
  }
  Help();
  MyList.SearchSongs(LastDir);
  Serial.println("Setup Done");
}

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);
  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);
  // A brief delay for the library read WAV info
  delay(25);
}

void loop() {
  TrackLoop(StartLoop);
  while (Serial.available() > 0) {
    char mode = Serial.read();
    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {}
    //Serial.printf("Read This %c\n", mode);
    switch(mode){
      case 'h':
        Help();
        break;
      case 'x':
        ToggleLoop();
        break;
      case 's':
        Stop();
        break;
      case 'f':
        MyList.Fwd();
        break;
      case 'b':
        MyList.Bwd();
        break;
      case 'l':
        MyList.DisplayCurrentList();
        break;
      #if USE_I2S == 1
      case 'e':
        analogWrite(PIN_SERVOR, ext_pos);
        break;
      case 'r':
        analogWrite(PIN_SERVOR, ret_pos);
        break;
      #endif       
      default:
        int tracknum = MyList.isInList(mode);
        if(tracknum != -1){
          playFile(MyList.SendTrack((uint16_t)tracknum));
        }
        else{
          Serial.println("MP3 Input not Recognized");
        }
        break;
    }
   }
   delay(20);
}

/*
Function to Stop Playing Music
*/
void Stop(){
  Serial.println("Sound Stopped");
  playWav1.stop();
}

/*
Function to Enable Sound Looping
*/
void TrackLoop(bool flag){
  if(!flag){
    return;
  }
  else{
    //Check if music is stopped, if yes then play the last song
    if(!playWav1.isPlaying()){
      playFile(MyList.SendTrack(MyList.CurrentTrackNo));
    }
  }
}

/*
Function to toggle the Loop Flag
*/
void ToggleLoop(){
  StartLoop = !StartLoop;
  Serial.printf("Looping Toggled, it is now: %s\n", StartLoop? "ENALBED": "DISABLED");
}

/*
Function to let the user know how to use the Music Player.
*/
void Help() {
  Serial.println(Message);  
  Serial.println("---------------------------------------------------------");
  Serial.println("Thank you for using the Music Player v1.0. ~~ Nischay J. [28th March 2023]");
  Serial.println("The Player is very simple to operate.");
  Serial.println("You operate using single character inputs.");
  Serial.println("The music player will display all available track at startup.");
  Serial.println("At a time only 10 can be selected.");
  Serial.println("Press 'l'(Lowercase L) to display the current 10 selected tracks." );
  Serial.println("To play a specific track, send the track number (0-9).");
  Serial.println("To stop a track, press 's'");  
  Serial.println("Send 'x' to toggle loop of track.");
  Serial.println("To go to the next 10 tracks press 'f' the next 10 tracks will be displayed.");
  Serial.println("To go to the last 10 tracks press 'b' the previous 10 tracks will be displayed.");
  Serial.println("To extend the arm, send 'e'.");
  Serial.println("To retract the arm, send 'r'.");
  Serial.println("Note: The arm is a servo motor and was used to test if the music playing block the Teensy.");
  Serial.println("To see this Help message again, press 'h'.");
  Serial.println("Enjoy your music!");
  Serial.println("---------------------------------------------------------");
}
