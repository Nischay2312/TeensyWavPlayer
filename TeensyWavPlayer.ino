#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <String.h>

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

void Play();
void Stop();
void Fwd();
void Bwd();

volatile uint8_t cntr_lim = 2;
volatile uint8_t cntr = 0;

#define ListSize 5
#define MaxName 25
char **SongList = 0;
int numsongs = 0;
File LastDir;
uint8_t listsize_r = ListSize;
void initializeSongList();
void SearchSongs(File dir);
void AddMoreListMember(size_t Size);
char* stringUpper(char* string_in);
void DisplayListContents();
void DisplayCurrentList();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    while (!Serial && millis () < 3000)
    ;

  AudioMemory (8);
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
  initializeSongList();
  SearchSongs(LastDir);
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
  while (Serial.available() > 0) {
    char mode = Serial.read();
    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {}
    //Serial.printf("Read This %c\n", mode);
    switch(mode){
      case 'p':
        Play();
        break;
      case 's':
        Stop();
        break;
      case 'f':
        Fwd();
        break;
      case 'b':
        Bwd();
        break;
      case 'l':
        //DisplayListContents();
        DisplayCurrentList();
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
        int songsel = ((int)mode - 48) + cntr*10;
        if(songsel >= 0 && songsel <=(numsongs)){
          Serial.printf("Song Number %d selected\n", songsel);
          playFile(SongList[songsel]);
        }
        else{
          Serial.println("MP3 Input not Recognized");
        }
        break;
    }
   }
}

void SearchSongs(File dir){
  numsongs = 0;
  //Serial.println("Printing WAV files:");
  while(true){
    File entry = dir.openNextFile();
    if(!entry){
      //Serial.println("Nothing Left");
      break;
    }
    //Serial.print("Opened: ");
    //Serial.println(entry.name());
    if(!(entry.isDirectory())){
      if(strstr(entry.name(), ".wav") != NULL){
        //Save the name
        strcpy(SongList[numsongs],(entry.name()));
        //Serial.printf("%d. ", numsongs);
        //Serial.println(SongList[numsongs]);
        numsongs++;
        //Serial.printf("Numsongs: %d\n", numsongs);
        if(numsongs >= listsize_r){
          //Add more members
          //Serial.println("Oops List full, adding one more member");
          AddMoreListMember(ListSize + listsize_r);
          listsize_r = listsize_r + ListSize;
        }
      }      
    }
    else{
      //Serial.println("File is a directory");
    }
  }
  Serial.println("WAV file Scan Complete, Now printing the whole Song List:");
  DisplayListContents();
  cntr_lim = numsongs / 10;
}

void DisplayListContents(){
  for(int i = 0; i < numsongs; i++){
    Serial.printf("%d. ",i);
    Serial.println(SongList[i]);
  }
}

void DisplayCurrentList(){
  uint8_t lim = numsongs < (cntr*10 + 10)?numsongs: (cntr*10 + 10);
  uint8_t start = 0 + (cntr*10);
  for(uint8_t i = start; i < lim; i++){
    Serial.printf("%d. ", i - (cntr*10));
    Serial.println(SongList[i]);
  }
}

char* stringUpper(char* string_in){
  for(int i = 0; string_in[i] != '\0'; i++){
    if(string_in[i] >= 'a' && string_in[i] <= 'z'){
      string_in[i] -= 32;
    }
  }
  return string_in;
}

void initializeSongList(){
  SongList = (char**)malloc(ListSize * sizeof(char*));
  for(int i = 0; i < ListSize; i++){
    SongList[i] = (char*)malloc(MaxName * sizeof(char*));
  }
}

void AddMoreListMember(size_t Size){
  SongList = (char**)realloc(SongList, sizeof(char*)*(Size));
  for(uint8_t i = 0; i < Size; i++){
    SongList[i] = (char*)realloc(SongList[i], MaxName * sizeof(char*));
  }
} 

void Play(){
  Serial.println("Started Playing");
  switch(cntr){
    case 0:
      playFile("K.WAV");  // filenames are always uppercase 8.3 format
      Serial.println("Playing KickBack");
      break;
    case 1:
      playFile("B.WAV");
      Serial.println("Playing Barbie");
      break;
    case 2:
      playFile("S.WAV");
      Serial.println("Playing SickoMode");
      break;
    case 3: 
      playFile("B.WAV");
      Serial.println("Playing BarbieGirl");
      break;
    case 4: 
      playFile("S.WAV");
      Serial.println("Playing SickoMode");
      break;
    default:
      Serial.println("Play counter Error");
  }
  
}

void Stop(){
  Serial.println("Sound Stopped");
  playWav1.stop();
}

void Fwd(){
  //Serial.println("Forwared Track Functionality to be added");
  cntr++;
  if(cntr > cntr_lim){
    cntr = 0;
  }
  //Serial.printf("Going Forward, current track no:%d", cntr);
  DisplayCurrentList();
}

void Bwd(){
  if(cntr == 0){
    cntr = cntr_lim;
  }
  else{
    cntr--;
  }
  //Serial.printf("Going Back, current track no:%d", cntr);
  DisplayCurrentList();
}

