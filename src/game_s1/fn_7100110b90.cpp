// Static initialiser of a class-registration global: builds the registrar object for the class
// named "base::spatial::COBox2D", registers its destructor with atexit and adds it to the registry.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern u8 g_unk_7108b6d670[];
extern u8 g_unk_7108b6d7a8[];

void fn_7100096604(void*);
void fn_710010f870(void*);
void fn_710010fe28(void* obj, CStrId* name, void* a, void* b, void (*factory)(void*), void* c);

void fn_7100110b90() {
    {
        CStrId name("base::spatial::COBox2D", true);
        fn_710010fe28(g_unk_7108b6d670, &name, nullptr, nullptr, fn_710010f870, nullptr);
    }
    fn_7100000250(fn_7100096604, g_unk_7108b6d670, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6d7a8, g_unk_7108b6d670);
}
