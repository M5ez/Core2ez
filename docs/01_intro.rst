*******************
Introducing Core2ez
*******************

Core2ez is a widget-based library to create graphical programs for the M5Stack Core2 device. It is written in C++ and can be used from the Arduino and PlatformIO programming environments. It comes with a large collection of sample programs that demonstrate how to use the various functions. A multi-waveform polyphonic sound library that supports attack-decay-sustain-release envelopes is also included.

Core2ez is free software made by the M5ez collective and is currently under development. We plan a first release in March of 2021. While it is already fully useable, we will add more features and the API may still change more than one would expect from a mature library. In addition the documentation for certain features may be lacking or incomplete. We are working on all of this. If you would like to help out, either by testing new features, proofreading documentation or simply making all kinds of suggestions, please feel more than welcome. 

Core2ez behaves very much like other widget-based libraries in that it allows the programmer to define elements like buttons, input boxes and various other interface elements and then provide code for what needs to happen if they are pressed, or when text is entered, or any number of other events happen. It was made to have a gentle learning curve and be extremely easy to use, especially for very simple programs that do not need anywhere near the full power of what it can do.

.. image:: images/core2.png

The M5Stack Core2 device is a little tinker-computer based on an ESP32 SoC that comes with Bluetooth, WiFi, touch screen, built-in speaker, microphone, gyroscope, accelerometer and plenty of GPIO and other pins exposed. Core2ez is not affiliated with M5Stack, we just really like these little tinker computers.

Previous versions of the device came with three buttons underneath the screen instead of a touch screen. We wrote an Arduino library for that one too, called M5ez. Yes, that's the same name as our collective, which makes both `M5ez (the library) <https://github.com/m5ez/m5ez>`_ and Core2ez. Even though both M5ez (the library) and Core2ez are made by the same group (M5ez, the collective), the two are very different. M5ez (the library) is based on menus and linear code flows, whereas Core2ez is a widget based system where actions are usually triggered by events. We'll explain what that means further along.
