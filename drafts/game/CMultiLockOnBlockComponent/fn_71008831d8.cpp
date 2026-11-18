// CMultiLockOnBlockComponent's unit (tools/tu_map.py 1339): until the block is open, counts down each locked
// point (unlocking it once its time passes the tunable's), and when every point is locked and each is flagged,
// opens the block: tells the owner, hooks the owner's timeline, and notifies the listeners.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU372.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkLockOnTarget_A5_b68 {
    u8 pad_0x0[0x3d];
    bool field_0x3d;
};

UnkLockOnTarget_A5_b68* fn_710031ca1c(u64 key);
void fn_7100a2d4c8(UnkWeakTarget* target, void* listener);
void fn_710082efe8(UnkTimelineComponent* timeline, UnkWeakHandle** out);
u32 fn_7100883d84(char* p0, char* p1);

struct UnkClone_fn_710082d0ac;
using UnkMultiLockOnListeners = base::UnkDeferredSet<base::UnkDeferredCallback<u64>>;
extern template void base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(UnkMultiLockOnListeners*, const u64*);

struct UnkTunable;
extern UnkTunable g_unk_717272b600;
struct UnkMultiLockOnLists;
extern UnkMultiLockOnLists g_unk_717272b660;
extern u64 g_unk_710164ae58;
extern u64 g_unk_710164ae60;
extern u64 g_unk_710164ae68;

// The unit's tunable as this function reads it.
struct UnkMultiLockOnTunable_l8831d8 {
    u8 pad_0x0[0x24];
    f32 lock_time;  // 0x24
};

struct UnkMultiLockOnPoint_l8831d8 {
    UnkActor* actor;
    bool locked;       // 0x8
    u8 pad_0x9[3];
    f32 timer;         // 0xc
    u64 lock_id;       // 0x10
    u64 relax_id;      // 0x18
    bool ready;        // 0x20
    u8 pad_0x21[7];
    UnkMultiLockOnPoint_l8831d8* next;  // 0x28
};

// A listener: the object and function it calls, and the counted link to what it listens to.
struct UnkMultiLockOnListener_l8831d8 {
    void** vtable;
    void* context;                    // 0x8
    u32 (*callback)(char*, char*);    // 0x10
    UnkWeakHandle* link;              // 0x18
};

struct UnkMultiLockOnBlock_l8831d8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[5];
    bool open;        // 0x3d
    u8 pad_0x3e[2];
    s32 point_count;  // 0x40
    s32 locked_count; // 0x44
    u8 pad_0x48[8];
    UnkMultiLockOnListener_l8831d8 listener;  // 0x50
    u8 pad_0x70[0x6c];
    u32 field_0xdc;
    u8 pad_0xe0[0x30];
    UnkMultiLockOnPoint_l8831d8* points;  // 0x110

    // vtable slot 0x168
    void vf_0x168(bool flag) {
        reinterpret_cast<void (*)(UnkMultiLockOnBlock_l8831d8*, bool)>(vtable[0x168 / 8])(this, flag);
    }
};

void fn_71008831d8(UnkMultiLockOnBlock_l8831d8* self, f32 dt) {
    if (self->open || !self->field_0xdc)
        return;
    const UnkMultiLockOnTunable_l8831d8* tunable =
        reinterpret_cast<const UnkMultiLockOnTunable_l8831d8*>(&g_unk_717272b600);
    for (UnkMultiLockOnPoint_l8831d8* p = self->points; p; p = p->next) {
        if (!p->locked)
            continue;
        if (!(p->timer >= tunable->lock_time)) {
            p->timer += dt;
            continue;
        }
        p->locked = false;
        p->timer = 0;
        --self->locked_count;
        if (UnkLockOnTarget_A5_b68* target = fn_710031ca1c(reinterpret_cast<u64>(p->actor)))
            target->field_0x3d = false;
        fn_7100d7c94c(p->actor, &g_unk_710164ae68);
        fn_7100d7c94c(self->owner, &p->lock_id);
        fn_7100d7c828(p->actor, &g_unk_710164ae60, false);
        fn_7100d7c828(self->owner, &p->relax_id, false);
    }
    if (self->locked_count != self->point_count)
        return;
    for (UnkMultiLockOnPoint_l8831d8* p = self->points; p; p = p->next) {
        if (!p->ready)
            return;
    }
    self->open = true;
    fn_7100d7c828(self->owner, &g_unk_710164ae58, false);
    if (UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        UnkMultiLockOnListener_l8831d8* listener = &self->listener;
        listener->context = self;
        listener->callback = fn_7100883d84;
        if (UnkWeakHandle* link = listener->link) {
            if (UnkWeakTarget* target = link->target)
                fn_7100a2d4d0(target, listener);
            if (UnkWeakHandle* p = listener->link) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                listener->link = nullptr;
            }
        }
        fn_710082efe8(timeline, &self->listener.link);
        fn_7100a2d4c8(listener->link ? listener->link->target : nullptr, listener);
    }
    self->vf_0x168(true);
    if (void* switcher = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(switcher, 0xb15b31b835c17820);
    for (UnkMultiLockOnPoint_l8831d8* p = self->points; p; p = p->next)
        fn_7100d7c828(p->actor, &g_unk_710164ae58, false);
    u64 arg = reinterpret_cast<u64>(self);
    base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(
        reinterpret_cast<UnkMultiLockOnListeners*>(&g_unk_717272b660), &arg);
}
