***************
How can I help?
***************

We realise we've embarked on a large journey with a very small team and we could use some help with pretty much everything we're doing right now. Help is particularly welcome in the following areas:

* **Coding**: help us develop new widgets or debug the existing ones.

* **Documentation**: help document Core2ez, or proofread existing documentation to help us improve it.

* **Suggestions**: Use Core2ex for your own projects and tell us what's missing, or what could be done differently.

* **Spread the word**: Tell others about Core2ez, help them to get to know it, and make sure it is talked about when the Core2 or other relevant things are discussed.


This project lives in its GitHub repository. The best way to talk to us about problems or suggestions regarding code or documentation is to file an issue in the repository. Feel free to include a Pull Request if you know how to fix the problem yourself.


Writing documentation
=====================

You'll notice that the documentation for Core2ez is displayed with the help of `Read the Docs <readthedocs.org>`_. They compile the version they display from the 'docs' directory in our library. The source for the documentation is written in `reStructuredText` (.rst) format. To get started with that, check out `this primer <https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html>`_. 

While you are making changes to the source of the documentation, you'll want to know what your changes look like in the format that Read the Docs renders to. For this you will need to install python and then install the necessary packages with pip.

  ``pip install Sphinx sphinx-rtd-theme``

After doing this, go the docs directory and type ``make html``. If no errors are reported, your modified documentation can be viewed by opening ``_build/html/index.html`` with a browser.

.. note::

    We've added a 'display' target to the Makefile for the documentation. It opens the documentation in a new window in Chrome on MacOS. If you use a Mac and Chrome, you can simply type ``make html display`` if you want to see your changes.