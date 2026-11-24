// Static initialiser of an enum-type registration: builds the enum info object for
// "base::reflection::EAttributeTarget", registers its destructor with atexit and adds it to the registry.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkTUInit.h"
#include "base/reflection/TypeObject.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

namespace base::reflection { enum class EAttributeTarget : u32; }
template <> u8 base::reflection::TypeObject<base::reflection::EAttributeTarget, u8[]>::object[] asm("g_unk_7101d5a038");
extern u8 g_unk_7101d5a150[];

void fn_7100093f60(void*);
void fn_710009410c(void* obj, CStrId* name, void (*fn)(void*), void* a);

void RegisterEAttributeTargetEnumType() {
    {
        CStrId name("base::reflection::EAttributeTarget", true);
        fn_710009410c(base::reflection::TypeObject<base::reflection::EAttributeTarget, u8[]>::object, &name, fn_7100093f60, nullptr);
    }
    fn_7100000250(fn_7100099468, base::reflection::TypeObject<base::reflection::EAttributeTarget, u8[]>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5a150, base::reflection::TypeObject<base::reflection::EAttributeTarget, u8[]>::object);
}
