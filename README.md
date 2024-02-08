# GRRLIB

[![Continuous Integration](https://github.com/GRRLIB/GRRLIB/workflows/Continuous%20Integration/badge.svg)](https://github.com/GRRLIB/GRRLIB/actions)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/02847b348a1e4e6b850f956541ef2361)](https://app.codacy.com/gh/GRRLIB/GRRLIB/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Join the chat at https://gitter.im/GRRLIB/GRRLIB](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/GRRLIB/GRRLIB?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## Table of Contents

* [Introduction](#introduction)
  * ...What is it?
* [Developing for the Wii](#developing-for-the-wii)
  * ...How do I even start?
* [Downloading GRRLIB](#downloading-grrlib)
  * ...Where do I get it from?
* [Installing GRRLIB](#installing-grrlib)
  * ...How do I get it to a useable state?
* [Using GRRLIB](#using-grrlib)
  * ...What essentials do I need to know to get going?
* [Upgrading to v4.4.0 From Previous Versions of GRRLIB](#upgrading-to-v440-from-previous-versions-of-grrlib)
  * ...I upgraded and now my programs won't compile properly!?
* [Using GitHub](#using-github)
  * ...What is this Git thing that the L337 devs keep talking about?
* [Credits](#credits)
  * ...Who do I thank for all this free stuff?
* [Licence](#licence)
  * ...When you say "free" do you actually mean something else?

## Introduction

GRRLIB is a C/C++ 2D/3D graphics library for Wii application developers.  It is
essentially a wrapper which presents a friendly interface to the Nintendo GX
core.

GRRLIB is supplied as a standard C/C++ library (aka. archive)
called 'libgrrlib'.  Because GRRLIB processes JPEG and PNG images, it requires
the installation of the 'libjpeg' and 'libpngu' libraries.  'libpngu', in turn,
requires 'libpng'.  GRRLIB has FileIO functions
to allow real-time loading and saving of graphical data, and thus requires
'libfat'.  GRRLIB also has the possibility to use TrueType fonts, so
'libfreetype' is also required.

```text
libgrrlib          <- 2D/3D graphics library
├── libfat         <- File I/O
├── libjpeg        <- JPEG image processor
├── libpngu        <- Wrapper for libpng
│   └── libpng     <- PNG image processor
└── libfreetype    <- TrueType font processor
```

## Developing for the Wii

Do not progress until you have installed and configured devkitPro.  Guides are
and assistance are available at <https://devkitpro.org/index.php>

If you have just performed a clean (re)install on your Windows PC, be sure to
reboot before you continue.

## Downloading GRRLIB

You are invited to use "the latest Git master version" of GRRLIB at all times.

The Git repository is located at: <https://github.com/GRRLIB/GRRLIB>

There is a simple guide to "Using GitHub" later in this document.

This document will presume that you have downloaded "the latest Git master
version" to a directory called  C:\grr

## Installing GRRLIB

This guide is for Windows.  If you are using Linux, I am going to presume you
are smart enough to convert these instructions.

```text
GRRLIB      is supplied as source code
libpngu     is supplied as source code
libpng      is supplied via devkitPro pacman (ppc-libpng)
libfreetype is supplied via devkitPro pacman (ppc-freetype)
libjpeg     is supplied via devkitPro pacman (ppc-libjpeg-turbo)
libfat      is supplied via devkitPro pacman (libfat-ogc)
```

The easy way is to install GRRLIB and all the required libraries with a few
commands:

```bash
  c:
  cd \grr\GRRLIB
  pacman --sync --needed --noconfirm libfat-ogc ppc-libpng ppc-freetype ppc-libjpeg-turbo
  make clean all install
```

This process may take some time depending on the speed of your PC.

If you used the method above the following steps are not required, GRRLIB is
installed and you are ready to start developing Wii homebrew games.

If you want, you could install the libfat, libpng,
libfreetype and libjpeg with there dependencies in a single command:

```bash
  pacman --sync --needed --noconfirm libfat-ogc ppc-libpng ppc-freetype ppc-libjpeg-turbo
```

Each library could also be installed individually:

To install libpngu:

```bash
  c:
  cd \grr\GRRLIB\lib\pngu
  make clean all install
```

To install libgrrlib for Wii:

```bash
  c:
  cd \grr\GRRLIB\GRRLIB
  make clean all install
```

To install libgrrlib for GameCube:

```bash
  c:
  cd \grr\GRRLIB\GRRLIB
  make PLATFORM=cube clean all install
```

## Using GRRLIB

After everything is installed, simply put

```c
#include <grrlib.h>
```

at the top of your .c/.cpp file and use the functions as required

You will need to add

```make
-lgrrlib -lpngu `$(PREFIX)pkg-config freetype2 libpng libjpeg --libs` -lfat
```

to the libs line in your makefile

...Remember the order of the libraries is critical.  You may (need to) insert
other libraries in the middle of the list, you may need to add others to the
start, or even the end - but do _not_ change the order of the libraries shown
here.

Also add $(PORTLIBS) to the LIBDIRS line in your makefile

You do NOT need to place /anything/ in your application directory.

If you would like to see a working example of this, you can look at the example
found in: C:\grr\examples\template\source

## Upgrading to v4.4.0 From Previous Versions of GRRLIB

This version now uses the ported libraries for the Wii.

To install them, use this command line:

```bash
  pacman --sync --needed --noconfirm libfat-ogc ppc-libpng ppc-freetype ppc-libjpeg-turbo
```

You will need to add $(PORTLIBS) to the LIBDIRS line in your makefile.

Previously it could have looked like this:

```make
LIBDIRS	:=
```

Now it needs to be like this:

```make
LIBDIRS	:= $(PORTLIBS)
```

The FreeType library is now compiled with dependencies,
so freetype2 must be added to pkg-config in your makefile.

For example:

```make
LIBS	:= -lgrrlib -lpngu `$(PREFIX)pkg-config freetype2 libpng libjpeg --libs` -lfat
```

Since GRRLIB and libpngu are now installed into the porlibs folder, make sure you don't have any leftovers from previous installations.
You will need to manually delete unneeded libraries in _devkitPro\libogc\lib\wii_ as well as include files in _devkitPro\libogc\include_

## Using GitHub

GitHub allows the developers to submit changes to the code in such a way that
these changes can be easily monitored, quickly merged together with other
changes. and (if necessary) reverted.

It also allows the power-users to gain access to the latest (often "in-test")
features.

To obtain the "cutting edge" codebase go to <https://github.com/GRRLIB/GRRLIB/tree/master>

1. Create a directory to hold the code (e.g. C:\grr)
2. Click the _Download ZIP_ button if you don't have the proper tools installed on your PC or Clone the following URL <https://github.com/GRRLIB/GRRLIB.git>

All official releases can be found here <https://github.com/GRRLIB/GRRLIB/releases>

## Credits

### Project Leader

* NoNameNo

### Documentation

* Crayon
* BlueChip

### Lead Coder

* NoNameNo

### Support Coders

* Crayon
* Xane
* DragonMinded
* BlueChip
* elisherer

### Advisors

* RedShade
* Jespa

## Licence

GRRLIB is released under the [MIT Licence](LICENCE.md).
If we had chosen the GPL licence you would be +forced+ (legally required) to release your source code.
But in the spirit of "free as in FREE" we have left you with the +option+ to release your
source code.

We do +request+ that you tell others about us by naming our library (GRRLIB) in
the credits of your game/application.  And, if you +choose+ to do that, we
encourage you to use our logo to achieve it; You can find our logo here:
C:\grr\docs\grrlib_logo.png
and here:
<https://grrlib.github.io/GRRLIB/grrlib_logo.png>
