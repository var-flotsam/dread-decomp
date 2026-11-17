// Units 255 and 256 of tools/tu_map.py (no reflected class): two setters that replace an owned object
// entered in the registry g_unk_716e34c2e0 (as src/base/spatial/UnkDecalProjector.cpp's decal): the
// old one is removed from the registry (fn_710019e100); with an owner, a new one (0x140 bytes, built
// by fn_7100195680; 0x138 bytes, built by fn_7100196480) is allocated with flags 0x3b, given its
// owner (fn_710019e95c) and entered in the registry. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "game/UnkTU204.h"
#include "game/UnkTU225.h"
#include "game/UnkTU226.h"
#include "types.h"

struct UnkKeyOwner;
void fn_7100195680(void* self);
void fn_7100196480(void* self);

namespace {

// As UnkProjectorRegistry (src/base/spatial/UnkDecalProjector.cpp).
struct UnkRegistry_b58 {
    u8 pad_0x0[0x8];
    base::global::CRntDictionary<u64, u64> objects;  // +0x8: each object its own value
};

template <u64 Size, void (*Ctor)(void*)>
__attribute__((always_inline)) inline void SetChild(void** slot, void* owner) {
    if (*slot) {
        fn_710019e100(static_cast<UnkKeyOwner*>(g_unk_716e34c2e0), reinterpret_cast<u64>(*slot));
        *slot = nullptr;
    }
    if (!owner)
        return;
    auto* registry = static_cast<UnkRegistry_b58*>(g_unk_716e34c2e0);
    void* obj = fn_71000839a0(Size, 0x3b);
    Ctor(obj);
    if (obj) {
        fn_710019e95c(obj, owner);
        registry->objects.InsertChecked(reinterpret_cast<u64>(obj), reinterpret_cast<u64>(obj));
    }
    *slot = obj;
}

}  // namespace

void fn_71001c3894(char* self, void* owner) {
    SetChild<0x140, fn_7100195680>(reinterpret_cast<void**>(self + 0x78), owner);
}

void fn_71001c3b14(char* self, void* owner) {
    SetChild<0x138, fn_7100196480>(reinterpret_cast<void**>(self + 0x80), owner);
}
