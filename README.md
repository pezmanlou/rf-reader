rf-reader
=========
* written for the teensy 2.0
* the teensy connects to a POSX card reader (model unknown, see pictures in folder)

to use
* open text editor
* swipe card
* teensy will emulate keyboard and type 1s and 0s corresponding to the bits encoded on the magnetic stripe
* output will be in this format: stripe-1 \t stripe-2 \t stripe-3 \n
