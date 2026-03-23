#ifndef PROJECT_SRC_STATE_H
#define PROJECT_SRC_STATE_H

#include <stdbool.h>
#include "config.h"

enum page_state {
	PAGE_INITIAL,
	PAGE_HOME,
	PAGE_BLOG_INDEX,
	PAGE_ARTICLE,
	PAGE_404
};

struct site_state {
	float runtime;
	bool is_dark;
	const struct theme *theme;
	enum page_state page;
};

extern struct site_state state;

#endif
