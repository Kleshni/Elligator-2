#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <gmp.h>

#include "elligator-2-curve25519.h"

static mpz_t p, divide_plus_p_3_8, divide_minus_p_1_2, divide_minus_p_1_4, square_root_negative_1;
static unsigned long A = 486662;
static mpz_t negative_A;
static mpz_t u, inverted_u;

void Elligator_2_Curve25519_initialize(void) {
	mpz_inits(
		p, divide_plus_p_3_8, divide_minus_p_1_2, divide_minus_p_1_4, square_root_negative_1,
		negative_A,
		u, inverted_u,
		NULL
	);

	// p = 2 ^ 255 - 19

	mpz_set_ui(p, 2);
	mpz_pow_ui(p, p, 255);
	mpz_sub_ui(p, p, 19);

	// divide_plus_p_3_8 = (p + 3) / 8

	mpz_add_ui(divide_plus_p_3_8, p, 3);
	mpz_divexact_ui(divide_plus_p_3_8, divide_plus_p_3_8, 8);

	// divide_minus_p_1_2 = (p - 1) / 2

	mpz_sub_ui(divide_minus_p_1_2, p, 1);
	mpz_divexact_ui(divide_minus_p_1_2, divide_minus_p_1_2, 2);

	// divide_minus_p_1_4 = (p - 1) / 4

	mpz_sub_ui(divide_minus_p_1_4, p, 1);
	mpz_divexact_ui(divide_minus_p_1_4, divide_minus_p_1_4, 4);

	// square_root_negative_1 = 2 ^ divide_minus_p_1_4 (mod p)

	mpz_set_ui(square_root_negative_1, 2);
	mpz_powm(square_root_negative_1, square_root_negative_1, divide_minus_p_1_4, p);

	// negative_A = -A (mod p)

	mpz_sub_ui(negative_A, p, A);

	// u = 2

	mpz_set_ui(u, 2);

	// inverted_u = 1 / u (mod p)

	mpz_invert(inverted_u, u, p);
}

void Elligator_2_Curve25519_destroy(void) {
	mpz_clears(
		p, divide_plus_p_3_8, divide_minus_p_1_2, divide_minus_p_1_4, square_root_negative_1,
		negative_A,
		u, inverted_u,
		NULL
	);
}

static void square_root(mpz_t result, mpz_t x) {
	mpz_t t;

	mpz_init(t);

	// t = x ^ ((p - 1) / 4) (mod p)

	mpz_powm(t, x, divide_minus_p_1_4, p);

	// result := x ^ ((p + 3) / 8) (mod p)

	mpz_powm(result, x, divide_plus_p_3_8, p);

	// If t = -1 (mod p)

	mpz_add_ui(t, t, 1);

	if (mpz_cmp(t, p) == 0) {
		// result := result * square_root(-1) (mod p)

		mpz_mul(result, result, square_root_negative_1);
		mpz_mod(result, result, p);
	}

	// If result > (p - 1) / 2

	if (mpz_cmp(result, divide_minus_p_1_2) > 0) {
		// result := -result (mod p)

		mpz_sub(result, p, result);
	}

	mpz_clear(t);
}

#define POINT_LENGTH 32
#define REPRESENTATIVE_LENGTH 32

bool Elligator_2_Curve25519_encode(uint8_t *representative, const uint8_t *point, bool alternative) {
	mpz_t r, x;
	mpz_t negative_plus_x_A, negative_multiply3_u_x_plus_x_A;

	mpz_inits(
		r, x,
		negative_plus_x_A, negative_multiply3_u_x_plus_x_A,
		NULL
	);

	// x

	mpz_import(x, POINT_LENGTH, -1, 1, 0, 0, point);

	// If x = 0

	if (mpz_cmp_ui(x, 0) == 0) {
		alternative = false;
	}

	// negative_plus_x_A = -(x + A) (mod p)

	mpz_add_ui(negative_plus_x_A, x, A);
	mpz_sub(negative_plus_x_A, p, negative_plus_x_A);

	// negative_multiply3_u_x_plus_x_A = -ux(x + A) (mod p)

	mpz_mul(negative_multiply3_u_x_plus_x_A, u, x);
	mpz_mod(negative_multiply3_u_x_plus_x_A, negative_multiply3_u_x_plus_x_A, p);
	mpz_mul(negative_multiply3_u_x_plus_x_A, negative_multiply3_u_x_plus_x_A, negative_plus_x_A);
	mpz_mod(negative_multiply3_u_x_plus_x_A, negative_multiply3_u_x_plus_x_A, p);

	// If -ux(x + A) is not a square modulo p

	if (mpz_legendre(negative_multiply3_u_x_plus_x_A, p) == -1) {
		return false;
	}

	if (alternative) {
		// r := -(x + A) / x (mod p)

		mpz_invert(r, x, p);
		mpz_mul(r, r, negative_plus_x_A);
	} else {
		// r := -x / (x + A) (mod p)

		mpz_invert(r, negative_plus_x_A, p);
		mpz_mul(r, r, x);
	}

	mpz_mod(r, r, p);

	// r := square_root(r / u) (mod p)

	mpz_mul(r, r, inverted_u);
	mpz_mod(r, r, p);
	square_root(r, r);

	memset(representative, 0, REPRESENTATIVE_LENGTH);
	mpz_export(representative, NULL, -1, 1, 0, 0, r);

	mpz_clears(
		r, x,
		negative_plus_x_A, negative_multiply3_u_x_plus_x_A,
		NULL
	);

	return true;
}

bool Elligator_2_Curve25519_decode(uint8_t *point, bool *alternative, const uint8_t *representative) {
	mpz_t r, x;
	mpz_t v, plus_v_A, t;
	int e;

	mpz_inits(
		r, x,
		v, plus_v_A, t,
		NULL
	);

	// r

	mpz_import(r, REPRESENTATIVE_LENGTH, -1, 1, 0, 0, representative);

	// If r >= (p - 1) / 2

	if (mpz_cmp(r, divide_minus_p_1_2) >= 0) {
		return false;
	}

	// v = -A / (1 + ur ^ 2) (mod p)

	mpz_mul(v, r, r);
	mpz_mod(v, v, p);
	mpz_mul(v, v, u);
	mpz_add_ui(v, v, 1);
	mpz_mod(v, v, p);
	mpz_invert(v, v, p);
	mpz_mul(v, v, negative_A);
	mpz_mod(v, v, p);

	// plus_v_A = v + A (mod p)

	mpz_add_ui(plus_v_A, v, A);

	// t = x ^ 3 + Ax ^ 2 + Bx (mod p)

	mpz_mul(t, v, v);
	mpz_mod(t, t, p);
	mpz_mul(t, t, plus_v_A);
	mpz_add(t, t, v);
	mpz_mod(t, t, p);

	// e = Legendre symbol (t / p)

	e = mpz_legendre(t, p);

	if (e == 1) {
		mpz_set(x, v);
	} else {
		mpz_sub(x, p, v);
		mpz_sub_ui(x, x, A);
		mpz_mod(x, x, p);
	}

	memset(point, 0, POINT_LENGTH);
	mpz_export(point, NULL, -1, 1, 0, 0, x);

	*alternative = e == 1;

	mpz_clears(
		r, x,
		v, plus_v_A, t,
		NULL
	);

	return true;
}
