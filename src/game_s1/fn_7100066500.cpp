// Static initialiser: constructs two string-id globals in place (registering their destructors
// with atexit), then registers 1 class: "base::global::CLut".
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern CStrId g_unk_7101d57990[2] HIDDEN;
void fn_710000051c(void*);
extern u8 g_unk_7101d57850[];
extern u8 g_unk_7101d57988[];
void fn_7100065d24(void*);
void fn_7100065d78(void* obj, CStrId* name, void* a, void* b, void (*factory)(void*), void* c);
void fn_7100096604(void*);
void fn_7100066500() {
    new (&g_unk_7101d57990[0]) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d57990[0], &g_unk_7101a19000);
    new (&g_unk_7101d57990[1]) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d57990[1], &g_unk_7101a19000);
    {
        CStrId name("base::global::CLut", true);
        fn_7100065d78(g_unk_7101d57850, &name, nullptr, nullptr, fn_7100065d24, nullptr);
    }
    fn_7100000250(fn_7100096604, g_unk_7101d57850, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d57988, g_unk_7101d57850);
}
