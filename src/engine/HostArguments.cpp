// The host argument walk nnMain starts with (src/engine/nnMain.cpp): reads every argument after
// the program's name and keeps none (the release build's option parsing is compiled out). Its
// parameters are unused; nnMain passes 0 and null. In the first translation unit of main
// (tools/tu_map.py: unit 0), apart from nnMain's.

#include <nn/os.h>

#include "types.h"

void fn_71000005d0(s32, char**) {
    const s32 argc = nn::os::GetHostArgc();
    for (s32 i = 1; i < argc; ++i)
        nn::os::GetHostArgv();
}
