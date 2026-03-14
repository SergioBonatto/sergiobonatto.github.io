#ifndef PROJECT_SRC_GRAPH_H
#define PROJECT_SRC_GRAPH_H

enum bar_seg_style {
	BAR_SEG_SOLID,
	BAR_SEG_HATCHED,
	BAR_SEG_EMPTY,
};

struct bar_segment {
	float pct;
	const char *color_var;
	float opacity;
	enum bar_seg_style style;
};

#endif
