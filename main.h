#ifndef MAIN_H
	#define MAIN_H

	#include <limits.h>

	#define P_BITS (256) // 255 significant bits + 1 for carry
	#define P_BYTES ((P_BITS + CHAR_BIT - 1) / CHAR_BIT)
	#define P_LIMBS ((P_BITS + GMP_NUMB_BITS - 1) / GMP_NUMB_BITS)
#endif
