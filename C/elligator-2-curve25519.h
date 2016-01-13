extern void Elligator_2_Curve25519_initialize(void);
extern void Elligator_2_Curve25519_destroy(void);
extern bool Elligator_2_Curve25519_encode(uint8_t *representative, const uint8_t *point, bool alternative);
extern bool Elligator_2_Curve25519_decode(uint8_t *point, bool *alternative, const uint8_t *representative);
