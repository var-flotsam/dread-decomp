// base::animation::CBlendSpace1D's unit (unit 13): reads the blend space's position from the
// stream's value named like the blend space (a float or an int), marks the space changed when
// it differs, then does the same for every source that is itself a blend space.
// Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

u64 fn_710007c70c(base::global::CRntFile* file, void* dest, u64 size);

__attribute__((visibility("hidden"))) extern base::reflection::CClass g_unk_7101d4bc00;

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkBlendStaticZero13 {
    u32 value;
    UnkBlendStaticZero13() : value(0) {}
};

inline const UnkBlendStaticZero13& UnkBlendStaticZero() {
    static UnkBlendStaticZero13 s_value asm("g_unk_7101d4bbd8");
    return s_value;
}

// The complement of that zero's low three bits.
inline u32 UnkBlendDefaultMask() {
    static u32 s_mask asm("g_unk_7101d4bbc8") = ~UnkBlendStaticZero().value & 7;
    return s_mask;
}

// The pooled string as the lookup sees it: its hash is the key of a stream value.
struct UnkBlendStringHash13 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkBlendStreamValue13 {
    u64 offset;
    u64 key;   // 0x8
    u32 type;  // 0x10: 'f' or 'i'
};
static_assert(sizeof(UnkBlendStreamValue13) == 0x18, "layout");

struct UnkBlendStream13 {
    UnkBlendStreamValue13* values;
    u32 count;
    u8 pad_0xc[0x1c];
    base::global::CRntFile file;  // 0x28

    UnkBlendStreamValue13* begin() { return values; }
    UnkBlendStreamValue13* end() { return values + count; }
};
static_assert(__builtin_offsetof(UnkBlendStream13, file) == 0x28, "layout");

struct UnkBlendSource13 {
    virtual const base::reflection::CType* vf_0x0();
};

struct UnkBlendReadSlot13 {
    f32 position;
    UnkBlendSource13* source;  // 0x8
    void* source_ctrl;
};

struct UnkBlendSpaceRead13 {
    void** vtable;
    u8 pad_0x8[0x20];
    base::global::CStrId name;  // 0x28
    f32 position;               // 0x30
    u8 pad_0x34[0xc];
    u32 mask;                   // 0x40
    UnkBlendReadSlot13* slots;  // 0x48
    u32 slot_count;             // 0x50
};
static_assert(__builtin_offsetof(UnkBlendSpaceRead13, slot_count) == 0x50, "layout");

void fn_7100007320(UnkBlendSpaceRead13* self, UnkBlendStream13* stream) {
    base::global::CStrId name(self->name);
    if (name.data != g_unk_7101d470e0.data) {
        f32 value = 0.0f;
        const UnkBlendStringHash13* key = reinterpret_cast<const UnkBlendStringHash13*>(name.data);
        UnkBlendStreamValue13* it =
            std::find_if(stream->begin(), stream->end(), [key](const UnkBlendStreamValue13& v) {
                return v.key == key->hash;
            });
        if (it != stream->end()) {
            switch (it->type) {
            case 'f':
                fn_710007c00c(&stream->file, it->offset, 0);
                fn_710007c70c(&stream->file, &value, 4);
                break;
            case 'i': {
                s32 integer;
                fn_710007c00c(&stream->file, it->offset, 0);
                fn_710007c70c(&stream->file, &integer, 4);
                value = static_cast<f32>(integer);
                break;
            }
            }
        }
        if (self->position != value) {
            self->position = value;
            self->mask = UnkBlendDefaultMask();
        }
    }
    for (UnkBlendReadSlot13 *slot = self->slots, *end = slot + self->slot_count; slot != end; ++slot) {
        UnkBlendSource13* source = slot->source;
        if (source) {
            const base::reflection::CType* type = source->vf_0x0();
            fn_71000062ac();
            if (fn_710009d50c(type, &g_unk_7101d4bc00))
                fn_7100007320(reinterpret_cast<UnkBlendSpaceRead13*>(source), stream);
        }
    }
}
