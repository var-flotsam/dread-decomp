// Static initialiser of a class-registration global: builds the registrar object for the class
// named "base::global::CRntFile", registers its destructor with atexit and adds it to the registry.
#include "base/global/CStrId.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern u8 g_unk_7101d58bd0[];
extern u8 g_unk_7101d58d08[];
extern u8 g_unk_7101a19000 HIDDEN;  // __dso_handle

void fn_7100096604(void*);
void fn_710007bb30(void*);
void fn_710007bc54(void* obj, CStrId* name, void* a, void* b, void (*factory)(void*), void* c);
void fn_710009ec00(void* registry, void* obj);
int fn_7100000250(void (*)(void*), void*, void*);

void fn_710007e600() {
    {
        CStrId name("base::global::CRntFile", true);
        fn_710007bc54(g_unk_7101d58bd0, &name, nullptr, nullptr, fn_710007bb30, nullptr);
    }
    fn_7100000250(fn_7100096604, g_unk_7101d58bd0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d58d08, g_unk_7101d58bd0);
}
