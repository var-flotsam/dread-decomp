// Unit 1115 (tools/tu_map.py, no class name): a registry object forgets a name it answers to and a
// graph id it knows. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

// The object of the registry, as far as this function reads it.
struct UnkRegObj_l6f8af4 {
    u8 pad_0x0[0x18];
    CRntVector<u32> ids;      // 0x18
    CRntVector<CStrId> names;  // 0x40
};
static_assert(__builtin_offsetof(UnkRegObj_l6f8af4, names) == 0x40, "layout");

// Releases the name at `index` and moves the later ones down.
static inline void EraseName(CRntVector<CStrId>& names, u32 index) {
    if (index < names.count) {
        names.data[index].~CStrId();
        for (u32 i = index; i < names.count - 1; ++i) {
            CStrId::MoveConstructReloading(names.data + i, *(names.data + i + 1));
            (names.data + i + 1)->~CStrId();
        }
        names.count = names.count - 1;
    }
}

// The entry and its name as the callers pass them (UnkUnit1115_b50.cpp).
struct UnkQueryEntry_b50;

void fn_71006f8af4(UnkQueryEntry_b50* entry, void* name_id, u32 id) {
    auto* self = reinterpret_cast<UnkRegObj_l6f8af4*>(entry);
    const CStrId& name = *static_cast<const CStrId*>(name_id);
    u32 n = self->names.count;
    for (u64 i = 0; i < n; ++i) {
        if (self->names.data[i] == name) {
            EraseName(self->names, i);
            break;
        }
    }
    self->ids.Remove(id);
}
