// nnMain, the application's entry: walks the host arguments (fn_71000005d0), runs the system
// start-up and the main loop (fn_7100132d30, which calls fn_71001325d0 first) with the game's
// main function fn_7100000980, then fn_7100000610, registers fn_7100000d40 as the per-frame system
// callback (fn_7100132f40 stores it for the notification loop fn_71001327c0) and shuts down with
// fn_7100000cf0 (fn_7100132e90 calls it, then the shutdown fn_7100132730). The three callbacks
// are loaded through the GOT, as any function whose address the code takes.

#include "types.h"

void fn_71000005d0(s32 argc, char** argv);  // src/engine/HostArguments.cpp
void fn_7100132d30(void (*main)());
void fn_7100000610();
void fn_7100000980();
void fn_7100000d40();
void fn_7100000cf0();
// Defined by src/lifted/ with these placeholder signatures; called here with the callback each one
// takes.
char* fn_7100132f40();
char* fn_7100132e90(char* callback);

extern "C" void nnMain() {
    fn_71000005d0(0, nullptr);
    fn_7100132d30(fn_7100000980);
    fn_7100000610();
    reinterpret_cast<void (*)(void (*)())>(fn_7100132f40)(fn_7100000d40);
    fn_7100132e90(reinterpret_cast<char*>(fn_7100000cf0));
}
