#pragma once

// The C++ runtime hooks the engine's static initialisers use: __cxa_atexit (fn_7100000250 in
// main, a library function), which registers the destructor of a reflected type or string id,
// and the module's DSO handle (__dso_handle; gen_lift lifts it without a name) that every
// registration passes. The handle is hidden, so it is reached directly, not through the GOT.

extern "C" int __cxa_atexit(void (*dtor)(void*), void* obj, void* dso);
int fn_7100000250(void (*dtor)(void*), void* obj, void* dso);  // __cxa_atexit, as main names it
extern char g_unk_7101a19000[] __attribute__((visibility("hidden")));  // the module's DSO handle
