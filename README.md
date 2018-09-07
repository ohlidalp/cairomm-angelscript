# CairoMM-AngelScript bindings README

Initial working demo project - bindings to create SVG image, add some elements and save the file.

Only builds as Debug using Windows 10 + MSVC 2017.

## Dependencies

Expected in this layout and versions:

```
/3rd_party
    /angelscript-2-32-0
        /include/angelscript.h
        /lib/angelscriptd.lib

    /cairomm-1-15-5
        /bin/Debug/cairomm-vc150-d-1_16.dll
        /include ...
        /lib/Debug/cairomm-vc150-d-1_16.lib
    
    /cairo-preshing-1.15.10
        /bin/x86/cairo.dll
        /include ...
        /lib/x86/cairo.lib
    
    /freetype-2.9.1
        /include ...
    
    /libsigcplusplus-2-99-1-(API-3-0)
        /bin/sigc-vc140-d-3_0.dll
        /include ..
        /lib/sigc-vc140-d-3_0.dll            
```

NOTE: AngelScript must be built with native platform support (do not use AS_MAX_PORTABILITY)

See file [3rd_party/BUILDING.md](3rd_party/BUILDING.md) for futher info.

## License

ZLib, same as AngelScript. See COPYING.txt for more info.
