A WAV player based on Teensy 4.0. Controller using Serial Input.<p>
**Based on the Example Sketch: WAVFilePlayer from PJRC**

## Player in Action
[![Video Thumbnail](https://i.imgur.com/etWtLjw.jpeg)](https://youtu.be/73Po7TvNCfo)

## Components Used:
1. [Teensy 4.0](https://www.pjrc.com/store/teensy40.html)
2. [4 Ohm 3W Speaker](https://a.co/d/9ChifTU)
3. [Adafruit SD Card Breakout Board](https://a.co/d/350EmVX)
4. [MAX98357A I2S Amplifier](https://a.co/d/iPsQDQp)
5. [16 GB MicroSD Card](https://a.co/d/iPsQDQp)
6. Basic electronic componets (jumper wires, soldering iron, breadboard, micro usb cable)

# Connections:
<ol>
<li> SD CARD:</li>
  <ul>
  <li>Adafruit Board Pin &emsp; Teensy Pin</li>
  <li> &emsp;CS &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 10</li>
  <li> &emsp;DI &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp; 11</li>
  <li> &emsp;DO &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 12</li>
  <li> &emsp;CLK &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 13</li>
  <li> &emsp;GND &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; GND</li>
  <li> &emsp;5v &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 5V</li>
  </ul>
 <li> MAX98537A:</li>
  <ul>
  <li>Amplifier Pin &emsp;&emsp;&emsp; Teensy Pin</li>
  <li> &emsp;LRCLK &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 20</li>
  <li> &emsp;BCLK &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 21</li>
  <li> &emsp;DIN &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 7</li>
  <li> &emsp;VDD &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 5V</li>
  <li> &emsp;GND &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; GND</li>
  <li> &emsp;Gain &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; GND</li>
  </ul>
 <li>Speaker connect to the I2S Amplifier Board Output. Polaity does not matter (at least for the ones that are linked above)</li>
</ol>

# Setup + How to Use

1. Flash the Arduino Sketch onto the teensy and open the serial monitor to view the output. If everything is correct then you will see the list of songs loaded in the root folder of the SD card as a list.
2. To load music easily, I have made a simple [python script](https://github.com/Nischay2312/TeensyWavPlayer/blob/main/ConvertWav.py). You can use this to quickly convert large batches of .WAV files into 44.1kHz .WAV files which are supported. Before using it, you need `pydub` module which can be done using this: `pip install pydub`. Also be sure to change the location of the Input and Output Folder. The script converts all the .WAV files from Input folder to 44.1kHz .WAV files and saves them in the Output folder.
3. The controls are simple: Send 'h' to see the help text. Send `l` (lowercase L) to display the current 10 tracks. Send the number corresponding to the track (0-9) to play it. Send `s` to stop playing. Sending `f` selects the next 10 track and sending `b` selects the previous 10 tracks. Sending 'x' will toggle the Track Loop Feature. Once enabled, the track will play again if it is stopped. HOORAY ENDLESS MUSIC!!!!  
