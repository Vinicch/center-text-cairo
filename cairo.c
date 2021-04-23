#include <stdio.h>
#include <cairo/cairo.h>

int main(void)
{
    cairo_surface_t *surface;
    cairo_t *cr;

    int width = 1280;
    int height = 720;

    const char *message = "Hello World";

    // Create canvas
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(surface);

    // Config font settings
    cairo_set_source_rgb(cr, 255, 255, 255);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 40.0);

    // Adjust positions
    cairo_text_extents_t te;
    cairo_text_extents(cr, message, &te);

    double x = (width / 2 - te.width / 2 - te.x_bearing);
    double y = (height / 2 + te.height / 2);

    // Move to center and show text
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, message);

    // Write to image, close and exit
    cairo_surface_write_to_png(surface, "image.png");

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return 0;
}