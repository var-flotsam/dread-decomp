// Unit 2616 (lua_bind): grows a keyed array of 0xd0-byte registries and their string-id keys to hold
// at least `capacity`; each registry is move-constructed (fn_71006f8ca0) and destroyed, each key moved
// and released. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRegistry_b0255;
void fn_71006f8ca0(UnkRegistry_b0255* self, UnkRegistry_b0255* other);

// The registry as this unit sees it: polymorphic, 0xd0 bytes.
struct UnkRegistryValue_lfbbbdc {
    virtual ~UnkRegistryValue_lfbbbdc();
    u8 pad_0x8[0xc8];
};
static_assert(sizeof(UnkRegistryValue_lfbbbdc) == 0xd0, "size");

struct UnkRegistryArray_lfbbbdc {
    UnkRegistryValue_lfbbbdc* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::IAllocator* allocator;
};

void fn_7100fbbbdc(UnkRegistryArray_lfbbbdc* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkRegistryValue_lfbbbdc* values = static_cast<UnkRegistryValue_lfbbbdc*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkRegistryValue_lfbbbdc), 8, self->flags));
        CStrId* keys = static_cast<CStrId*>(self->allocator->vf_0x40((u64)capacity * sizeof(CStrId), 8, self->flags));
        if (self->values) {
            UnkRegistryValue_lfbbbdc* src = self->values;
            UnkRegistryValue_lfbbbdc* dst = values;
            for (u32 i = 0; i < self->count; ++dst, ++src, ++i) {
                fn_71006f8ca0(reinterpret_cast<UnkRegistry_b0255*>(dst), reinterpret_cast<UnkRegistry_b0255*>(src));
                src->~UnkRegistryValue_lfbbbdc();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (CStrId* old = self->keys) {
            CStrId* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                CStrId::MoveConstructReloading(dst, *old);
                old->Reset();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}
