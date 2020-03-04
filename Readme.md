Elligator 2
===========

C implementation of the [Elligator 2](https://elligator.cr.yp.to/elligator-20130828.pdf) algorithm for the [Curve25519](https://cr.yp.to/ecdh.html) elliptic curve. The library also provides a function for point conversion from Ed25519 to Curve25519.

Requires the [GMP](https://gmplib.org/) library. [CMake](https://cmake.org/) is used as a build system, [Check](https://libcheck.github.io/check/) is needed for tests and [Doxygen](https://www.stack.nl/~dimitri/doxygen/) for documentation generation.

Binaries
--------

Build the target `library`. On Linux: run `cmake .` to generate a makefile, and then `make library`.

Tests
-----

Make the target `check`.

Documentation
-------------

Read in the [header](elligator-2-curve25519.h) file or make the target `documentation` to generate HTML.

Install
-------

Execute the target `install/strip`.

Links
-----

* [Source code](https://github.com/Kleshni/Elligator-2/archive/master.zip).
* [Git repository](https://github.com/Kleshni/Elligator-2.git).
* [Issue tracker](https://github.com/Kleshni/Elligator-2/issues).
* Bitmessage: BM-2cT5WWccBgLsHTw5ADLcodTz4dbqdtrwrQ.
* Mail: [kleshni@protonmail.ch](mailto:kleshni@protonmail.ch).

Licence: CC0 or Public Domain.
