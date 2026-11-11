// CCharClassSoundProofTriggerComponent's unit (tools/tu_map.py, unit 1544): the trigger's links to actors,
// resolved on first use as src/game/CActor/UnkLinkVectorEqual_b54.cpp does. Names and layouts are placeholders.
#include <new>
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkLinkHandle_ovb0130 {
    u8 pad_0x0[0x10];
    void* target;
};

// A link resolved on first use, as src/game/CActor/UnkLinkVectorEqual_b54.cpp has it.
struct UnkLink_ovb0130 {
    u8 pad_0x0[0x8];
    u8 key[0x28];
    u32 state;
    u32 pad_0x34;
    UnkLinkHandle_ovb0130* handle;

    void* Get() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(key);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&handle),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        UnkLinkHandle_ovb0130* h = *reinterpret_cast<UnkLinkHandle_ovb0130**>(reinterpret_cast<u8*>(this) + 0x38);
        return h ? h->target : nullptr;
    }
};
static_assert(sizeof(UnkLink_ovb0130) == 0x40, "UnkLink_ovb0130 layout");

// fn_7100a30948: whether the list holds a link to the target of `link`.
bool fn_7100a30948(base::UnkVector<UnkLink_ovb0130>* self, UnkLink_ovb0130* link) {
    for (u64 i = 0; i < self->count; ++i) {
        UnkLink_ovb0130& entry = self->data[i];
        void* target = link->Get();
        if (entry.Get() == target)
            return true;
    }
    return false;
}

// The link's base: a vtable and a list allocating with flags 0xf.
struct UnkLinkBase_ovb0130 {
    void** vtable;
    void* data;  // 0x8
    u32 count;
    u32 capacity;
    u32 step;  // 0x18
    bool field_0x1c;
    u32 flags;  // 0x20
    base::UnkAllocator* allocator;  // 0x28
};

struct UnkTargetLink_ovb0130 : UnkLinkBase_ovb0130 {
    u32 state;  // 0x30
    u32 pad_0x34;
    UnkWeakHandle* handle;  // 0x38
};
static_assert(sizeof(UnkTargetLink_ovb0130) == 0x40, "UnkTargetLink_ovb0130 layout");

// fn_7100a30a9c: constructs a link already resolved (state 2) to the weak handle of `target`.
void fn_7100a30a9c(UnkTargetLink_ovb0130* self, UnkWeakTarget* target) {
    self->vtable = &g_unk_7101a20998[2];
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = 10;
    self->field_0x1c = true;
    self->flags = 0xf;
    self->allocator = fn_7100083990();
    self->state = 0;
    self->vtable = &g_unk_7101a20950[2];
    self->handle = nullptr;
    UnkRefPtr ref;
    UnkWeakHandle* h = GetWeakHandle(target);
    ref.field_0x0 = nullptr;
    if (h) {
        ++h->ref_count;
        ref.field_0x0 = h;
    }
    fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&self->handle), reinterpret_cast<const base::UnkRefHandle*>(&ref));
    self->state = 2;
}
