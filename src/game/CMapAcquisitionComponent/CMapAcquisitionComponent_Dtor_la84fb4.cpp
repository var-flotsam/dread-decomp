// CMapAcquisitionComponent unit (tools/tu_map.py): the component's destructor. Unregisters the timeline
// listener at +0x820, clears and frees the id table at +0x7d0, releases the string id at +0x7c8, destroys the
// four states (last first) and the state machine at +0x548, releases the string id at +0x540 and runs the
// usable component's destructor (fn_7100a9e6bc). Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CPullOffGrapplePointComponent.h"
#include "game/CTimelineComponent.h"
#include "game/CUsableComponent.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a91d50[];  // the class's vtable
extern void* g_unk_7101a92218[];  // the states' base vtable
extern void* g_unk_7101a92270[];  // the state machine's vtable

struct UnkMapState_la84fb4 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    u64 block[8];

    void Destroy() {
        for (int i = 7; i >= 0; --i)
            block[i] = 0;
        vtable = g_unk_7101a92218 + 2;
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkMapState_la84fb4) == 0x70, "layout");

struct UnkMapMachine_la84fb4 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 count;

    void Destroy() {
        vtable = g_unk_7101a92270 + 2;
        field_0x10 = 0;
        field_0x18 = 0;
        count = 0;
    }
};

struct UnkMapAcquisition_la84fb4 {
    void** vtable;
    u8 pad_0x8[0x540 - 0x8];
    CStrId name;                       // 0x540
    UnkMapMachine_la84fb4 machine;     // 0x548
    u8 pad_0x570[0x5f8 - 0x570];
    UnkMapState_la84fb4 states[4];     // 0x5f8
    u8 pad_0x7b8[0x7c8 - 0x7b8];
    CStrId id;                         // 0x7c8
    UnkStrIdTable table;               // 0x7d0
    u8 pad_0x7f8[0x820 - 0x7f8];
    UnkAttackListenerRaw listener;     // 0x820
};
static_assert(__builtin_offsetof(UnkMapAcquisition_la84fb4, states) == 0x5f8, "layout");
static_assert(__builtin_offsetof(UnkMapAcquisition_la84fb4, table) == 0x7d0, "layout");
static_assert(__builtin_offsetof(UnkMapAcquisition_la84fb4, listener) == 0x820, "layout");

void fn_7100a84fb4(UnkMapAcquisition_la84fb4* self) {
    self->vtable = g_unk_7101a91d50 + 2;
    self->listener.Destroy(g_unk_7101a77000, fn_7100a2d4d0);
    fn_71004f11f8(&self->table);
    if (self->table.keys)
        self->table.allocator->vf_0x48(self->table.keys);
    if (self->table.values)
        self->table.allocator->vf_0x48(self->table.values);
    self->id.Reset();
    for (int i = 3; i >= 0; --i)
        self->states[i].Destroy();
    self->machine.Destroy();
    self->name.Reset();
    fn_7100a9e6bc(reinterpret_cast<char*>(self));
}
