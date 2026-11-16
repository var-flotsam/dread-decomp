// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): the component's constructor.
// Builds the smart-object base (fn_7100a028c4), the timer limit (0.3) and the two slot colours, the empty track
// name, the state machine (no state yet, "none" 6) and its seven states (ids 1 to 5, 7 and 6), the watch and its
// delegate (fn_7100a0d480), the id map at +0x560 (fn_7100a0cfe8) and the key at +0x5a0. Names and layouts are
// placeholders.
#include "base/UnkDeferredSet.h"
#include "base/color/CColor4F.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

extern void* g_unk_7101a8b3a8[];  // the class's vtable
extern void* g_unk_7101a8b5d8[];  // the states' vtable
extern void* g_unk_7101a8b688[];  // the state machine's vtable

void fn_7100a028c4(void* p);

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}
bool fn_7100a0d480(UnkWeightPlatform_b49a4* self, bool on);

struct UnkWapIdMap_la08d5c {
    u8 pad_0x0[0x40];
};
void fn_7100a0cfe8(UnkWapIdMap_la08d5c* self);

struct UnkWapState_la08d5c {
    void** vtable;
    u64 field_0x8;
    u32 id;
    base::reflection::UnkReflValue value;
    u64 block[8];

    void Init(u32 stateId) {
        field_0x8 = 0;
        id = stateId;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
        for (int i = 0; i < 8; i++)
            block[i] = 0;
        vtable = g_unk_7101a8b5d8 + 2;
    }
};
static_assert(sizeof(UnkWapState_la08d5c) == 0x70, "layout");

typedef void* __attribute__((aligned(4))) UnkWapStatePtr_la08d5c;

struct UnkWapMachine_la08d5c {
    void** vtable;
    u64 field_0x8;
    void* current;
    void* previous;
    u32 count;
    UnkWapStatePtr_la08d5c states[16];
    u32 pending;
    bool busy;
    bool field_0x49;

    void Init() {
        vtable = g_unk_7101a8b688 + 2;
        field_0x8 = 0;
        current = nullptr;
        previous = nullptr;
        count = 0;
        pending = 6;
        busy = false;
        field_0x49 = false;
    }
};
static_assert(__builtin_offsetof(UnkWapMachine_la08d5c, pending) == 0xa4, "layout");

struct UnkWapDelegate_la08d5c {
    UnkWeightPlatform_b49a4* object;
    bool (*function)(UnkWeightPlatform_b49a4* self, bool on);
    bool field_0x10;
};

struct UnkWeightPlatform_la08d5c {
    void** vtable;
    u8 pad_0x8[0x148 - 0x8];
    f32 limit;      // 0x148
    f32 timer;      // 0x14c
    CColor4F to;    // 0x150
    CColor4F from;  // 0x160
    u32 slot;       // 0x170
    u32 linkedSlot;                   // 0x174
    CStrId track;                     // 0x178
    UnkWapMachine_la08d5c machine;    // 0x180
    UnkWapState_la08d5c states[7];    // 0x230
    base::UnkDeferredSet<void>* watch;  // 0x540
    UnkWapDelegate_la08d5c delegate;  // 0x548
    UnkWapIdMap_la08d5c ids;          // 0x560
    u64 key;                          // 0x5a0
    bool field_0x5a8;
};
static_assert(__builtin_offsetof(UnkWeightPlatform_la08d5c, states) == 0x230, "layout");
static_assert(__builtin_offsetof(UnkWeightPlatform_la08d5c, watch) == 0x540, "layout");
static_assert(__builtin_offsetof(UnkWeightPlatform_la08d5c, ids) == 0x560, "layout");
static_assert(__builtin_offsetof(UnkWeightPlatform_la08d5c, key) == 0x5a0, "layout");

void fn_7100a08d5c(UnkWeightPlatform_la08d5c* self) {
    fn_7100a028c4(self);
    self->vtable = g_unk_7101a8b3a8 + 2;
    self->limit = 0.3f;
    self->timer = 0.0f;
    self->to = CColor4F(0.0f, 0.0f, 0.0f, 1.0f);
    self->from = CColor4F(1.0f, 1.0f, 0.0f, 1.0f);
    self->slot = 0;
    self->linkedSlot = 0;
    InitEmptyStrId(&self->track);
    self->machine.Init();
    static const u32 kIds[7] = {1, 2, 3, 4, 5, 7, 6};
    for (int i = 0; i < 7; i++)
        self->states[i].Init(kIds[i]);
    self->watch = nullptr;
    self->delegate.object = reinterpret_cast<UnkWeightPlatform_b49a4*>(self);
    self->delegate.function = fn_7100a0d480;
    self->delegate.field_0x10 = false;
    fn_7100a0cfe8(&self->ids);
    fn_7100066630(&self->key, &g_unk_7101d58d10);
    self->field_0x5a8 = false;
}
