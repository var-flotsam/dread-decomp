// Static initialiser of a class-registration global: builds the registrar object for the class
// named "base::spatial::CAABox2D", registers its destructor with atexit and adds it to the registry.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypeDtors.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern u8 g_unk_7108b6d530[];
extern u8 g_unk_7108b6d668[];

void fn_710010c7a4(void*);
void fn_710010cb80(void* obj, CStrId* name, void* a, void* b, void (*factory)(void*), void* c);

void fn_710010d540() {
    {
        CStrId name("base::spatial::CAABox2D", true);
        fn_710010cb80(g_unk_7108b6d530, &name, nullptr, nullptr, fn_710010c7a4, nullptr);
    }
    fn_7100000250(fn_7100096604, g_unk_7108b6d530, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6d668, g_unk_7108b6d530);
}
