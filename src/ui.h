#ifndef PROJECT_SRC_UI_H
#define PROJECT_SRC_UI_H

#include <stddef.h>
#include "graph.h"

/*
 * add_paragraph - Appends a text paragraph to the feed element.
 * @text_ptr: pointer to the string in WASM memory
 * @len: length of the string
 */
void add_paragraph(const char *text_ptr, size_t len);

/*
 * add_image - Appends an image to the feed element.
 * @path_ptr: pointer to the image path
 * @path_len: length of the path string
 * @alt_ptr: pointer to the alt text string
 * @alt_len: length of the alt text string
 * @scale: scale factor for the image width
 */
void add_image(const char *path_ptr, size_t path_len,
	       const char *alt_ptr, size_t alt_len, float scale);

/*
 * add_theme_toggle - Creates a text toggle in the header.
 * @label: initial text (e.g., ":light")
 * @style: CSS style string
 */
void add_theme_toggle(const char *label, const char *style);

/*
 * add_footer - Appends a footer element to the body.
 * @year: current year
 * @style: CSS style string
 */
void add_footer(int year, const char *style);

/*
 * clear_feed - Removes all content from the feed element.
 */
void clear_feed(void);

/*
 * add_nav_link - Creates a navigation link in the header.
 * @label: link text
 * @style: CSS style string
 * @id: element ID
 */
void add_nav_link(const char *label, const char *style, const char *id);

/*
 * add_blog_entry - Appends a blog entry link to the feed element.
 * @title: post title
 * @date: post date string
 * @index: index of the post in the global array
 */
void add_blog_entry(const char *title, const char *date, int index);

/*
 * update_theme_toggle_label - Updates the text of the theme toggle button.
 * @label: new text
 */
void update_theme_toggle_label(const char *label);

/*
 * add_named_graph - Injects a predefined graph by its identifier.
 * @name: identifier string
 * @len: length of the identifier
 */
void add_named_graph(const char *name, size_t len);

/*
 * add_bar - Appends a segmented graph bar to the UI.
 * @height: total height in pixels
 * @width: width in pixels
 * @segs: pointer to array of bar_segment structures
 * @n: number of segments
 */
void add_bar(int height, int width, const struct bar_segment *segs, int n);

#endif
