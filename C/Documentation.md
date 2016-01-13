Elligator 2
===========

C implementation of the [Elligator 2](http://elligator.cr.yp.to/elligator-20130828.pdf) algorithm for [Curve25519](https://cr.yp.to/ecdh.html).

The u value and the square root function are taken from the section 5.5 of the Elligator specification.

It depends on the [GMP](https://gmplib.org/) library.

`elligator-2-curve25519.h`
--------------------------

The main header file. Requires `stdint.h` and `stdbool.h` to be already included.

`Elligator_2_Curve25519_initialize`
-----------------------------------

``` C
void Elligator_2_Curve25519_initialize(void);
```

Initializes the library.

`Elligator_2_Curve25519_destroy`
--------------------------------

``` C
void Elligator_2_Curve25519_destroy(void);
```

Returns the library to the uninitialized state.

`Elligator_2_Curve25519_encode` and `Elligator_2_Curve25519_decode`
-------------------------------------------------------------------

``` C
bool Elligator_2_Curve25519_encode(uint8_t *representative, const uint8_t *point, bool alternative);
bool Elligator_2_Curve25519_decode(uint8_t *point, bool *alternative, const uint8_t *representative);
```

Encode the point or decode the representative.

The `uint8_t` arguments are 32-byte little-endian numbers.

Both functions return `true` on success or `false` otherwise.

See the Javascript version [documentation](../Javascript/Documentation.md) for details.
