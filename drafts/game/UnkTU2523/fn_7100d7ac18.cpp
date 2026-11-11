#include "base/global/CRntFile.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkEntityReset_b0220;
void fn_71002327e4(UnkEntityReset_b0220* self);
void fn_7100233a38(void* object, void* command);

// A character class (a reflected object): the values the entity takes over from it.
struct UnkCharClass_b0220 {
    u8 pad_0x0[0x13c];
    u32 field_0x13c;
    u32 field_0x140;
    u32 field_0x144;
    u32 field_0x148;
    struct __attribute__((packed)) Pair {
        u64 a;
        u32 b;
    };
    Pair field_0x14c;  // 0x14c
    u32 field_0x158;
};
static_assert(__builtin_offsetof(UnkCharClass_b0220, field_0x14c) == 0x14c, "layout");
static_assert(__builtin_offsetof(UnkCharClass_b0220, field_0x158) == 0x158, "layout");

// A component of the actor definition: entries of 0x98 bytes at +0x30 (count at +0x38).
struct UnkDefComponent_b0220 {
    void* vtable;
    u8 pad_0x8[0x28];
    u8* entries;  // 0x30
    u32 count;    // 0x38
};

struct UnkActorDef_b0220 {
    void* vtable;
    u8 pad_0x8[0x98];
    UnkDefComponent_b0220** components;  // 0xa0
    u32 component_count;                 // 0xa8
};

struct UnkEntityReset_b0220 {
    u8 pad_0x0[0x28];
    UnkReflObject* def;  // 0x28
    u8 pad_0x30[0x94 - 0x30];
    u32 field_0x94;
    u32 field_0x98;
    u32 field_0x9c;
    u8 pad_0xa0[0x308 - 0xa0];
    u64 field_0x308;
    bool flag_0x310;
    u8 pad_0x311;
    bool flag_0x312;
    u8 pad_0x313[0x37c - 0x313];
    u32 field_0x37c;
    u32 field_0x380;
    u32 field_0x384;
    UnkCharClass_b0220::Pair field_0x388;
    u32 field_0x394;
    u8 pad_0x398[8];
    u32 field_0x3a0;
    u8 pad_0x3a4[4];
    u64 field_0x3a8;
    u8 pad_0x3b0[0x3d0 - 0x3b0];
    u8 field_0x3d0;
};

template <const base::reflection::CClass* (*Getter)()>
static inline bool IsKind_b0220(UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), reinterpret_cast<const base::reflection::CType*>(Getter()));
}

void fn_7100d7ac18(UnkEntityReset_b0220* self) {
    fn_71002327e4(self);
    self->field_0x3a8 = 0xbf80000000000000;
    bool was_set = self->flag_0x310;
    self->field_0x3d0 = 0;
    self->field_0x308 = 0;
    self->flag_0x312 = false;
    if (was_set)
        self->flag_0x310 = false;
    UnkReflObject* def = self->def;
    if (def && IsKind_b0220<fn_7100254b14>(def)) {
        UnkReflObject* object = self->def;
        if (object && IsKind_b0220<fn_71011abbd4>(object)) {
            UnkCharClass_b0220* cls = reinterpret_cast<UnkCharClass_b0220*>(object);
            self->field_0x37c = cls->field_0x13c;
            self->field_0x380 = cls->field_0x140;
            self->field_0x384 = cls->field_0x148;
            u64 pair_a = cls->field_0x14c.a;
            u32 pair_b = cls->field_0x14c.b;
            self->field_0x388.a = pair_a;
            self->field_0x388.b = pair_b;
            self->field_0x394 = cls->field_0x158;
            u32 v = cls->field_0x144;
            self->field_0x94 = v;
            self->field_0x98 = v;
            self->field_0x9c = v;
        }
        UnkActorDef_b0220* actor_def = reinterpret_cast<UnkActorDef_b0220*>(def);
        UnkDefComponent_b0220** it = actor_def->components;
        UnkDefComponent_b0220** end = it + actor_def->component_count;
        for (; it != end; ++it) {
            UnkDefComponent_b0220* component = *it;
            if (!component)
                continue;
            if (!IsKind_b0220<fn_71011aca94>(reinterpret_cast<UnkReflObject*>(component)))
                continue;
            u8* entry = component->entries;
            u64 remaining = (u64)component->count * 0x98;
            while (remaining) {
                if (!entry[0x10]) {
                    u8* command = entry + 0x18;
                    fn_710007c00c(reinterpret_cast<base::global::CRntFile*>(entry + 0x50), *reinterpret_cast<u32*>(entry + 0x14), 0);
                    fn_7100233a38(self, command);
                }
                entry += 0x98;
                remaining -= 0x98;
            }
        }
    }
    self->field_0x3a0 = self->field_0x3a0 + 1;
}
