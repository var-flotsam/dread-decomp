// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): the component's destructor.
// Clears and frees the id table at +0x570, releases the string id at +0x568, unregisters the watch at +0x540,
// destroys the seven states (last first) and the state machine at +0x180, releases the track name at +0x178 and
// runs the base destructor (fn_7100a02c54). Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a8b3a8[];  // the class's vtable
extern void* g_unk_7101a8b630[];  // the states' base vtable
extern void* g_unk_7101a8b688[];  // the state machine's vtable

struct UnkObj_7100a02c54;
void fn_7100a02c54(UnkObj_7100a02c54* self);

struct UnkWapState_la09060 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    u64 block[8];

    void Destroy() {
        for (int i = 0; i < 8; i++)
            block[i] = 0;
        vtable = g_unk_7101a8b630 + 2;
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkWapState_la09060) == 0x70, "layout");

struct UnkWapMachine_la09060 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 count;

    void Destroy() {
        vtable = g_unk_7101a8b688 + 2;
        field_0x10 = 0;
        field_0x18 = 0;
        count = 0;
    }
};

struct UnkWapWatch_la09060 {
    base::UnkDeferredSet<void>* set;
    u64 field_0x8;
    u64 field_0x10;

    void Destroy() {
        if (set) {
            fn_71004f3ea4(set, &set);
            set = nullptr;
        }
        field_0x10 = 0;
        field_0x8 = 0;
    }
};

struct UnkWeightPlatform_la09060 {
    void** vtable;
    u8 pad_0x8[0x178 - 0x8];
    CStrId track;                    // 0x178
    UnkWapMachine_la09060 machine;   // 0x180
    u8 pad_0x1a8[0x230 - 0x1a8];
    UnkWapState_la09060 states[7];   // 0x230
    UnkWapWatch_la09060 watch;       // 0x540
    u8 pad_0x558[0x568 - 0x558];
    CStrId name;                     // 0x568
    UnkStrIdTable table;             // 0x570
};
static_assert(__builtin_offsetof(UnkWeightPlatform_la09060, states) == 0x230, "layout");
static_assert(__builtin_offsetof(UnkWeightPlatform_la09060, watch) == 0x540, "layout");
static_assert(__builtin_offsetof(UnkWeightPlatform_la09060, table) == 0x570, "layout");

void fn_7100a09060(UnkWeightPlatform_la09060* self) {
    self->vtable = g_unk_7101a8b3a8 + 2;
    fn_71004f11f8(&self->table);
    if (self->table.keys)
        self->table.allocator->vf_0x48(self->table.keys);
    if (self->table.values)
        self->table.allocator->vf_0x48(self->table.values);
    self->name.Reset();
    self->watch.Destroy();
    for (int i = 6; i >= 0; --i)
        self->states[i].Destroy();
    self->machine.Destroy();
    self->track.Reset();
    fn_7100a02c54(reinterpret_cast<UnkObj_7100a02c54*>(self));
}
