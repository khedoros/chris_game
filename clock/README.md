## An experiment in ChatGPT-aided development

The first version of the program was generated by ChatGPT with the following prompt:

> I want a program written in C++, using the SFML library, with an analog clock next to a digital clock. I'd like for the arrow keys to control the current time displayed, up+down controlling the hour and left+right controlling the minutes, perhaps.

The second version is the output from a further request:

> Currently, 12 o'clock points to the left, in the code you produced. Move it to so that 12 is up, like on a standard clock. Also, mark the hours with large ticks around the circumference of the analog clock, and the minutes with smaller ticks.

The third version is hand-modified to fix bugs (the GPT didn't fix the 12 o'clock position, the hands weren't placed correctly) and add functionality (limit to 12 hours instead of 24, improve the appearance of the output, add a text representation of the current time based on examples from Chris's homework)
