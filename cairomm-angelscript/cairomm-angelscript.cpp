
#include "stdafx.h"
#include "cairomm-angelscript.h"

const double PI =  3.14159265358979323846;

// Dummy test functions
int AddThree(int n) { return n+3; }
int AddFour(int n) { return n+4; }

// library-check function
void CairoCheck_DrawTestImage(std::string filename)
{
#ifdef CAIRO_HAS_SVG_SURFACE

    
    double width = 600;
    double height = 400;
    auto surface = Cairo::SvgSurface::create(filename, width, height);

    auto cr = Cairo::Context::create(surface);

    cr->save(); // save the state of the context
    cr->set_source_rgb(0.86, 0.45, 0.47);
    cr->paint();    // fill image with the color
    cr->restore();  // color is back to black now

    cr->save();
    // draw a border around the image
    cr->set_line_width(20.0);    // make the line wider
    cr->rectangle(0.0, 0.0, width, height);
    cr->stroke();

    cr->set_source_rgba(0.0, 0.0, 0.0, 0.7);
    // draw a circle in the center of the image
    cr->arc(width / 2.0, height / 2.0, 
            height / 4.0, 0.0, 2.0 * PI);
    cr->stroke();

    // draw a diagonal line
    cr->move_to(width / 4.0, height / 4.0);
    cr->line_to(width * 3.0 / 4.0, height * 3.0 / 4.0);
    cr->stroke();
    cr->restore();

    cr->show_page();

    std::cout << "CheckCairoWorks(): Wrote SVG file \"" << filename << "\"" << std::endl;

#else

    std::cout << "CheckCairoWorks(): You must compile cairo with SVG support for this example to work."  << std::endl;

#endif    
}

using namespace std;

bool RegisterCairo(asIScriptEngine *engine)
{
    int r;

    r = engine->SetDefaultNamespace("TestDummy");
    if (r < asSUCCESS)
    {
        cout <<"ERR creating namespace"<<endl;
        return false;
    }

    r = engine->RegisterGlobalFunction("int AddThree(int n)", asFUNCTION(AddThree), asCALL_CDECL); // Dummy test function
    if (r < asSUCCESS)
    {
        cout <<"ERR reg. function AddThree"<<endl;
        return false;
    }

    r = engine->SetDefaultNamespace(""); // Attempt to reset the namespace to global
    if (r != asSUCCESS)
    {
        cout <<"ERR - could not reset namespace"<<endl;
        return false;
    }

    r = engine->RegisterGlobalFunction("int AddFour(int n)", asFUNCTION(AddFour), asCALL_CDECL); // Dummy test function
    if (r < asSUCCESS)
    {
        cout <<"ERR reg. function AddFour"<<endl;
        return false;
    }

    r = engine->SetDefaultNamespace("CairoCheck"); // TEST: can we refer from namespaced function to non-namespaced 'string'?
    if (r != asSUCCESS)
    {
        cout <<"ERR - could not set namespace CairoCheck"<<endl;
        return false;
    }

    r = engine->RegisterGlobalFunction("void DrawTestImage(string)", asFUNCTION(CairoCheck_DrawTestImage), asCALL_CDECL); // library check
    if (r < asSUCCESS)
    {
        cout <<"ERR reg. function CairoCheck_DrawTestImage()"<<endl;
        return false;
    }

    return true;
}
