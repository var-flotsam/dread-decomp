// Static initialiser of an enum-type registration: builds the enum info object for
// "base::reflection::EAttributeTarget", registers its destructor with atexit and adds it to the registry.
#include "base/global/CStrId.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern u8 g_unk_7101d5a038[];
extern u8 g_unk_7101d5a150[];
extern u8 g_unk_7101a19000 HIDDEN;  // __dso_handle

void fn_7100099468(void*);
void fn_7100093f60(void*);
void fn_710009410c(void* obj, CStrId* name, void (*fn)(void*), void* a);
void fn_710009ec00(void* registry, void* obj);
int fn_7100000250(void (*)(void*), void*, void*);

void fn_7100094300() {
    {
        CStrId name("base::reflection::EAttributeTarget", true);
        fn_710009410c(g_unk_7101d5a038, &name, fn_7100093f60, nullptr);
    }
    fn_7100000250(fn_7100099468, g_unk_7101d5a038, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5a150, g_unk_7101d5a038);
}
