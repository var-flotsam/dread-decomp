// Static initialiser of a class-registration global: builds the registrar object for the class
// named "base::file::CPack", registers its destructor with atexit and adds it to the registry.
#include "base/global/CStrId.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern u8 g_unk_7101d59410[];
extern u8 g_unk_7101d59548[];
extern u8 g_unk_7101a19000 HIDDEN;  // __dso_handle

void fn_7100096604(void*);
void fn_7100088c50(void*);
void fn_7100088ca8(void* obj, CStrId* name, void* a, void* b, void (*factory)(void*), void* c);
void fn_710009ec00(void* registry, void* obj);
int fn_7100000250(void (*)(void*), void*, void*);

void fn_71000891c0() {
    {
        CStrId name("base::file::CPack", true);
        fn_7100088ca8(g_unk_7101d59410, &name, nullptr, nullptr, fn_7100088c50, nullptr);
    }
    fn_7100000250(fn_7100096604, g_unk_7101d59410, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d59548, g_unk_7101d59410);
}
