# CairoMM-AngelScript bindings README

Very early demo project - no bindings yet, just naked AngelScript.

Only builds as Debug using Windows 10 + MSVC 2017.

## Dependencies

Expected in this layout and versions:

```
/3rd_party
    /angelscript-2-32-0
        /include
            angelscript.h
        /lib
            angelscriptd.lib
```

NOTE: AngelScript must be built with native platform support (do not use AS_MAX_PORTABILITY)

## License

ZLib, same as AngelScript. See COPYING.txt for more info.
