#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <gmp.h>

#include "main.h"
#include "elligator-2-curve25519.h"

static const unsigned char p_bytes[P_BYTES] = {
	0xed, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f
};

static const unsigned char negative_1_bytes[P_BYTES] = {
	0xec, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f
};

static const unsigned char negative_2_bytes[P_BYTES] = {
	0xeb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f
};

static const unsigned char divide_negative_1_2_bytes[P_BYTES] = {
	0xf6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f
};

static const unsigned char divide_plus_p_3_8_bytes[P_BYTES] = {
	0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f
};

static const unsigned char divide_minus_p_1_2_bytes[P_BYTES] = {
	0xf6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f
};

static const unsigned char square_root_negative_1_bytes[P_BYTES] = {
	0xb0, 0xa0, 0x0e, 0x4a, 0x27, 0x1b, 0xee, 0xc4, 0x78, 0xe4, 0x2f, 0xad, 0x06, 0x18, 0x43, 0x2f,
	0xa7, 0xd7, 0xfb, 0x3d, 0x99, 0x00, 0x4d, 0x2b, 0x0b, 0xdf, 0xc1, 0x4f, 0x80, 0x24, 0x83, 0x2b
};

static const unsigned char A_bytes[P_BYTES] = {
	0x06, 0x6d, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char negative_A_bytes[P_BYTES] = {
	0xe7, 0x92, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f
};

static const unsigned char u_bytes[P_BYTES] = {
	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char inverted_u_bytes[P_BYTES] = {
	0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f
};

static const unsigned char d_bytes[P_BYTES] = {
	0xa3, 0x78, 0x59, 0x13, 0xca, 0x4d, 0xeb, 0x75, 0xab, 0xd8, 0x41, 0x41, 0x4d, 0x0a, 0x70, 0x00,
	0x98, 0xe8, 0x79, 0x77, 0x79, 0x40, 0xc7, 0x8c, 0x73, 0xfe, 0x6f, 0x2b, 0xee, 0x6c, 0x03, 0x52
};

static mp_limb_t p[P_LIMBS];
static mp_limb_t negative_1[P_LIMBS];
static mp_limb_t negative_2[P_LIMBS];
static mp_limb_t divide_negative_1_2[P_LIMBS];
static mp_limb_t divide_plus_p_3_8[P_LIMBS];
static mp_limb_t divide_minus_p_1_2[P_LIMBS];
static mp_limb_t square_root_negative_1[P_LIMBS];
static mp_limb_t A[P_LIMBS];
static mp_limb_t negative_A[P_LIMBS];
static mp_limb_t u[P_LIMBS];
static mp_limb_t inverted_u[P_LIMBS];
static mp_limb_t d[P_LIMBS];

static mp_size_t scratch_space_length;

static void decode_bytes(mp_limb_t *number, const uint8_t *bytes) {
	mp_limb_t scratch_space[1];

	for (size_t i = 0; i < P_BYTES; ++i) {
		mpn_lshift(number, number, P_LIMBS, 8);
		mpn_sec_add_1(number, number, 1, bytes[P_BYTES - i - 1], scratch_space);
	}
}

// Erases the number

static void encode_bytes(uint8_t *bytes, mp_limb_t *number) {
	for (size_t i = 0; i < P_BYTES; ++i) {
		bytes[P_BYTES - i - 1] = mpn_lshift(number, number, P_LIMBS, 8);
	}
}

void Elligator_2_Curve25519_initialize(void) {
	static bool initialized = false;

	if (initialized) {
		return;
	}

	decode_bytes(p, p_bytes);
	decode_bytes(negative_1, negative_1_bytes);
	decode_bytes(negative_2, negative_2_bytes);
	decode_bytes(divide_negative_1_2, divide_negative_1_2_bytes);
	decode_bytes(divide_plus_p_3_8, divide_plus_p_3_8_bytes);
	decode_bytes(divide_minus_p_1_2, divide_minus_p_1_2_bytes);
	decode_bytes(square_root_negative_1, square_root_negative_1_bytes);
	decode_bytes(A, A_bytes);
	decode_bytes(negative_A, negative_A_bytes);
	decode_bytes(u, u_bytes);
	decode_bytes(inverted_u, inverted_u_bytes);
	decode_bytes(d, d_bytes);

	mp_size_t scratch_space_lengths[] = {
		// For least_square_root

		mpn_sec_powm_itch(P_LIMBS, P_BITS - 1, P_LIMBS),
		mpn_sec_sqr_itch(P_LIMBS),
		mpn_sec_div_r_itch(P_LIMBS + P_LIMBS, P_LIMBS),
		mpn_sec_sub_1_itch(P_LIMBS),
		mpn_sec_mul_itch(P_LIMBS, P_LIMBS),

		// For Elligator_2_Curve25519_encode

		mpn_sec_powm_itch(P_LIMBS, P_BITS - 1, P_LIMBS),
		mpn_sec_mul_itch(P_LIMBS, P_LIMBS),
		mpn_sec_div_r_itch(P_LIMBS + P_LIMBS, P_LIMBS),
		mpn_sec_sqr_itch(P_LIMBS),
		mpn_sec_sub_1_itch(P_LIMBS),

		// For Elligator_2_Curve25519_decode

		mpn_sec_sqr_itch(P_LIMBS),
		mpn_sec_div_r_itch(P_LIMBS + P_LIMBS, P_LIMBS),
		mpn_sec_div_r_itch(P_LIMBS, P_LIMBS),
		mpn_sec_mul_itch(P_LIMBS, P_LIMBS),
		mpn_sec_add_1_itch(P_LIMBS),
		mpn_sec_powm_itch(P_LIMBS, P_BITS - 1, P_LIMBS),

		// For Elligator_2_Curve25519_convert_from_Ed25519

		mpn_sec_sqr_itch(P_LIMBS),
		mpn_sec_div_r_itch(P_LIMBS + P_LIMBS, P_LIMBS),
		mpn_sec_mul_itch(P_LIMBS, P_LIMBS),
		mpn_sec_add_1_itch(P_LIMBS),
		mpn_sec_powm_itch(P_LIMBS, P_BITS - 1, P_LIMBS),
		mpn_sec_sub_1_itch(P_LIMBS)
	};

	for (size_t i = 0; i < sizeof scratch_space_lengths / sizeof *scratch_space_lengths; ++i) {
		if (scratch_space_lengths[i] > scratch_space_length) {
			scratch_space_length = scratch_space_lengths[i];
		}
	}

	initialized = true;
}

void Elligator_2_Curve25519_destroy(void) {}

// Returns trash if the number is a quadratic non-residue

static void least_square_root(mp_limb_t *root, const mp_limb_t *number, mp_limb_t *scratch_space) {
	mp_limb_t a[P_LIMBS + P_LIMBS];
	mp_limb_t b[P_LIMBS];

	// root := number ^ ((p + 3) / 8)

	mpn_add_n(b, number, p, P_LIMBS); // The next function requires a nonzero input
	mpn_sec_powm(root, b, P_LIMBS, divide_plus_p_3_8, P_BITS - 1, p, P_LIMBS, scratch_space);

	// If root ^ 2 != number, root := root * square_root(-1)

	mpn_sec_sqr(a, root, P_LIMBS, scratch_space);
	mpn_sec_div_r(a, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);
	mpn_sub_n(b, a, number, P_LIMBS);

	mp_limb_t condition = mpn_sec_sub_1(b, b, P_LIMBS, 1, scratch_space) ^ 1;

	mpn_sec_mul(a, root, P_LIMBS, square_root_negative_1, P_LIMBS, scratch_space);
	mpn_sec_div_r(a, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);

	mpn_cnd_swap(condition, root, a, P_LIMBS);

	// If root > (p - 1) / 2, root := -root

	condition = mpn_sub_n(a, divide_minus_p_1_2, root, P_LIMBS);

	mpn_sub_n(a, p, root, P_LIMBS); // If root = 0, a := p

	mpn_cnd_swap(condition, root, a, P_LIMBS);
}

bool Elligator_2_Curve25519_encode(uint8_t *representative, const uint8_t *point, bool high_y) {
	mp_limb_t scratch_space[scratch_space_length];

	mp_limb_t a[P_LIMBS + P_LIMBS];
	mp_limb_t b[P_LIMBS + P_LIMBS];
	mp_limb_t c[P_LIMBS + P_LIMBS];

	// a := point

	decode_bytes(a, point);

	// b := -a / (a + A), or b := p if a = 0

	mpn_add_n(b, a, A, P_LIMBS);
	mpn_sec_powm(c, b, P_LIMBS, negative_2, P_BITS - 1, p, P_LIMBS, scratch_space);
	mpn_sec_mul(b, c, P_LIMBS, a, P_LIMBS, scratch_space);
	mpn_sec_div_r(b, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);
	mpn_sub_n(b, p, b, P_LIMBS);

	// If high_y = true, b := 1 / b or b := 0 if it was = p

	mpn_sec_powm(c, b, P_LIMBS, negative_2, P_BITS - 1, p, P_LIMBS, scratch_space);
	mpn_cnd_swap(high_y, b, c, P_LIMBS);

	// c := b / u

	mpn_sec_mul(c, b, P_LIMBS, inverted_u, P_LIMBS, scratch_space);
	mpn_sec_div_r(c, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);

	// If c is a square modulo p, b := least_square_root(c)

	least_square_root(b, c, scratch_space);

	// Determine, whether b ^ 2 = c

	mpn_sec_sqr(a, b, P_LIMBS, scratch_space);
	mpn_sec_div_r(a, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);
	mpn_sub_n(a, a, c, P_LIMBS);

	bool result = mpn_sec_sub_1(a, a, P_LIMBS, 1, scratch_space);

	encode_bytes(representative, b);

	return result;
}

bool Elligator_2_Curve25519_decode(uint8_t *point, bool *high_y, const uint8_t *representative) {
	mp_limb_t scratch_space[scratch_space_length];

	mp_limb_t a[P_LIMBS + P_LIMBS];
	mp_limb_t b[P_LIMBS + P_LIMBS];
	mp_limb_t c[P_LIMBS];
	mp_limb_t e[P_LIMBS + P_LIMBS];

	// a := representative

	decode_bytes(a, representative);

	// Determine whether a < (p - 1) / 2

	bool result = mpn_sub_n(b, divide_minus_p_1_2, a, P_LIMBS) ^ 1;

	// b := -A / (1 + u * a ^ 2)

	mpn_sec_sqr(b, a, P_LIMBS, scratch_space);
	mpn_sec_div_r(b, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);
	mpn_sec_mul(a, u, P_LIMBS, b, P_LIMBS, scratch_space);
	mpn_sec_div_r(a, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);
	mpn_sec_add_1(b, a, P_LIMBS, 1, scratch_space);
	mpn_sec_powm(a, b, P_LIMBS, negative_2, P_BITS - 1, p, P_LIMBS, scratch_space);
	mpn_sec_mul(b, a, P_LIMBS, negative_A, P_LIMBS, scratch_space);
	mpn_sec_div_r(b, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);

	// a := b ^ 3 + A * b ^ 2 + b (with 1-bit overflow)

	mpn_sec_sqr(a, b, P_LIMBS, scratch_space);
	mpn_sec_div_r(a, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);
	mpn_add_n(c, b, A, P_LIMBS);
	mpn_sec_mul(e, c, P_LIMBS, a, P_LIMBS, scratch_space);
	mpn_sec_div_r(e, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);
	mpn_add_n(a, e, b, P_LIMBS);

	// If a is a quadratic residue modulo p, point := b and high_y := 1
	// Otherwise point := -b - A and high_y := 0

	mpn_sub_n(c, p, b, P_LIMBS);
	mpn_add_n(c, c, negative_A, P_LIMBS);
	mpn_sec_div_r(c, P_LIMBS, p, P_LIMBS, scratch_space);

	mpn_sec_powm(e, a, P_LIMBS, divide_minus_p_1_2, P_BITS - 1, p, P_LIMBS, scratch_space);
	*high_y = mpn_sub_n(e, e, divide_minus_p_1_2, P_LIMBS);

	mpn_cnd_swap(*high_y, b, c, P_LIMBS);

	encode_bytes(point, c);

	return result;
}

bool Elligator_2_Curve25519_convert_from_Ed25519(uint8_t *point, const uint8_t *source) {
	mp_limb_t scratch_space[scratch_space_length];

	mp_limb_t y[P_LIMBS];
	mp_limb_t a[P_LIMBS + P_LIMBS];
	mp_limb_t b[P_LIMBS + P_LIMBS];
	mp_limb_t c[P_LIMBS + P_LIMBS];

	uint8_t y_bytes[P_BYTES];

	memcpy(y_bytes, source, 31);

	y_bytes[31] = source[31] & 0x7f;

	decode_bytes(y, y_bytes);

	// Check if y < p

	bool result = mpn_sub_n(a, y, p, P_LIMBS);

	// a := (y ^ 2 - 1) / (1 + d * y ^ 2)

	mpn_sec_sqr(a, y, P_LIMBS, scratch_space);
	mpn_sec_div_r(a, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);
	mpn_sec_mul(b, a, P_LIMBS, d, P_LIMBS, scratch_space);
	mpn_sec_add_1(b, b, P_LIMBS, 1, scratch_space);
	mpn_sec_div_r(b, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);
	mpn_sec_powm(c, b, P_LIMBS, negative_2, P_BITS - 1, p, P_LIMBS, scratch_space);
	mpn_add_n(b, a, negative_1, P_LIMBS);
	mpn_sec_mul(a, b, P_LIMBS, c, P_LIMBS, scratch_space);
	mpn_sec_div_r(a, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);

	// Check, whether a is a square modulo p (including a = 0)

	mpn_add_n(a, a, p, P_LIMBS);
	mpn_sec_powm(b, a, P_LIMBS, divide_negative_1_2, P_BITS - 1, p, P_LIMBS, scratch_space);

	result &= mpn_sub_n(c, b, divide_minus_p_1_2, P_LIMBS);

	// If a = p, the parity bit must be 0

	mpn_sub_n(a, a, p, P_LIMBS);

	result ^= mpn_sec_sub_1(a, a, P_LIMBS, 1, scratch_space) & source[31] >> 7;

	// If y != 1, c := (1 + y) / (1 - y), otherwise c := 0

	mpn_sub_n(a, p, y, P_LIMBS);
	mpn_sec_add_1(a, a, P_LIMBS, 1, scratch_space);
	mpn_sec_powm(b, a, P_LIMBS, negative_2, P_BITS - 1, p, P_LIMBS, scratch_space);
	mpn_sec_add_1(a, y, P_LIMBS, 1, scratch_space);
	mpn_sec_mul(c, a, P_LIMBS, b, P_LIMBS, scratch_space);
	mpn_sec_div_r(c, P_LIMBS + P_LIMBS, p, P_LIMBS, scratch_space);

	encode_bytes(point, c);

	return result;
}

#ifdef WITH_TESTS
	#include <check.h>

	#include "tests.h"

	static void core_case_setup(void) {
		Elligator_2_Curve25519_initialize();
	}

	START_TEST(test_least_square_root) {
		mp_limb_t scratch_space[scratch_space_length];

		mp_limb_t number[P_LIMBS];
		mp_limb_t expected_root[P_LIMBS];
		mp_limb_t root[P_LIMBS];

		decode_bytes(number, least_square_root_tests[_i].number);
		decode_bytes(expected_root, least_square_root_tests[_i].root);

		least_square_root(root, number, scratch_space);

		if (least_square_root_tests[_i].result) {
			ck_assert_int_eq(mpn_cmp(expected_root, root, P_LIMBS), 0);
		}
	} END_TEST

	START_TEST(test_encoding) {
		uint8_t representative[ELLIGATOR_2_CURVE25519_REPRESENTATIVE_LENGTH] = {0};

		ck_assert_int_eq(
			Elligator_2_Curve25519_encode(representative, encoding_tests[_i].point, encoding_tests[_i].high_y),
			encoding_tests[_i].result
		);

		if (encoding_tests[_i].result) {
			ck_assert_int_eq(
				memcmp(representative, encoding_tests[_i].representative, ELLIGATOR_2_CURVE25519_REPRESENTATIVE_LENGTH),
				0
			);
		}
	} END_TEST

	START_TEST(test_decoding) {
		uint8_t point[ELLIGATOR_2_CURVE25519_POINT_LENGTH] = {0};
		bool high_y = false;

		ck_assert_int_eq(
			Elligator_2_Curve25519_decode(point, &high_y, decoding_tests[_i].representative),
			decoding_tests[_i].result
		);

		ck_assert_int_eq(memcmp(point, decoding_tests[_i].point, ELLIGATOR_2_CURVE25519_POINT_LENGTH), 0);
		ck_assert_int_eq(high_y, decoding_tests[_i].high_y);
	} END_TEST

	START_TEST(test_conversion) {
		uint8_t point[ELLIGATOR_2_CURVE25519_POINT_LENGTH] = {0};

		ck_assert_int_eq(
			Elligator_2_Curve25519_convert_from_Ed25519(point, conversion_tests[_i].source),
			conversion_tests[_i].result
		);

		if (conversion_tests[_i].result) {
			ck_assert_int_eq(memcmp(point, conversion_tests[_i].point, ELLIGATOR_2_CURVE25519_POINT_LENGTH), 0);
		}
	} END_TEST

	Suite *create_main_suite(void) {
		TCase *core_case = tcase_create("Core");

		tcase_add_unchecked_fixture(core_case, core_case_setup, NULL);
		tcase_add_loop_test(core_case, test_least_square_root, 0, LEAST_SQUARE_ROOT_TESTS_COUNT);
		tcase_add_loop_test(core_case, test_encoding, 0, ENCODING_TESTS_COUNT);
		tcase_add_loop_test(core_case, test_decoding, 0, DECODING_TESTS_COUNT);
		tcase_add_loop_test(core_case, test_conversion, 0, CONVERSION_TESTS_COUNT);

		Suite *result = suite_create("Main");

		suite_add_tcase(result, core_case);

		return result;
	}
#endif
