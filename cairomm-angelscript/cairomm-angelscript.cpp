
#include "stdafx.h"
#include "cairomm-angelscript.h"

// Dummy test functions
int AddThree(int n) { return n+3; }
int AddFour(int n) { return n+4; }

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

    return true;
}
