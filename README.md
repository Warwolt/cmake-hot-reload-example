# Hot Reloading in C++

This is a small example repository of doing hot reloading in C++ on Windows by
employing the DLL based technique initially presented by Casey Muratori in his
Handmade Hero series.

The basic premise is that a DLL will contain code that is dynamically linked
into an executable at runtime, which permits that DLL to be updated and reloaded
without having to terminate the program.

In this repository, hot reloading is only used in Debug builds, and in Release
builds all code is linked statically. To support this, the following structures are employed:

```
// Debug
+------+    +-----------+    +-----------+
| Main | -> | MyApp.lib | -> | MyApp.dll |
+------+    +-----------+    +-----------+
```

```
// Release
+------+    +-----------+
| Main | -> | MyApp.lib |
+------+    +-----------+
```

In the Debug configuration, the MyApp.lib static library acts as a "loader"
library, that loads the DLL containing the application code, and gives a static
interface for calling the methods in the DLL. This is basically a hand-written
"import library". This static library also implements a "load_library" function
to support the hot reloading, by unloading a previous DLL instance before
loading.

The Release configuration is simpler, where all of the application code is
inside MyLib.lib, and is just directly statically linked into MyApp.exe
