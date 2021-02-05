.. Core2ez documentation master file, created by
   sphinx-quickstart on Thu Feb  4 12:33:57 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.


.. toctree::
   :maxdepth: 2
   :caption: Contents:

Introducing Core2ez
===================

Core2ez is a widget-based library to create graphical programs for the M5Stack Core2 device. It is written in C++ and can be used from the Arduino and PlatformIO programming environments. It comes with a large collection of sample programs that demonstrate how to use the various functions. A multi-waveform polyphonic sound library that supports attack-decay-sustain-release envelopes is also included.

Core2ez is currently under development. While it is fully useable, the API may still change more than one would expect from a mature library. In addition the documentation for certain features may be lacking or incomplete. We are working on all of this. If you would like to help out, either by testing new features, proofreading documentation or simply making all kinds of suggestions, please feel free. 

Core2ez behaves very much like other widget-based libraries in that it allows the programmer to define elements like buttons, input boxes and various other interface elements and then provide code for what needs to happen if they are pressed, or when text is entered, or any number of other events happen. It was made to have a gentle learning curve and be extremely easy to use, even for very simple programs that do not need anywhere near the full power of what it can do.

|core2|

.. |core2| image:: images/core2.png

The M5Stack Core2 device is a little tinker-computer based on an ESP32 SoC that comes with Bluetooth, WiFi, touch screen, built-in speaker, microphone, gyroscope and accelerometer. This project is not affiliated with M5Stack, they just happen to make our favorite little tinker computers right now.

Previous versions of the device came with three buttons underneath the screen instead of a touch screen. We wrote a library for that one too, it is called M5ez. Even though both M5ez and Core2ez are made by the same group, they are very different. M5ez was based on menus and linear code flows, while Core2ez is a widget based system where actions are usually triggered by events. We'll explain what that means further along.


Getting Started with Core2ez
============================

You can buy the Core2 device from M5Stack directly, but it's quite popular and lots of distributors and online vendors also have it `in stock <https://www.google.com/search?q=m5stack+core2+buy>`_. Assuming you have the device, follow M5Stack's `instructions <https://docs.m5stack.com/#/en/arduino/arduino_core2_development>`_ for downloading and configuring the Arduino environment for it.

.. note::

    This library is made for the Arduino Integrated Development Environment (IDE) but should also work with the PlatformIO IDE/debugger. If there are specific things to document about getting set up to work with PlatformIO we will do so in a later stage. For now this documentation focusses on the Arduino IDE.
    
Test by compiling one of the examples that come with the 'M5Core2' library to see that everything works as intended before continuing.

Once that works, install this library. At this point in development, this is done by downloading the library directly into the Arduino libraries directory. To do this, go to the Arduino libraries directory using ``cd ~/Documents/Arduino/libraries`` on MacOS, or go to the 'libraries' subdirectory of wherever your Arduino sketches are on other systems. Once there, enter ``git clone https://github.com/m5ez/Core2ez`` to install the library. Alternatively, you can `download the ZIP file <https://github.com/M5ez/Core2ez/archive/master.zip>`_ and unzip it in the Arduino libraries directory.

To upgrade to the latest development version, cd into to the 'libraries/Core2ez' directory and enter ``git pull`` if you used git to download, or repeat the ZIP download from above.

.. note::

    Once we have officially released, you'll be able to use the Arduino library manager to install the release version of Core2ez and keep it up to date.

Once Core2ez is downloaded, you should be able to compile and run the examples provided. To test, try compiling `File / Examples / Core2ez / General / circles` and move your fingers across the touch screen. If you see a red circle following your finger, Core2ez is correctly set up.


Writing documentation
=====================

You'll notice that the documentation for Core2ez is displayed with the help of `Read the Docs <readthedocs.org>`_. They compile the version they display from the 'docs' directory in our library. The source for the documentation is written in `reStructuredText` (.rst) format. To get started with that, check out `this primer <https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html>`_. 

While you are making changes to the source of the documentation, you'll want to know what your changes look like in the format that Read the Docs renders to. For this you will need to install python and then install the necessary packages with pip.

  ``pip install Sphinx sphinx-rtd-theme``

After doing this, go the docs directory and type ``make html``. If no errors are reported, your modified documentation can be viewed by opening ``_build/html/index.html`` with a browser.

.. note::

    We've added a 'display' target to the Makefile for the documentation. It opens the documentation in a new window in Chrome on MacOS. If you use a Mac and Chrome, you can simply type ``make html display`` if you want to see your changes.


How can I help?
===============

We realise we've embarked on a large journey with a very small team and we could use some help with pretty much everything we're doing right now. Help is particularly welcome in the following areas:

* **Coding**: help us develop new widgets or debug the existing ones.

* **Documentation**: help document Core2ez, or proofread existing documentation to help us improve it.

* **Suggestions**: Use Core2ex for your own projects and tell us what's missing, or what could be done differently.

* **Spread the word**: Tell others about Core2ez, help them to get to know it, and make sure it is talked about when the Core2 or other relevant things are discussed.


This project lives in its GitHub repository. The best way to talk to us about problems or suggestions regarding code or documentation is to file an issue in the repository. Feel free to include a Pull Request if you know how to fix the problem yourself.


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
