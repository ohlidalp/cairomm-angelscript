# Building dependencies for 'cairomm-angelscript'

The goal is to provide automated build; This arrangement is temporary.

Currently the project builds on Windowsn in Debug only, dependencies were built pretty much by hand. Binaries will be provided in a release tag soon.

Following are my notes on building, I wrote them on-the-go

## AngelScript

Download SDK from http://www.angelcode.com/angelscript/downloads.html and use VisualStudio project to build.

NOTE: AS_MAX_PORTABILITY is off by default which is good, we must build without it.

## Cairo

Download this dependency from https://github.com/preshing/cairo-windows

## FreeType

download FT2 release form sourceforge:	https://sourceforge.net/projects/freetype/files/freetype2/2.9.1/ft291.zip/download

this is all you need, only headers are required by CairoMM.

## SigC++

STATUS: I built it manually, got DLL+LIB, happy with it for now

NOTE: versions are copied from CMakeLists.txt (root):
```
set (SIGCXX_MAJOR_VERSION 2)
set (SIGCXX_MINOR_VERSION 99)
set (SIGCXX_MICRO_VERSION 1)

set (SIGCXX_API_VERSION 3.0)
```

1. open SLN, try building project "sigc" --> 
error MSB8036: The Windows SDK version 8.1 was not found. Install the required version of Windows SDK or change the SDK version in the project property pages or by right-clicking the solution and selecting "Retarget solution".

2. retargeted OK

3. try building "sigc" ---> fatal error C1083: Cannot open include file: 'msvc_recommended_pragmas.h'

4. open "sigc-build-defines.props", locate <ForcedIncludeFiles>msvc_recommended_pragmas.h;%(ForcedIncludeFiles)</ForcedIncludeFiles> and delete the "msvc_recommended_pragmas.h". That's a Glib (GNOME) header: https://github.com/GNOME/glib/blob/master/msvc_recommended_pragmas.h. Save and re-open solution.

5. try building "sigc"  ---> fatal error C1083: Cannot open include file: 'sigc++config.h': No such file or directory

6. I grab the config.h from Cmake build I did earlier - I could as well create it manually from config.h.in
I put the file to MSVC_Net2017 and try again.

7. try building "sigc"  --> 1>RC : fatal error RC1110: could not open sigc.rc

8. I create it manually from sigc.rc.in and try again

9. Looks good enough. MSVC build log follows
	NOTE: PlatformToolset=Visual Studio 2017 (v141)

	1>------ Build started: Project: sigc, Configuration: Debug Win32 ------
	1>C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\VC\VCTargets\Microsoft.CppBuild.targets(1205,5): warning MSB8012: TargetPath(C:\Users\po185021\source\libsigcplusplus\MSVC_Net2017\Debug\Win32\bin\sigc.dll) does not match the Linker's OutputFile property value (C:\Users\po185021\source\libsigcplusplus\MSVC_Net2017\Debug\Win32\bin\sigc-vc140-d-3_0.dll). This may cause your project to build incorrectly. To correct this, please make sure that $(OutDir), $(TargetName) and $(TargetExt) property values match the value specified in %(Link.OutputFile).
	1>C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\VC\VCTargets\Microsoft.CppBuild.targets(1207,5): warning MSB8012: TargetName(sigc) does not match the Linker's OutputFile property value (sigc-vc140-d-3_0). This may cause your project to build incorrectly. To correct this, please make sure that $(OutDir), $(TargetName) and $(TargetExt) property values match the value specified in %(Link.OutputFile).
	1>connection.obj : warning LNK4075: ignoring '/EDITANDCONTINUE' due to '/SAFESEH' specification
	1>   Creating library C:\Users\po185021\source\libsigcplusplus\MSVC_Net2017\Debug\Win32\bin\sigc-vc140-d-3_0.lib and object C:\Users\po185021\source\libsigcplusplus\MSVC_Net2017\Debug\Win32\bin\sigc-vc140-d-3_0.exp
	1>libsigc++3.vcxproj -> C:\Users\po185021\source\libsigcplusplus\MSVC_Net2017\Debug\Win32\bin\sigc.dll
	1>Done building project "libsigc++3.vcxproj".
	========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========

10. try build "test_bind"
	1>c:\users\po185021\source\libsigcplusplus\sigc++\tuple-utils\tuple_transform_each.h(27): error C2429: language feature 'nested-namespace-definition' requires compiler flag '/std:c++17'
	1>c:\users\po185021\source\libsigcplusplus\sigc++\tuple-utils\tuple_transform_each.h(38): error C4984: 'if constexpr' is a C++17 language extension
	
