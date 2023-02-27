# microprocessor-musical-note-matching
Design documentation for the circuitry and code for a note-recognition game done using an mbed lpc176x (LPCXpresso) microprocessor

## Details
This device is a musical training game. Gameplay consists of players hearing musical notes on a pentatonic scale (the major pentatonic scale of A3, C4, D4, E4, G4, and A4) and playing the corresponding note up an octave higher in response (the notes A4, C5, D5, E5, G5, and A5).

Upon startup, a short piece of music is played to the user using the sounds that they’ll be training on. The game then plays a note randomly selected from the scale; when the user correctly plays the note an octave above it in response, the game gives a confirmation sound effect and plays a new note. The process of the game playing a note and awaiting the proper response from the user continues until the end of the game.


## Documentation
See the attached documentation.pdf for how the audio amplification circuitry and game programming (including DAC sine-wave production and input) work.
