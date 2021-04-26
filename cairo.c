#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <cairo/cairo.h>

/**
 * Calculates the width and height of all lines togheter.
 * */
void measure_block_size(cairo_t *cr, const char *lines[], int *width, int *height)
{
    for (int i = 0; i < 4; i++)
    {
        cairo_text_extents_t te;
        cairo_text_extents(cr, lines[i], &te);

        if (te.width > *width)
            *width = te.width;

        *height = *height + te.height + 10;
    }
}

/**
 * Writes all lines into the cairo surface.
 * */
void write_multiline(cairo_t *cr, int width, int height)
{
    time_t t;
    struct tm *tmp;
    char date_time[50];

    // Get current date and time
    time(&t);
    tmp = localtime(&t);

    // Format date and time as dd/mm/YYYY HH:MM:SS
    strftime(date_time, sizeof(date_time), "%d/%m/%Y %X", tmp);

    const char *lines[] = {"Vinicch",
                           "Hello World",
                           getlogin(),
                           date_time};

    int block_width = 0, block_height = 0, line_height = 0;

    measure_block_size(cr, lines, &block_width, &block_height);

    for (int i = 0; i < 4; i++)
    {
        // Adjust positions
        cairo_text_extents_t te;
        cairo_text_extents(cr, lines[i], &te);

        double x = (width / 2 - block_width / 2 - te.x_bearing);
        double y = (height / 2 - block_height / 2 + line_height);

        // Move to center and show text
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, lines[i]);

        line_height = line_height + te.height + 10;
    }
}

int main()
{
    cairo_surface_t *surface;
    cairo_t *cr;

    int width = 1280;
    int height = 720;

    // Create context
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(surface);

    // Config source settings
    cairo_set_source_rgb(cr, 255, 255, 255);

    // Config mask settings
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 40.0);

    // Write watermark text on surface
    write_multiline(cr, width, height);

    // Write to image, close and exit
    cairo_surface_write_to_png(surface, "image.png");

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return 0;
}