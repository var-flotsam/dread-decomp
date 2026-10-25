// Static initialiser of a class-registration global: builds the registrar object for the class
// named "base::snd::CSoundGroupManager", registers its destructor with atexit and adds it to the registry.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypeDtors.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern u8 g_unk_7108b6c320[];
extern u8 g_unk_7108b6c458[];

void fn_71001043e4(void*);
void fn_710010443c(void* obj, CStrId* name, void* a, void* b, void (*factory)(void*), void* c);

void fn_7100104e30() {
    {
        CStrId name("base::snd::CSoundGroupManager", true);
        fn_710010443c(g_unk_7108b6c320, &name, nullptr, nullptr, fn_71001043e4, nullptr);
    }
    fn_7100000250(fn_7100096604, g_unk_7108b6c320, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7108b6c458, g_unk_7108b6c320);
}
