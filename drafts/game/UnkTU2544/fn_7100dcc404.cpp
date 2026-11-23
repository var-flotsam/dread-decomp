// CBossRushSurvivalConfiguration's unit (tools/tu_map.py unit 2544): starts a boss rush or dread rush run
// (survival or not): resets the run's state, fills the survival item list, copies the record the run
// competes against, enters the "INGAME" mode and requests the running state. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2544.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;

extern CStrId g_unk_71727bdb88 __attribute__((visibility("hidden")));
void fn_7100dffe04(UnkGameManager* manager, s32 value);

// The tunable's starting values of a survival run.
struct UnkRushTunables_dcc404 {
    u8 pad_0x0[0x28];
    s32 field_0x28;
    s32 life;      // 0x2c
    s32 missiles;  // 0x30
};

// The run's state machine (the template of src/game/UnkStateMachineSetA5_b61.cpp).
struct UnkSmState_dcc404 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_dcc404* __attribute__((aligned(4))) UnkSmStatePtr_dcc404;

struct UnkSmMachine_dcc404 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_dcc404* previous, UnkSmState_dcc404* next);
    virtual void vf_0x28(UnkSmState_dcc404* current, UnkSmState_dcc404* next);

    u64 field_0x8;
    UnkSmState_dcc404* current;
    UnkSmState_dcc404* previous;
    u32 count;
    UnkSmStatePtr_dcc404 states[16];
    u32 pending;
    bool busy;
};

static const u32 kNoState = 4;

// The state with the given id, or false when the machine has none.
static inline bool FindState(UnkSmMachine_dcc404* machine, u32 id, UnkSmState_dcc404*& out) {
    UnkSmStatePtr_dcc404* end = machine->states + machine->count;
    for (UnkSmStatePtr_dcc404* p = machine->states; p != end; ++p) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (0 leaves every state) unless the machine is busy (then it stays pending).
static inline void RequestState(UnkSmMachine_dcc404* machine, u32 id) {
    machine->pending = id;
    if (machine->busy)
        return;
    UnkSmState_dcc404* current = machine->current;
    if (!current || current->id != id) {
        UnkSmState_dcc404* found;
        bool enter = FindState(machine, id, found);
        if (enter || id == 0) {
            if (current) {
                current->vf_0x18(id);
                machine->vf_0x28(machine->current, found);
            }
            machine->previous = machine->current;
            machine->current = found;
            if (enter) {
                found->vf_0x10(machine->previous ? machine->previous->id : kNoState);
                machine->vf_0x20(machine->previous, found);
            }
        }
    }
    machine->pending = kNoState;
}

// An item the survival run starts with, and its amount.
struct UnkRushItem_dcc404 {
    UnkPropertyEx<>* item;
    s32 amount;
};

// Appends an item, growing the array to the next multiple of the step (the elements are copied one by one).
static inline void AddItem(CRntVector<UnkRushItem_dcc404>& items, const UnkRushItem_dcc404& value) {
    u32 n = items.count;
    u32 old_capacity = items.capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && items.step != 0)
        new_capacity = (n / items.step + 1) * items.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkRushItem_dcc404* p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkRushItem_dcc404*>(
            items.allocator->vf_0x40((u64)new_capacity * sizeof(UnkRushItem_dcc404), 8, items.flags));
        if (items.data) {
            UnkRushItem_dcc404* dst = p;
            UnkRushItem_dcc404* src = items.data;
            for (u32 i = 0; i < items.count; ++i)
                *dst++ = *src++;
            items.allocator->vf_0x48(items.data);
        }
        n = items.count;
        items.data = p;
        items.capacity = new_capacity;
    } else {
        p = items.data;
    }
    p[n] = value;
    ++items.count;
}

// The survival settings created on first use (a growable list and an active flag).
struct UnkRushBoard_dcc404 {
    CRntVector<u64> list{0xf};
    bool active = true;
};

// A level-times dictionary of a record (copied by fn_7100dcf980).
struct UnkRushTimes_dcc404;
void fn_7100dcf980(UnkRushTimes_dcc404* self, const UnkRushTimes_dcc404* other);

struct UnkRushTimes_dcc404 {
    u8 data[0x28];

    UnkRushTimes_dcc404& operator=(const UnkRushTimes_dcc404& other) {
        fn_7100dcf980(this, &other);
        return *this;
    }
};

// A CBossRushManager::SRushRecord.
struct UnkRushRecord_dcc404 {
    UnkRushTimes_dcc404 times[4];
    s32 field_0xa0;
    s32 field_0xa4;
};

