
#include "stdafx.h"
#include "cairomm-angelscript.h"
#include <string>

// ================================================================================================
//     Registration helper class
// ================================================================================================

class Helper
{
public:
    Helper(asIScriptEngine* e): m_engine(e), m_obj_name(nullptr) {}

    void SetActiveObject(const char* name)
    {
        m_obj_name = name;
    }

    void SetNamespace(const char* name)
    {
        int res = m_engine->SetDefaultNamespace(name);
        if (res < asSUCCESS)
        {
            throw cairomm_angelscript::SetupError("SetDefaultNamespace() failed"); // TODO: more descriptive!
        }
    }

    void RegObject(const char* name, size_t size, asDWORD flags)
    {
        int res = m_engine->RegisterObjectType(name, size, flags);
        if (res < asSUCCESS)
        {
            throw cairomm_angelscript::SetupError("RegisterObjectType() failed"); // TODO: more descriptive!
        }
        this->SetActiveObject(name);
    }

    void RegBehaviour(asEBehaviours behav, const char* decl, const asSFuncPtr & ptr, asDWORD flags = asCALL_CDECL_OBJFIRST)
    {
        int res = m_engine->RegisterObjectBehaviour(m_obj_name, behav, decl, ptr, flags);
        if (res < asSUCCESS)
        {
            throw cairomm_angelscript::SetupError("RegisterObjectBehaviour() failed"); // TODO: more descriptive!
        }
    }

    void RegConstructor(const char* decl, const asSFuncPtr & ptr)
    {
        this->RegBehaviour(asBEHAVE_CONSTRUCT, decl, ptr);
    }

    void RegDestructor(const char* decl, const asSFuncPtr & ptr)
    {
        this->RegBehaviour(asBEHAVE_DESTRUCT, decl, ptr);
    }

    void RegMethod(const char* decl, const asSFuncPtr & ptr, asDWORD flags = asCALL_CDECL_OBJFIRST)
    {
        int res = m_engine->RegisterObjectMethod(m_obj_name, decl, ptr, flags);
        if (res < asSUCCESS)
        {
            throw cairomm_angelscript::SetupError("RegisterObjectMethod() failed"); // TODO: more descriptive!
        }
    }

    void RegFunction(const char* name, const asSFuncPtr & ptr, asDWORD flags = asCALL_CDECL)
    {
        int res = m_engine->RegisterGlobalFunction(name, ptr, flags);
        if (res < asSUCCESS)
        {
            throw cairomm_angelscript::SetupError("RegisterGlobalFunction() failed"); // TODO: more descriptive!
        }
    }
private:
    asIScriptEngine* m_engine;
    const char*      m_obj_name;
};

// ================================================================================================
//     Wrappers (they are inevitable)
//     see https://www.gamedev.net/forums/topic/540419-custom-smartpointers-and-angelscript-/
// ================================================================================================

typedef Cairo::RefPtr<Cairo::Context>    CtxRef;
typedef Cairo::RefPtr<Cairo::SvgSurface> SvgSurfRef;

void SvgSurfRef_ctor     (void* memory)                              { new(memory) SvgSurfRef(); }       // placement-new operator
void SvgSurfRef_copy_ctor(void* memory, const SvgSurfRef& other)     { new(memory) SvgSurfRef(other); }  // placement-new operator
void SvgSurfRef_dtor     (void* memory)                              { ((SvgSurfRef*)memory)->~SvgSurfRef(); }

void CtxRef_ctor         (void* memory)                              { new(memory) CtxRef(); }        // placement-new operator
void CtxRef_copy_ctor    (void* memory, const CtxRef& other)         { new(memory) CtxRef(other); }   // placement-new operator
void CtxRef_dtor         (void* memory)                              { static_cast<CtxRef*>(memory)->~shared_ptr(); }

