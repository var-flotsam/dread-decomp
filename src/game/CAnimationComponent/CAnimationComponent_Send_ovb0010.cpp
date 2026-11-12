// CAnimationComponent's unit (tools/tu_map.py), two functions.
// fn_7101177898 looks the pair of ids at +0x30 of two named objects up
// in the component's action sets (+0x48, count at +0x50), then sends the result with mode 2 and the
// track entry of hash 0xf4061ff25a2163d6 (fn_7101177044), first descriptor g_unk_710164b968 and the
// all-bits mask. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CActionSet.h"
#include "types.h"

extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT

struct UnkActionSetMap;

struct UnkNamed_ovb0010 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkEntry_ovb0010 {
    u8 pad_0x0[0x70];
    void* field_0x70;
};

struct UnkMap_ovb0010 {
    u8 pad_0x0[0xc8];
    UnkEntry_ovb0010** values;
    u64* keys;
    u32 count;

    UnkEntry_ovb0010** Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

struct UnkAnim_ovb0010 {
    u8 pad_0x0[0x40];
    UnkMap_ovb0010* map;
    UnkActionSetMap** sets;
    u32 count;

    __attribute__((always_inline)) void* Lookup(const u64& key1, const u64& key2) {
        void* r = nullptr;
        UnkActionSetMap** it = sets;
        UnkActionSetMap** end = it + count;
        if (it != end) {
            do {
                r = fn_7100b8c61c(*it, &key1, &key2);
                ++it;
            } while (it != end && !r);
        }
        return r;
    }
};

// A set of flags with a copy constructor: passed by value, it travels as the address of the caller's copy.
struct alignas(8) UnkFlags_7101176690 {
    u32 value;
    UnkFlags_7101176690(u32 v) : value(v) {}
    UnkFlags_7101176690(const UnkFlags_7101176690& o) : value(o.value) {}
};
struct UnkStruct_7101176690;
struct UnkS16;
struct UnkS12;

bool fn_7101177044(UnkStruct_7101176690* self, void* r, s32 idx, void* data, UnkFlags_7101176690 flags,
                   const UnkS16& a, const UnkS12& b, UnkFlags_7101176690 c);

// Sends the action the pair of keys names with mode 2 and the all-bits mask.
static inline bool Send(UnkAnim_ovb0010* self, u64 key1, u64 key2) {
    void* r = self->Lookup(key1, key2);
    UnkMap_ovb0010* m = self->map;
    UnkEntry_ovb0010** it = m->Find(0xf4061ff25a2163d6);
    UnkEntry_ovb0010** end = m->values + m->count;
    void* data = (it != end && *it) ? (*it)->field_0x70 : nullptr;
    return fn_7101177044(reinterpret_cast<UnkStruct_7101176690*>(self), r, -1, data, UnkFlags_7101176690{2},
                         reinterpret_cast<const UnkS16&>(g_unk_710164b968),
                         reinterpret_cast<const UnkS12&>(g_unk_7172769740),
                         UnkFlags_7101176690{UnkAll_SpitSend().value});
}

bool fn_7101177898(UnkAnim_ovb0010* self, UnkNamed_ovb0010** first, UnkNamed_ovb0010** second) {
    return Send(self, (*first)->id, (*second)->id);
}

// fn_71011785ec clears every tag bit of a guarded 15-bit all-tags mask
// (the complement of the enum zero CCutSceneDef's tag fields default to) from the word at +0x74 and
// sets them in the word at +0x78; when asked, folds +0x74 into the word at +0x70 minus the bits of
// +0x78 and, if that changed it, lets fn_71011787dc refresh.
struct UnkAnimTags_ovb0010 {
    u8 pad_0x0[0x70];
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
};

void fn_71011787dc(UnkAnimTags_ovb0010* self, bool flag);

inline const u32& AllTags_ovb0010() {
    static u32 s_value = ~base::reflection::refl_GetEnumZero<0x7172794a88>() & 0x7fff;
    return s_value;
}

void fn_71011785ec(UnkAnimTags_ovb0010* self, bool apply, bool flag) {
    self->field_0x74 &= ~AllTags_ovb0010();
    self->field_0x78 |= AllTags_ovb0010();
    if (!apply)
        return;
    u32 old = self->field_0x70;
    u32 value = (self->field_0x74 | old) & ~self->field_0x78;
    self->field_0x70 = value;
    if (value != old)
        fn_71011787dc(self, flag);
}
