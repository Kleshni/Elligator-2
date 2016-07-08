#ifndef TESTS_H
	#define TESTS_H

	#include <stdint.h>
	#include <stdbool.h>

	#include <check.h>

	#include "main.h"
	#include "elligator-2-curve25519.h"

	struct least_square_root_test {
		uint8_t number[P_BYTES];
		bool result;
		uint8_t root[P_BYTES];
	};

	struct encoding_test {
		uint8_t point[ELLIGATOR_2_CURVE25519_POINT_LENGTH];
		bool high_y;
		bool result;
		uint8_t representative[ELLIGATOR_2_CURVE25519_REPRESENTATIVE_LENGTH];
	};

	struct decoding_test {
		uint8_t representative[ELLIGATOR_2_CURVE25519_REPRESENTATIVE_LENGTH];
		bool result;
		uint8_t point[ELLIGATOR_2_CURVE25519_POINT_LENGTH];
		bool high_y;
	};

	struct conversion_test {
		uint8_t source[ELLIGATOR_2_CURVE25519_ED25519_POINT_LENGTH];
		bool result;
		uint8_t point[ELLIGATOR_2_CURVE25519_POINT_LENGTH];
	};

	#define LEAST_SQUARE_ROOT_TESTS_COUNT 6
	#define ENCODING_TESTS_COUNT 6
	#define DECODING_TESTS_COUNT 7
	#define CONVERSION_TESTS_COUNT 7

	extern const struct least_square_root_test least_square_root_tests[LEAST_SQUARE_ROOT_TESTS_COUNT];
	extern const struct encoding_test encoding_tests[ENCODING_TESTS_COUNT];
	extern const struct decoding_test decoding_tests[DECODING_TESTS_COUNT];
	extern const struct conversion_test conversion_tests[CONVERSION_TESTS_COUNT];

	Suite *create_main_suite(void);
#endif