11. added /std:c++17 to "test_bind" commandline -> it builds OK.

12. Run "sigc-install" --> works, throws directory vc14 to path {\..\..\vs14\Win32 …}, nevermind, I'll move it manually

## CairoMM 

Platform: Windows 10 x64
IDE: Visual Studio Community 2017, version 15.7.2

STATUS: got DLL+LIB from manual build, happy with it. NOTE: headers are a mess, I don't have "cairommconfig" but looking in "cairommconfig.in" it's dummy anyway, headers are mixed with sources in "cairom-1.5x/cairomm", "cairomm.h" expects other headrs to be in subdir "include/cairomm/*", must compose manually.

1. download cairomm source release from https://www.cairographics.org/cairomm/,  - source releases
		cairomm-1.15.5.tar.gz	2017-08-25 10:11 	1.3M
2. go to directory MSVC_Net2017

3. open "cairomm-build-defines.props", locate <ForcedIncludeFiles>***</ForcedIncludeFiles> and remove "msvc_recommended_pragmas.h" - that header is not available in this package or any needed deps. More info: https://github.com/GNOME/glib/blob/master/msvc_recommended_pragmas.h

4. Open solution file "cairomm.sln". It's targeted on Windows SDK 8.1. Right-click on the solution in "solution explorer" panel, find "Regarget solution" and choose your version.

5. OK, I need LibSigC++ - https://github.com/libsigcplusplus/libsigcplusplus

6. built libsigc manually

7. in "*paths.props", set  <GlibEtcInstallRoot>c:\Users\PO185021\source\cairomm\sigc-vc14-win32-debug</GlibEtcInstallRoot>

8. "cairomm-build-defines.props" 
	AdditionalIncludeDirectories> <--- add c:\Users\PO185021\source\cairomm\cairo-windows-1.15.10\include\
	<AdditionalLibraryDirectories> <--- add {c:\Users\PO185021\source\cairomm\cairo-windows-1.15.10\lib\}
	
9 BUILDING
	1>------ Build started: Project: gendef, Configuration: Debug Win32 ------
	1>gendef.obj : warning LNK4075: ignoring '/EDITANDCONTINUE' due to '/SAFESEH' specification
	1>gendef.vcxproj -> C:\Users\po185021\source\cairomm\cairomm-1.15.5\MSVC_Net2017\Debug\Win32\bin\gendef.exe
	1>Done building project "gendef.vcxproj".
	========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========

10 Try building "cairomm" --> Eh, I need FreeType2
	1>------ Build started: Project: cairomm, Configuration: Debug Win32 ------
	1>xlib_surface.cc
	1>c:\users\po185021\source\cairomm\cairo-windows-1.15.10\include\cairo-ft.h(46): fatal error C1083: Cannot open include file: 'ft2build.h': No such file or directory
	..SNIP..
	1>Done building project "cairomm.vcxproj" -- FAILED.
	========== Build: 0 succeeded, 1 failed, 1 up-to-date, 0 skipped ==========
	
11. obtain FT2   - see above

12.  
	Update "cairomm-build-defines.props" 
		AdditionalIncludeDirectories> <--- c:\Users\PO185021\source\cairomm\freetype-2.9.1\include
		
13 BUILD!! --> looks good!
	NOTE: PlatformToolset=Visual Studio 2017 (v141)
	1>------ Build started: Project: gendef, Configuration: Debug Win32 ------
	..SNIP..
	1>gendef.vcxproj -> C:\Users\po185021\source\cairomm\cairomm-1.15.5\MSVC_Net2017\Debug\Win32\bin\gendef.exe
	1>Done building project "gendef.vcxproj".
	
	2>------ Build started: Project: cairomm, Configuration: Debug Win32 ------
	..SNIP..
	2>   Creating library C:\Users\po185021\source\cairomm\cairomm-1.15.5\MSVC_Net2017\Debug\Win32\bin\cairomm-vc150-d-1_16.lib and object C:\Users\po185021\source\cairomm\cairomm-1.15.5\MSVC_Net2017\Debug\Win32\bin\cairomm-vc150-d-1_16.exp
	2>cairomm.vcxproj -> C:\Users\po185021\source\cairomm\cairomm-1.15.5\MSVC_Net2017\Debug\Win32\bin\cairomm-vc150-d-1_16.dll
	2>Done building project "cairomm.vcxproj".
	========== Build: 2 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
	
14 build all others (optional, EXE utilities) -> works OK!!

		

	

	