// Pseudo-member functions must accept `this` as `const&` (otherwise the program crashes)
void Ctx_save            (const CtxRef& ctx)                                { ctx->save(); }
void Ctx_paint           (const CtxRef& ctx)                                { ctx->paint(); }
void Ctx_restore         (const CtxRef& ctx)                                { ctx->restore(); }
void Ctx_stroke          (const CtxRef& ctx)                                { ctx->stroke(); }
void Ctx_show_page       (const CtxRef& ctx)                                { ctx->show_page(); }
void Ctx_move_to         (const CtxRef& ctx, double x, double y)            { ctx->move_to(x,y); }
void Ctx_line_to         (const CtxRef& ctx, double x, double y)            { ctx->line_to(x,y); }
void Ctx_set_source_rgb  (const CtxRef& ctx, double r, double g, double b)  { ctx->set_source_rgb(r,g,b); }
void Ctx_set_line_width  (const CtxRef& ctx, double w)                      { ctx->set_line_width(w); }
void Ctx_set_source_rgba (const CtxRef& ctx, double r, double g, double b, double a) { ctx->set_source_rgba(r,g,b,a); }
void Ctx_rectangle       (const CtxRef& ctx, double x, double y, double w, double h) { ctx->rectangle(x,y,w,h); }
void Ctx_arc             (const CtxRef& ctx, double xc, double yc, double radius, double a1, double a2) { ctx->arc(xc, yc, radius, a1, a2); }

// Angelscript only supports polymorphism for reference types
CtxRef Ctx_from_Svg      (SvgSurfRef surface)                        { return Cairo::Context::create(surface); }


// ================================================================================================
//     Public 'register!' function
// ================================================================================================

void cairomm_angelscript::RegisterInterface(asIScriptEngine *engine)
{
    Helper h(engine);

    h.SetNamespace("Cairo");

    // SvgSurface
    h.RegObject("SvgSurfaceRef", sizeof(SvgSurfRef), asOBJ_VALUE | asGetTypeTraits<SvgSurfRef>());
    
    h.RegConstructor("void f()",                                        asFUNCTION(SvgSurfRef_ctor));
    h.RegConstructor("void f(const SvgSurfaceRef &in)",                 asFUNCTION(SvgSurfRef_copy_ctor));
    h.RegDestructor ("void f()",                                        asFUNCTION(SvgSurfRef_dtor));

    // Context
    h.RegObject("ContextRef", sizeof(CtxRef), asOBJ_VALUE | asGetTypeTraits<CtxRef>());

    h.RegConstructor("void f()",                                        asFUNCTION(CtxRef_ctor));
    h.RegConstructor("void f(const ContextRef &in)",                    asFUNCTION(CtxRef_copy_ctor));
    h.RegDestructor ("void f()",                                        asFUNCTION(CtxRef_dtor));

    h.RegMethod("void save            ()",                              asFUNCTION(Ctx_save));
    h.RegMethod("void paint           ()",                              asFUNCTION(Ctx_paint));
    h.RegMethod("void restore         ()",                              asFUNCTION(Ctx_restore));
    h.RegMethod("void stroke          ()",                              asFUNCTION(Ctx_stroke));
    h.RegMethod("void show_page       ()",                              asFUNCTION(Ctx_show_page));
    h.RegMethod("void move_to         (double x, double y)",            asFUNCTION(Ctx_move_to));
    h.RegMethod("void line_to         (double x, double y)",            asFUNCTION(Ctx_line_to));
    h.RegMethod("void set_line_width  (double w)",                      asFUNCTION(Ctx_set_line_width));
    h.RegMethod("void set_source_rgb  (double r, double g, double b)",  asFUNCTION(Ctx_set_source_rgb));
    h.RegMethod("void set_source_rgba (double r, double g, double b, double a)", asFUNCTION(Ctx_set_source_rgba));
    h.RegMethod("void rectangle       (double x, double y, double w, double h)", asFUNCTION(Ctx_rectangle));
    h.RegMethod("void arc             (double xc, double yc, double radius, double a1, double a2)", asFUNCTION(Ctx_arc));

    // Static functions
    h.SetNamespace("Cairo::SvgSurface");
    h.RegFunction("SvgSurfaceRef create(string filename, double w, double h)", asFUNCTION(Cairo::SvgSurface::create));

    h.SetNamespace("Cairo::Context");
    h.RegFunction("ContextRef create(SvgSurfaceRef)", asFUNCTION(Ctx_from_Svg));
}
