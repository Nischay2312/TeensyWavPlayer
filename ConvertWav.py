import os
import wave
from pydub import AudioSegment

# set the desired sampling rate
target_sr = 44100

# define the input and output folders
input_folder = r"C:\Users\nisch\OneDrive - UBC\UBC_UAS\2023\TeensyAudioTest\Input"
output_folder = r"C:\Users\nisch\OneDrive - UBC\UBC_UAS\2023\TeensyAudioTest\Output"

# get a list of all .wav files in the input folder
file_list = [f for f in os.listdir(input_folder) if f.endswith(".wav")]

# loop through each file in the list
for filename in file_list:
    # open the input file
    wav_file = wave.open(os.path.join(input_folder, filename), 'rb')
    
    # get the current sampling rate
    current_sr = wav_file.getframerate()
    
    # read in the audio data
    audio_data = wav_file.readframes(wav_file.getnframes())
    
    # convert the audio data to an AudioSegment object
    audio_segment = AudioSegment(
        data=audio_data,
        sample_width=wav_file.getsampwidth(),
        frame_rate=current_sr,
        channels=wav_file.getnchannels()
    )
    
    # resample the audio segment to the target sampling rate
    resampled_segment = audio_segment.set_frame_rate(target_sr)
    
    # construct the output filename
    output_filename = os.path.splitext(filename)[0] + "_resampled.wav"
    
    # save the resampled audio as a .wav file
    resampled_segment.export(os.path.join(output_folder, output_filename), format="wav")
    
    # close the input file
    wav_file.close()