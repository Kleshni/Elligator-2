/**
	\file

	The #Elligator_2_Curve25519_initialize function must be called first to initialize the library.

	A representative value is a little-endian array of #ELLIGATOR_2_CURVE25519_REPRESENTATIVE_LENGTH bytes. The encoding function produces a 254-bit number with the most 2 significant bits of the last byte always unset. More precisely, it's <= 2 ^ 254 - 10. The decoding function accepts all numbers under 2 ^ 255 - 19.

	The header file contains an include guard.
*/

#ifndef ELLIGATOR_2_CURVE25519_H
	/**
		Include guard.
	*/

	#define ELLIGATOR_2_CURVE25519_H

	#include <stdint.h>
	#include <stdbool.h>

	#include <gmp.h>

	#if GMP_NUMB_BITS < 8
		#error "Too short GMP limbs"
	#endif

	/**
		Byte length of a point on Curve25519.
	*/

	#define ELLIGATOR_2_CURVE25519_POINT_LENGTH 32

	/**
		Byte length of a representative.
	*/

	#define ELLIGATOR_2_CURVE25519_REPRESENTATIVE_LENGTH 32

	/**
		Byte length of a point on Ed25519.
	*/

	#define ELLIGATOR_2_CURVE25519_ED25519_POINT_LENGTH 32

	/**
		Initializes the library. Can be called multiple times.

		It's thread unsafe.
	*/

	void Elligator_2_Curve25519_initialize(void);

	/**
		Does nothing, kept for compatibility.
	*/

	void Elligator_2_Curve25519_destroy(void);

	/**
		Tries to encode a point.

		\param [out] representative The resulting representative.

		\param point The point.

		\param high_y The algorithm can produce two different values for a single x coordinate. Which one to return is determined by the y coordinate: whether it's > 2 ^ 254 - 10 or not. If the point is 0, this parameter is ignored.

		\returns `true` on success. If the point can't be encoded, `false` is returned and the content of the `representative` array is undefined, but can depend on the input.

		\note The `high_y` value is recoverable from the representative.

		\note The function doesn't validate the point.

		\see The header file description.
	*/

	bool Elligator_2_Curve25519_encode(uint8_t *representative, const uint8_t *point, bool high_y);

	/**
		Decodes a representative.

		\param [out] point The resulting point.

		\param [out] high_y The `high_y` argument of the corresponding #Elligator_2_Curve25519_encode call, or `false` if the point is 0.

		\param representative The representative.

		\returns `true` if the representative is the least square root, i. e. is <= 2 ^ 254 - 10, or `false` otherwise.

		\see The header file description.
	*/

	bool Elligator_2_Curve25519_decode(uint8_t *point, bool *high_y, const uint8_t *representative);

	/**
		Tries to convert a point from the Ed25519 curve to a point on Curve25519, using their birational equivalence.

		\param [out] point The resulting point.

		\param source The original point.

		\returns The function validates the input point and returns a Boolean result. If `false` is returned, the content of the `point` array is undefined, but can depend on the input.

		\see The header file description.
	*/

	bool Elligator_2_Curve25519_convert_from_Ed25519(uint8_t *point, const uint8_t *source);
#endif
