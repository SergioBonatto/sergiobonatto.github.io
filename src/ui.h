#ifndef UI_H
#define UI_H

/*
 * add_paragraph(text)
 * Appends a text paragraph to the feed element.
 *
 * text - the string to display
 */
void add_paragraph(const char* text);

/*
 * add_image(path, alt, width, height)
 * Appends an image to the feed element.
 *
 * path   - URL or path to the image
 * alt    - alt text for the image
 * width  - display width in pixels, or -1 for default
 * height - display height in pixels, or -1 for default
 */
void add_image(const char* path, const char* alt, int width, int height);

#endif
