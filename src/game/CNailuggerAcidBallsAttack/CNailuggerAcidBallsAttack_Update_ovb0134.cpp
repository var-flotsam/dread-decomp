// CNailuggerAcidBallsAttack's unit (tools/tu_map.py unit 941). fn_71006259ac is the copy assignment of a
// record: its plain fields are copied from `other`, and (unless it is assigned to itself) the weak
// reference at +0x48 is made to refer to what the other one refers to.
// fn_7100624e44 is the attack's update. After the base attack's update (fn_71003609a8) it takes the owner's AI component as a
// CNailongAIComponent. State 1 counts its timer (+0x68) up to the data's delay (+0xf8) and then
// switches to state 2; state 2 counts its timer (+0x7c) up to the duration of the current entry
// (+0x74) of the AI's pattern (fn_7100621aa8, by +0x70), fires that entry (fn_71006250c0, with the
// pattern's name) and moves to the next one, switching to state 3 after the last (+0x78). State
// changes go through fn_7100624fe0. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game_s2/UnkSpitAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkNailPatternEntry_ovb0134 {
    u8 pad_0x0[0x1c];
    f32 duration;  // 0x1c
};

struct UnkNailPattern_ovb0134 {
    u8 pad_0x0[0x8];
    CStrId name;                             // 0x8
    UnkNailPatternEntry_ovb0134* entries;  // 0x10
};

struct UnkNailPatternRef_ovb0134 {
    UnkNailPattern_ovb0134* pattern;
};

struct UnkNailData_ovb0134 {
    u8 pad_0x0[0xf8];
    f32 delay;  // 0xf8
};

struct UnkNailAttack_ovb0134 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkNailData_ovb0134* data;  // 0x28
    u8 pad_0x30[0x20];
    s32 state;  // 0x50
    u8 pad_0x54[0x14];
    f32 delay_timer;  // 0x68
    u8 pad_0x6c[0x4];
    s32 pattern_id;   // 0x70
    u32 index;        // 0x74
    u32 count;        // 0x78
    f32 timer;        // 0x7c
};

UnkNailPatternRef_ovb0134* fn_7100621aa8(UnkReflObject* ai, s32 id);
// The pattern's name, handed over as an id the caller releases (by the value it handed over).
struct UnkNameArg_ovb0134 {
    base::global::UnkStringData* data;
};
void fn_71006250c0(UnkNailAttack_ovb0134* self, UnkNailPatternEntry_ovb0134* entry, u32 index,
                   __attribute__((noescape)) UnkNameArg_ovb0134* name);
void fn_7100624fe0(UnkNailAttack_ovb0134* self, s32 state);

void fn_7100624e44(UnkNailAttack_ovb0134* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    UnkReflObject* component = static_cast<UnkReflObject*>(self->owner->field_0x240);
    UnkReflObject* ai = nullptr;
    if (component)
        ai = fn_710009d50c(component->vf_0x0(), fn_710061f464()) ? component : nullptr;
    switch (self->state) {
    case 1:
        self->delay_timer += dt;
        if (self->delay_timer >= self->data->delay)
            return fn_7100624fe0(self, 2);
        break;
    case 2: {
        UnkNailPatternRef_ovb0134* ref = fn_7100621aa8(ai, self->pattern_id);
        self->timer += dt;
        UnkNailPattern_ovb0134* pattern = ref->pattern;
        u32 index = self->index;
        UnkNailPatternEntry_ovb0134* entry = &pattern->entries[index];
        if (self->timer >= entry->duration) {
            {
                base::global::UnkStringData* data = pattern->name.data;
                UnkNameArg_ovb0134 name = {data};
                if (&g_unk_7101d47108->empty != data)
                    ++data->ref_count;
                fn_71006250c0(self, entry, index, &name);
                if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
                    if (data && &pool->empty != data) {
                        if (--data->ref_count == 0) {
                            fn_71000803a4(pool, data);
                            pool = g_unk_7101d47108;
                        }
                    }
                    name.data = &pool->empty;
                }
            }
            self->timer = 0.0f;
            if (++self->index == self->count)
                return fn_7100624fe0(self, 3);
        }
        break;
    }
    }
}

struct UnkNailuggerRecord_ovb0134 {
    u8 pad_0x0[0x8];
    u8 field_0x8[0x20];
    u64 field_0x28;
    u8 pad_0x30[0x8];
    u8 field_0x38[0x10];
    UnkWeakHandle* field_0x48;
    u8 field_0x50[0x30];
};

UnkNailuggerRecord_ovb0134* fn_71006259ac(UnkNailuggerRecord_ovb0134* self,
                                          const UnkNailuggerRecord_ovb0134* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 0x20);
    self->field_0x28 = other->field_0x28;
    __builtin_memcpy(self->field_0x38, other->field_0x38, 0x10);
    if (self != other)
        CopyWeakRef(&self->field_0x48, other->field_0x48);
    __builtin_memcpy(self->field_0x50, other->field_0x50, 0x30);
    return self;
}
