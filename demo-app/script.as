void main()
{
    string filename = "demo-script.svg";
    double width = 600;
    double height = 400;

    Print("creating surface...\n");
    Cairo::SvgSurfaceRef surface = Cairo::SvgSurface::create(filename, width, height); 
    Print("creating context...\n");
    Cairo::ContextRef ctx = Cairo::Context::create(surface);

    Print("saving state...\n");
    ctx.save(); // save the state of the context
    Print("setting source rgb...\n");
    ctx.set_source_rgb(0.86, 0.45, 0.47);
    ctx.paint();    // fill image with the color
    ctx.restore();  // color is back to black now

    ctx.save();
    // draw a border around the image
    ctx.set_line_width(20.0);    // make the line wider
    ctx.rectangle(0.0, 0.0, width, height);
    ctx.stroke();

    ctx.set_source_rgba(0.0, 0.0, 0.0, 0.7);
    // draw a circle in the center of the image
    ctx.arc(width / 2.0, height / 2.0, height / 4.0, 0.0, 2.0 * 3.14);
    ctx.stroke();

    // draw a diagonal line
    ctx.move_to(width / 4.0, height / 4.0);
    ctx.line_to(width * 3.0 / 4.0, height * 3.0 / 4.0);
    ctx.stroke();
    ctx.restore();

    Print("writing image to disk...\n");
    ctx.show_page();

    Print("Wrote SVG file \"" + filename + "\"");

}
