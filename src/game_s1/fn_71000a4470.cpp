// Static initialiser: constructs two string-id globals in place (registering their destructors
// with atexit), then registers 1 class: "EExecutionReturnValue".
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTUInit.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern CStrId g_unk_7101d5bdb0[2] HIDDEN;
void fn_710000051c(void*);
extern u8 g_unk_7101d5b720[];
extern u8 g_unk_7101d5b838[];
void fn_71000a2184(void*);
void fn_71000a2270(void* obj, CStrId* name, void (*fn)(void*), void* a);
void fn_71000a4470() {
    new (&g_unk_7101d5bdb0[0]) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d5bdb0[0], &g_unk_7101a19000);
    new (&g_unk_7101d5bdb0[1]) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d5bdb0[1], &g_unk_7101a19000);
    {
        CStrId name("EExecutionReturnValue", true);
        fn_71000a2270(g_unk_7101d5b720, &name, fn_71000a2184, nullptr);
    }
    fn_7100000250(fn_7100099468, g_unk_7101d5b720, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5b838, g_unk_7101d5b720);
}
