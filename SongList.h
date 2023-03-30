#ifndef __SONGLIST_H__
#define __SONGLIST_H__

#include "Arduino.h"
#include <string.h>
#include <SD.h>
#include <SerialFlash.h>

#define ListSize 5
#define MaxName 25

class SongList{
  private:
    char **Songlist = 0;
    uint16_t numsongs;
    uint16_t listsize_r = ListSize;
    uint16_t counter = 0;
    uint16_t counter_limit = 0;
    void initializeSongList();
    void AddMoreListMember(size_t Size);
    char* stringUpper(char* string_in);
  public:
    uint16_t CurrentTrackNo;
    /*
    SongList Class Constructor
    Initializes the list data structure 
    */
    SongList();
    /*
    Displays all the contents in the list. Data is output to the serial port.
    */
    void DisplayListContents();
    /*
    Displays the current 10 tracks that can be selected. The data is output to the serial port.
    */
    void DisplayCurrentList();
    /*
    Finds all the .Wav files in a specified directory. Fills the internal data structure with their name.
    ParameterL rootDir: File pointer to the directory that you want to search the tracks in.
    */
    void SearchSongs(File rootDir);
    /*
    checks if the selected song is in the list. Returns the postiton of the song, otherwise returns -1.
    Parameter: tracknum, a character which should be from 0-9.
    */
    int isInList(char tracknum); 
    /*
    Outputs the track name corresponding to the tracknum selected.
    Parameter: tracknum, uint16_t track number selected to play.
    Returns: pointer to the string containing the track name.
    */
    char* SendTrack(uint16_t tracknum);
    /*
    Moves the internal track selector forward by 10 tracks.
    Outputs the list of track that can be selected.
    Data is output to the serial port.
    */
    void Fwd();
    /*
    Moves the internal track selector backward by 10 tracks.
    Outputs the list of track that can be selected.
    Data is output to the serial port.
    */
    void Bwd();
};

#endif
