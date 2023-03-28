#include "SongList.h"

SongList::SongList(){
  this->initializeSongList();
}

void SongList::initializeSongList(){
  Songlist = (char**)malloc(ListSize * sizeof(char*));
  for(int i = 0; i < ListSize; i++){
    Songlist[i] = (char*)malloc(MaxName * sizeof(char*));
  }
}

void SongList::AddMoreListMember(size_t Size){
  Songlist = (char**)realloc(Songlist, sizeof(char*)*(Size));
  for(uint8_t i = 0; i < Size; i++){
    Songlist[i] = (char*)realloc(Songlist[i], MaxName * sizeof(char*));
  }
}

char* SongList::stringUpper(char* string_in){
  for(int i = 0; string_in[i] != '\0'; i++){
    if(string_in[i] >= 'a' && string_in[i] <= 'z'){
      string_in[i] -= 32;
    } 
  }
  return string_in;
}

void SongList::DisplayCurrentList(){
  uint16_t lim = numsongs < (counter*10 + 10)?numsongs: (counter*10 + 10);
  uint16_t start = 0 + (counter*10);
  Serial.println("Current 10 electable Songs Shown Below:\n------------------------------");
  for(uint16_t i = start; i < lim; i++){
    Serial.printf("%d. ", i - (counter*10));
    Serial.println(Songlist[i]);
  }
  Serial.println("------------------------------");
}

void SongList::DisplayListContents(){
  Serial.println("Full List of Songs Shown Below:\n------------------------------");
  for(int i = 0; i < numsongs; i++){
    Serial.printf("%d. ",i);
    Serial.println(Songlist[i]);
  }
  Serial.println("------------------------------");
}

void SongList::SearchSongs(File rootDir){
  while(true){
    File entry = rootDir.openNextFile();
    if(!entry){
      break;
    }
    if(!(entry.isDirectory())){
      if(strstr(entry.name(), ".wav") != NULL){
        //Save the name
        strcpy(Songlist[numsongs],(entry.name()));
        numsongs++;
        if(numsongs >= listsize_r){
          this->AddMoreListMember(ListSize + listsize_r);
          listsize_r = listsize_r + ListSize;
        }
      }      
    }
    else{
      //Serial.println("File is a directory");
      }
  }
  Serial.println("WAV file Scan Complete, Now printing the whole Song List:");
  this->DisplayListContents();
  counter_limit = numsongs / 10;  
}
int SongList::isInList(char tracknum){
  int songsel = ((int)tracknum - 48) + counter*10;
  if(songsel >= 0 && songsel <=(numsongs)){
    return songsel;
  }
  else 
    return -1;
}

char* SongList::SendTrack(uint16_t tracknum){
  return Songlist[tracknum];
}

void SongList::Fwd(){
  //Serial.println("Forwared Track Functionality to be added");
  counter++;
  if(counter > counter_limit){
    counter = 0;
  }
  //Serial.printf("Going Forward, current track no:%d", cntr);
  this->DisplayCurrentList();
}

void SongList::Bwd(){
  if(counter == 0){
    counter = counter_limit;
  }
  else{
    counter--;
  }
  //Serial.printf("Going Back, current track no:%d", cntr);
  this->DisplayCurrentList();
}

