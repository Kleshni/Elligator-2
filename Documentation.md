Elligator 2
===========

Javascript implementation of the [Elligator 2](http://elligator.cr.yp.to/elligator-20130828.pdf) algorithm for [Curve25519](http://cr.yp.to/ecdh.html).

It depends on the [Elliptic](https://github.com/indutny/elliptic/) library.

`Elligator2Curve25519(curve)`
-----------------------------

Constructor of a coder. Requires an instance of a curve, which can be obthained with the following code:

```
var ECC = new ellipticjs.ec("curve25519");
var curve = ECC.curve;
```

### `encode(point, alternative)`

Encodes the point.

Returns the encoded point as a `Uint8Array` of 32 bytes or `null` if it can't be encoded. Note, that the returned representative is a 254-bits value, not 256.

The algorithm can return two different values for a single x coordinate. Which one to return is determined by y coordinate. Sinse Curve25519 doesn't use y due to optimizations, you should specify a Boolean value as the second argument of the function. It should be unpredictable, because it's recoverable from the representative.

### `decode(representative)`

Decodes the representative.

Returns an array with the point and the second argument of the corresponding call to the `encode` function. It's also able to return `null`, but this is very unlikely.
