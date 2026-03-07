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
 * add_image(path, alt, width, height)
 * Appends an image to the feed element.
 *
 * path   - URL or path to the image
 * alt    - alt text for the image
 * width  - display width in pixels, or -1 for default
 * height - display height in pixels, or -1 for default
 */
void add_image(const char *path, const char *alt, int width, int height);

/*
 * add_theme_toggle(label)
 * Creates a text toggle in the header.
 *
 * label - initial text (e.g., ":light")
 * style - CSS style string
 */
void add_theme_toggle(const char *label, const char *style);

/*
 * update_theme_toggle_label(label)
 * Updates the text content of the theme toggle.
 */
void update_theme_toggle_label(const char *label);

#endif
