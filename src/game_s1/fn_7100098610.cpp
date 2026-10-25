// Static initialiser of an enum-type registration: builds the enum info object for
// "base::reflection::ECollectionAccessMode", registers its destructor with atexit and adds it to the registry.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern u8 g_unk_7101d5a330[];
extern u8 g_unk_7101d5a448[];

void fn_7100099468(void*);
void fn_71000982c4(void*);
void fn_7100098410(void* obj, CStrId* name, void (*fn)(void*), void* a);

void fn_7100098610() {
    {
        CStrId name("base::reflection::ECollectionAccessMode", true);
        fn_7100098410(g_unk_7101d5a330, &name, fn_71000982c4, nullptr);
    }
    fn_7100000250(fn_7100099468, g_unk_7101d5a330, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5a448, g_unk_7101d5a330);
}
