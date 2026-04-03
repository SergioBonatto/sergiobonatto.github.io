#ifndef PROJECT_SRC_MATH_H
#define PROJECT_SRC_MATH_H

#include "buffer.h"

/**
 * math_to_mathml - Converts a LaTeX string to MathML and appends it to the buffer.
 * @b: The buffer to append to.
 * @latex: The LaTeX source string.
 * @len: Length of the LaTeX string.
 * @display_mode: If true, wraps in <math display="block">, otherwise <math>.
 */
void math_to_mathml(Buffer *b, const char *latex, size_t len, bool display_mode);

#endif
