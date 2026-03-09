#ifndef PROJECT_SRC_UI_H
#define PROJECT_SRC_UI_H

/*
 * add_paragraph(text_ptr, len)
 * Appends a text paragraph to the feed element using a raw pointer view.
 *
 * text_ptr - pointer to the string in WASM memory
 * len      - length of the string
 */
void add_paragraph(const char *text_ptr, size_t len);

/*
 * add_image(path_ptr, path_len, alt_ptr, alt_len, scale)
 * Appends an image using raw pointer views.
 */
void add_image(const char *path_ptr, size_t path_len,
	       const char *alt_ptr, size_t alt_len, float scale);

/*
 * add_theme_toggle(label)
 * Creates a text toggle in the header.
 *
 * label - initial text (e.g., ":light")
 * style - CSS style string
 */
void add_theme_toggle(const char *label, const char *style);

/*
 * add_footer(year, style)
 * Appends a footer element to the body.
 */
void add_footer(int year, const char *style);

/*
 * clear_feed()
 * Removes all content from the feed element.
 */
void clear_feed(void);

/*
 * add_nav_link(label, style, id)
 * Creates a text link in the header.
 */
void add_nav_link(const char *label, const char *style, const char *id);

/*
 * add_blog_entry(title, date, index)
 * Appends a blog entry link to the feed element.
 */
void add_blog_entry(const char *title, const char *date, int index);

/*
 * update_theme_toggle_label(label)
 */
void update_theme_toggle_label(const char *label);

#endif