struct UnkRushLevelTime_dcc404 {
    f32 time;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkRushStart_dcc404 {
    void** vtable;
    UnkSmMachine_dcc404 machine;  // 0x8
    bool started;                 // 0xb8
    bool dread_rush;              // 0xb9
    bool flag_0xba;
    bool survival;  // 0xbb
    s32 field_0xbc;
    s32 field_0xc0;
    f32 field_0xc4;
    f32 field_0xc8;
    f32 field_0xcc;
    s32 field_0xd0;
    u8 pad_0xd4[4];
    CRntVector<UnkRushItem_dcc404> items;  // 0xd8
    u32 level;                             // 0x100
    u8 pad_0x104[0x998 - 0x104];
    UnkRushLevelTime_dcc404 levels[13];  // 0x998
    u8 pad_0xa34[4];
    f64 time;  // 0xa38
    u8 pad_0xa40[0xa68 - 0xa40];
    UnkRushRecord_dcc404 boss_rush_record;   // 0xa68
    UnkRushRecord_dcc404 dread_rush_record;  // 0xb10
    UnkRushRecord_dcc404 record;             // 0xbb8: the one this run competes against
    s32 survival_record;                     // 0xc60
    f32 survival_time;                       // 0xc64
    bool new_record;                         // 0xc68
    u8 pad_0xc69[3];
    s32 best;       // 0xc6c
    f32 best_time;  // 0xc70
    s32 runs;       // 0xc74
    u8 pad_0xc78[0xdf4 - 0xc78];
    bool flag_0xdf4;
    u8 pad_0xdf5[0x13e8 - 0xdf5];
    f32 field_0x13e8;
    f32 field_0x13ec;
};
static_assert(__builtin_offsetof(UnkRushStart_dcc404, items) == 0xd8, "layout");
static_assert(__builtin_offsetof(UnkRushStart_dcc404, level) == 0x100, "layout");
static_assert(__builtin_offsetof(UnkRushStart_dcc404, survival_record) == 0xc60, "layout");
static_assert(__builtin_offsetof(UnkRushStart_dcc404, field_0x13e8) == 0x13e8, "layout");

void fn_7100dcc404(UnkRush_b0195* rush, bool dread_rush, bool survival, bool flag) {
    UnkRushStart_dcc404* self = reinterpret_cast<UnkRushStart_dcc404*>(rush);
    fn_7100e8f9a0(static_cast<char*>(g_unk_7101d47310->field_0x2228), false, &g_unk_71727bdb88);
    if (self->started)
        return;
    self->flag_0xdf4 = false;
    self->field_0x13e8 = 0.0f;
    self->field_0x13ec = -1.0f;
    self->survival = survival;
    if (survival) {
        const UnkRushTunables_dcc404* tunables = reinterpret_cast<const UnkRushTunables_dcc404*>(&g_unk_71727bd078);
        self->best = self->survival_record;
        self->best_time = self->survival_time;
        self->field_0xc0 = 0;
        self->field_0xd0 = 0;
        self->field_0xcc = 0.0f;
        self->field_0xbc = tunables->field_0x28;
        self->field_0xc4 = 0.0f;
        self->field_0xc8 = 0.0f;
        self->items.count = 0;
        AddItem(self->items, {&g_unk_717275a218, tunables->missiles});
        AddItem(self->items, {&g_unk_717275a1d8, tunables->missiles});
        AddItem(self->items, {&g_unk_717275a058, tunables->life});
        AddItem(self->items, {&g_unk_717275a118, tunables->life});
    }
    UnkRushBoard_dcc404* board = reinterpret_cast<UnkRushBoard_dcc404*>(g_unk_71726bc568);
    if (!board) {
        board = new (fn_71000839a0(0x30, 0x3f)) UnkRushBoard_dcc404();
        g_unk_71726bc568 = reinterpret_cast<UnkSettings_b0158*>(board);
    }
    board->active = false;
    self->dread_rush = dread_rush;
    fn_7100dffe04(g_unk_7101d47310, dread_rush ? 2 : 0);
    self->flag_0xba = flag;
    self->started = true;
    UnkRushRecord_dcc404* record = self->dread_rush ? &self->dread_rush_record : &self->boss_rush_record;
    self->new_record = false;
    self->record = *record;
    for (u32 i = 0; i < 13; ++i) {
        self->levels[i].time = -1.0f;
        self->levels[i].field_0x4 = -1.0f;
        self->levels[i].field_0x8 = -1.0f;
    }
    self->time = 0.0;
    self->runs = 0;
    self->level = flag;
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x498 || StringBuffer_Differs(static_cast<char*>(manager->field_0x498) + 8, "INGAME"))
        fn_7100deac9c(g_unk_7101d47310, CRntString("INGAME"));
    RequestState(&self->machine, 3);
}
