#pragma once

#undef DEBOUNCE
#define DEBOUNCE 15

// Default RGB effect - solid color
#undef RGB_MATRIX_DEFAULT_MODE
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR

// Default brightness - 1/4th. Only applied when EEPROM is reset;
// after that the last brightness set via Fn+Up/Down is kept.
#undef RGB_MATRIX_DEFAULT_VAL
#define RGB_MATRIX_DEFAULT_VAL 64
