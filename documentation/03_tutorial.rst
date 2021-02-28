********
Tutorial
********

This tutorial, currently under construction, will have topics for all major areas of Core2ez programming. The tutorial aims to cover a lot of ground while still being readable to novice programmers with only minimal experience in C and Arduino programming. It is accompanied by the reference manual, which will show every single aspect of every programming elements.

Overview of How Things Work
===========================

The Arduino IDE (Integrated Development Environment) allows you to program in C++ while taking away some of the complexities that normally come with that. Originally made for circuit boards sold under the Arduino name, lots of different pieces of hardware can now be programmed using the same software. In setting up your environment (see `Getting Started <02_getting_started.html>`_), you have told the Arduino software about the M5Stack Core2 and you have installed a library that provides a set of functions to talk to the specific hardware components of the Core2. This library is called ``M5Core2`` and provided by M5Stack.

This ``M5Core2`` library does the job of letting you talk to the hardware, but it is also quite rudimentary: you can put things on the display and talk to the other hardware, but it does not provide the ease of programming that Core2ez provides.

``Core2ez``, this library, is included in your programs in addition to the ``M5Core2`` library. Think of it as one layer on top of another. You will largely be talking with the hardware through this library, which then talks to ``M5Core2`` in turn. But you can also still use any of the functions in ``M5Core2`` directly.


The Screen
==========

To put things on the screen, we use the ``ezScreen`` object. 

 ezScreen supports most of the same functions as the underlying ``m5.lcd``.





.. image:: _static/doxygen_html/classez_widget__inherit__graph.svg



Points and the Touch Sensor
===========================

All Kinda of Widgets
--------------------

Sound
-----
