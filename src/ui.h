#ifndef PROJECT_SRC_UI_H
#define PROJECT_SRC_UI_H

/*
 * add_paragraph(text)
 * Appends a text paragraph to the feed element.
 *
 * text - the string to display
 */
void add_paragraph(const char *text);

/*
 * add_image(path, alt, scale)
 * Appends an image to the feed element.
 *
 * path   - URL or path to the image
 * alt    - alt text for the image
 * scale  - scale factor (1.0f for original size, 0.5f for half, etc.)
 */
void add_image(const char *path, const char *alt, float scale);

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
