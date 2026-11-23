// Unit 2566 (minimap): creates an object of the minimap or map-definition class named by a type id: the class must be a
// kind of base::core::CAsset and have a constructor; a block of its size is allocated and constructed.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;

struct UnkTypeIdHolder {
    u64 id;
};

extern UnkTypeIdHolder g_unk_71727c4670;  // "minimap"
extern UnkTypeIdHolder g_unk_71727c83d8;  // "mmapdef"

struct UnkClassView_le8f600 {
    u8 unk_0x0[0x18];
    u64 size;                      // 0x18
    u8 unk_0x20[0x8];
    void (*construct)(void* obj);  // 0x28
};

void* fn_7100e8f600(void* self, const u64* typeId, u32 flags) {
    const CClass* found;
    if (*typeId == g_unk_71727c4670.id) {
        found = fn_7100e31c70();
    } else if (*typeId == g_unk_71727c83d8.id) {
        found = fn_7100e66250();
    } else {
        return nullptr;
    }
    const UnkClassView_le8f600* cls = reinterpret_cast<const UnkClassView_le8f600*>(found);
    if (!fn_710009d50c(reinterpret_cast<const CType*>(cls), reinterpret_cast<const CType*>(fn_7100016894())))
        return nullptr;
    if (!cls->construct)
        return nullptr;
    void* obj = GetDefaultAllocator()->vf_0x40(cls->size, 8, flags);
    if (obj)
        cls->construct(obj);
    return obj;
}
