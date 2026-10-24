// Static initialiser of a class-registration global: builds the registrar object for the class
// named "base::file::CPackSetManager", registers its destructor with atexit and adds it to the registry.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern u8 g_unk_7101d596a8[];
extern u8 g_unk_7101d597e0[];

void fn_7100096604(void*);
void fn_710008b6d4(void*);
void fn_710008b72c(void* obj, CStrId* name, void* a, void* b, void (*factory)(void*), void* c);

void fn_710008c710() {
    {
        CStrId name("base::file::CPackSetManager", true);
        fn_710008b72c(g_unk_7101d596a8, &name, nullptr, nullptr, fn_710008b6d4, nullptr);
    }
    fn_7100000250(fn_7100096604, g_unk_7101d596a8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d597e0, g_unk_7101d596a8);
}
