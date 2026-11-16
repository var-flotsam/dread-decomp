// CWeightActivablePropComponent's unit: the destructor. Stores the vtable, resets the string-id table at
// +0x210 and the string id at +0x208, detaches the listener at +0x1e0, clears the state members and two
// reflected values, then runs the component destructor. Names and layouts are placeholders.
#include "base/UnkStrIdTable.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a971b8[];
extern void* g_unk_7101a97420[];
extern void* g_unk_7101a97478[];

struct UnkWeightLink_lac90a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    UnkWeakTarget* owner;    // 0x10
};

struct UnkWeightListener_lac90a8 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeightLink_lac90a8* link;  // 0x18
};

// Two words cleared together.
struct UnkWeightPair_lac90a8 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkWeightDtor_lac90a8 : UnkBaseComponent {
    void* field_0x40;
    u8 pad_0x48[0x8];
    void** field_0x50;
    u8 pad_0x58[0x10];
    base::reflection::UnkReflValue value_0x68;
    UnkWeightPair_lac90a8 field_0x80;
    UnkWeightPair_lac90a8 field_0x90;
    UnkWeightPair_lac90a8 field_0xa0;
    UnkWeightPair_lac90a8 field_0xb0;
    void** field_0xc0;
    u8 pad_0xc8[0x10];
    base::reflection::UnkReflValue value_0xd8;
    UnkWeightPair_lac90a8 field_0xf0;
    UnkWeightPair_lac90a8 field_0x100;
    UnkWeightPair_lac90a8 field_0x110;
    UnkWeightPair_lac90a8 field_0x120;
    void** field_0x130;
    u8 pad_0x138[8];
    UnkWeightPair_lac90a8 field_0x140;
    u32 field_0x150;
    u8 pad_0x154[0x1e0 - 0x154];
    UnkWeightListener_lac90a8 listener;  // 0x1e0
    u8 pad_0x200[8];
    UnkStringData* id;  // 0x208
    UnkStrIdTable table;  // 0x210
};
static_assert(__builtin_offsetof(UnkWeightDtor_lac90a8, listener.link) == 0x1f8, "link");
static_assert(__builtin_offsetof(UnkWeightDtor_lac90a8, table) == 0x210, "table");

void fn_7100ac90a8(UnkWeightDtor_lac90a8* self) {
    self->vtable = g_unk_7101a971b8 + 2;
    self->field_0x40 = g_unk_7101a971b8 + 0x34;
    fn_71004f11f8(&self->table);
    if (self->table.keys)
        self->table.allocator->vf_0x48(self->table.keys);
    if (self->table.values)
        self->table.allocator->vf_0x48(self->table.values);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &self->id;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    self->listener.vtable = g_unk_7101a22d48 + 2;
    if (UnkWeightLink_lac90a8* link = self->listener.link) {
        if (UnkWeakTarget* owner = link->owner) {
            fn_710117b3f0(owner, &self->listener);
            link = self->listener.link;
            if (!link)
                goto cleared;
        }
        if (--link->ref_count == 0)
            link->Release();
    }
cleared:
    self->field_0x150 = 0;
    self->field_0x140 = UnkWeightPair_lac90a8();
    self->field_0xf0 = UnkWeightPair_lac90a8();
    self->field_0x120 = UnkWeightPair_lac90a8();
    self->field_0x110 = UnkWeightPair_lac90a8();
    self->field_0x130 = g_unk_7101a97478 + 2;
    self->field_0x100 = UnkWeightPair_lac90a8();
    self->field_0xc0 = g_unk_7101a97420 + 2;
    fn_71000a1c60(&self->value_0xd8);
    self->field_0xb0 = UnkWeightPair_lac90a8();
    self->field_0x50 = g_unk_7101a97420 + 2;
    self->field_0xa0 = UnkWeightPair_lac90a8();
    self->field_0x90 = UnkWeightPair_lac90a8();
    self->field_0x80 = UnkWeightPair_lac90a8();
    fn_71000a1c60(&self->value_0x68);
    fn_71010c4194(self);
}
