#include <confuse.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <cairo/cairo.h>

/**
 * Initializes a config structure.
 **/
cfg_t *init_config()
{
    cfg_opt_t source_opts[] = {CFG_INT("red", 255, CFGF_NONE),
                               CFG_INT("green", 255, CFGF_NONE),
                               CFG_INT("blue", 255, CFGF_NONE),
                               CFG_END()};

    cfg_opt_t font_opts[] = {CFG_STR("face", "Sans", CFGF_NONE),
                             CFG_INT("slant", 0, CFGF_NONE),
                             CFG_INT("weight", 0, CFGF_NONE),
                             CFG_FLOAT("size", 40.0, CFGF_NONE),
                             CFG_END()};

    cfg_opt_t line_opts[] = {CFG_STR("text", "Hello World", CFGF_NONE),
                             CFG_END()};

    cfg_opt_t opts[] = {CFG_INT("width", 800, CFGF_NONE),
                        CFG_INT("height", 600, CFGF_NONE),
                        CFG_SEC("source", source_opts, CFGF_NONE),
                        CFG_SEC("font", font_opts, CFGF_NONE),
                        CFG_SEC("line", line_opts, CFGF_MULTI),
                        CFG_END()};

    cfg_t *cfg;
    cfg = cfg_init(opts, CFGF_NONE);

    return cfg;
}

/**
 * Returns the current formatted date.
 */
void now(char *date_time, int size, const char *format)
{
    time_t t;
    struct tm *tmp;

    time(&t);
    tmp = localtime(&t);

    // Format date and time as dd/mm/YYYY HH:MM:SS
    strftime(date_time, size, format, tmp);
}

/**
 * Calculates the width and height of all lines togheter.
 **/
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
 **/
void write_multiline(cairo_t *cr, int width, int height, cfg_t *config)
{
    // Get the first two lines
    cfg_t *line_one_config = cfg_getnsec(config, "line", 0);
    cfg_t *line_two_config = cfg_getnsec(config, "line", 1);

    char *line_one_text = cfg_getstr(line_one_config, "text");
    char *line_two_text = cfg_getstr(line_two_config, "text");

    // Get username
    char *username = getlogin();

    char date_time[50];
    now(date_time, sizeof(date_time), "%d/%m/%Y %X");

    // Build multiline text
    const char *lines[] = {line_one_text,
                           line_two_text,
                           username,
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
    // Init config
    cfg_t *config = init_config();

    int parse_result = cfg_parse(config, "config.conf");

    if (parse_result != CFG_SUCCESS)
        return parse_result;

    cfg_t *source_config = cfg_getsec(config, "source");
    cfg_t *font_config = cfg_getsec(config, "font");

    int width = cfg_getint(config, "width");
    int height = cfg_getint(config, "height");

    int red = cfg_getint(source_config, "red");
    int green = cfg_getint(source_config, "green");
    int blue = cfg_getint(source_config, "blue");

    char *fontFace = cfg_getstr(font_config, "face");
    int slant = cfg_getint(font_config, "slant");
    int weight = cfg_getint(font_config, "weight");
    double size = cfg_getfloat(font_config, "size");

    // Begin drawing
    cairo_surface_t *surface;
    cairo_t *cr;

    // Create context
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(surface);

    // Config source settings
    cairo_set_source_rgb(cr, red, green, blue);

    // Config mask settings
    cairo_select_font_face(cr, fontFace, slant, weight);
    cairo_set_font_size(cr, size);

    // Write watermark text on surface
    write_multiline(cr, width, height, config);

    // Write to image, close and exit
    cairo_surface_write_to_png(surface, "image.png");

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    cfg_free(config);

    return 0;
}