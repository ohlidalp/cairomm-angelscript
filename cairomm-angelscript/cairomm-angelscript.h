/*
  'cairomm-angelscript'
  Bindings between Cairo graphics library and AngelScript scripting component.

  Copyright (C) 2018 Petr Ohlidal <petr@ohlidal.cz>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#ifndef ANGELSCRIPT_H 
// Avoid having to inform include path if header is already include before
#include <angelscript.h>
#endif

namespace cairomm_angelscript
{

class SetupError: public std::runtime_error
{
public:
    SetupError(const char* msg): runtime_error(msg) {}
    //const char* what() { return std::runtime_error::what(); }
};

/// @throws SetupException on error
void RegisterInterface(asIScriptEngine *engine);

} // namespace CairoAngelscript

