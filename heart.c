#include <stdlib.h>

struct love {
    int beats;
};

int main(void) {
    struct love *heart = malloc(sizeof *heart);

    heart = NULL; /* brapao */

    return 0;
}
