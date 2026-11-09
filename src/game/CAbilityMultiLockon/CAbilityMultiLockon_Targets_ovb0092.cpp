// CAbilityMultiLockon's unit (tools/tu_map.py, unit 431): the ability's lock-on bookkeeping. Five slots
// (+0x3a0) hold weak handles of the targets being aimed at; the locked targets are a vector of counted
// handles (+0x3c8, grown through fn_710031da18); the on-screen marks are a growing array (+0x700) of
// {fade time, mark} records. Releasing a held target (fn_710031965c) drops it from the game manager's
// points, its component at +0x2e8 and its two states; locking one (fn_710031a13c) plays its lock
// timelines and appends its handle; a new mark starts fully visible (fn_710031c7b8); the per-frame
// update (fn_7100319958) drops faded marks from the back and fades the others (alpha of the mark's
// model). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkStruct_71002334fc;
void fn_71002334fc(UnkStruct_71002334fc*, bool);
void fn_7100233910(UnkStruct_71002334fc*, bool);
void fn_71011a2a24(UnkGameManagerSub_0x608*, void*);
void fn_7100a2bc3c(void*);
void fn_7100d7c94c(UnkActor*, const u64*);
UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);
template <class Tag>
struct UnkSlotArray;
struct UnkTag_710031da18;
void fn_710031da18(UnkSlotArray<UnkTag_710031da18>* v, u32 capacity);

// The lock timelines' ids.
extern const u64 g_unk_7101648df8;
extern const u64 g_unk_7101648e08;
extern const u64 g_unk_7101648dc0;
extern const u64 g_unk_7101648e20;

// A held target as the release sees it.
struct UnkLockonTarget_ovb0092 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    u8 pad_0x1b0[0x138];
    void* field_0x2e8;
};

struct UnkLockonSlot_ovb0092 {
    u8 pad_0x0[0x10];
    UnkLockonTarget_ovb0092* target;
};

// A mark on screen: its fade time and the mark object.
struct UnkLockonEntry_ovb0092 {
    f32 time;
    void* target;

    UnkLockonEntry_ovb0092() : time(0.0f), target(nullptr) {}
};

struct UnkColor_ovb0092 {
    f32 r, g, b, a;
    UnkColor_ovb0092() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
};

struct UnkLockonModel_ovb0092;
void fn_71001bff28(UnkLockonModel_ovb0092* model, u32 index, UnkColor_ovb0092* color);
void fn_71001bfea4(UnkLockonModel_ovb0092* model, u32 index, const UnkColor_ovb0092* color);

struct UnkLockonMark_ovb0092 {
    u8 pad_0x0[0x8];
    u8 item[0x1b0];                 // 0x8: what the render manager draws
    UnkLockonModel_ovb0092* model;  // 0x1b8
};

// base::UnkVector's layout, with a growing element accessor and an append (as
// src/game/GUI/IDisplayObjectTrack/IDisplayObjectTrack_Keys_ovb0020.cpp).
template <class T>
struct UnkGrowArray_ovb0092 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool grow;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reallocate(u32 n) {
        T* p = static_cast<T*>(allocator->vf_0x40((u64)n * sizeof(T), 8, flags));
        if (T* old = data) {
            T* dst = p;
            T* src = old;
            for (u32 i = 0; i < count; ++i, ++dst, ++src)
                __builtin_memcpy(dst, src, sizeof(T));
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }

    __attribute__((always_inline)) T& At(u32 index) {
        if (grow && count <= index) {
            u32 cap = capacity;
            if (cap <= index && step != 0) {
                u32 grown = (index / step + 1) * step;
                if (cap < grown) {
                    Reallocate(grown);
                    cap = grown;
                }
            }
            u32 n = index + 1;
            if (cap < n)
                Reallocate(n);
            for (u32 i = count; i < n; ++i)
                data[i] = T();
            count = n;
        }
        return data[index];
    }

    __attribute__((always_inline)) void PushBack(const T& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        if (old_capacity < new_capacity) {
            Reallocate(new_capacity);
            n = count;
        }
        __builtin_memcpy(&data[n], &value, sizeof(T));
        ++count;
    }
};

using UnkLockonMarks_ovb0092 = UnkGrowArray_ovb0092<UnkLockonEntry_ovb0092>;

struct UnkHandlePtr_ovb0092 {
    UnkWeakHandle* p = nullptr;
};

// A reference held for the duration of the lock.
struct UnkHandleRef_ovb0092 {
    UnkWeakHandle* p;
    UnkHandleRef_ovb0092(UnkWeakHandle* h) : p(h) {
        if (p)
            ++p->ref_count;
    }
    ~UnkHandleRef_ovb0092() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

struct UnkMultiLockon_ovb0092 {
    u8 pad_0x0[0x3a0];
    UnkLockonSlot_ovb0092* slots[5];               // 0x3a0
    base::UnkVector<UnkHandlePtr_ovb0092> locked;  // 0x3c8
    u8 pad_0x3f0[0x310];
    UnkLockonMarks_ovb0092 marks;                  // 0x700
};
static_assert(__builtin_offsetof(UnkMultiLockon_ovb0092, marks) == 0x700, "UnkMultiLockon_ovb0092 layout");

// Releases `target` when one of the slots holds it.
void fn_710031965c(UnkMultiLockon_ovb0092* self, UnkStruct_71002334fc* target) {
    UnkLockonTarget_ovb0092* held;
    for (u32 i = 0; i < 5; ++i) {
        UnkLockonSlot_ovb0092* slot = self->slots[i];
        held = slot ? slot->target : nullptr;
        if (reinterpret_cast<UnkStruct_71002334fc*>(held) == target)
            goto found;
    }
    return;
found:
    if (held->field_0x1a8) {
        if (UnkGameManagerSub_0x608* points = g_unk_7101d47310->field_0x608)
            fn_71011a2a24(points, target);
    }
    UnkLockonTarget_ovb0092* t = reinterpret_cast<UnkLockonTarget_ovb0092*>(target);
    if (t->field_0x2e8)
        fn_7100a2bc3c(t->field_0x2e8);
    fn_7100233910(target, false);
    fn_71002334fc(target, false);
}

// The mark record at `index`, growing the array to reach it.
__attribute__((noinline)) UnkLockonEntry_ovb0092* fn_7100319aec(UnkLockonMarks_ovb0092* entries, u32 index) {
    return &entries->At(index);
}

// Drops the marks that have faded out (from the back), then fades the others by four times `dt`.
void fn_7100319958(UnkMultiLockon_ovb0092* self, f32 dt) {
    UnkLockonMarks_ovb0092* marks = &self->marks;
    for (s32 i = marks->count; i > 0;) {
        --i;
        UnkLockonEntry_ovb0092* e = fn_7100319aec(marks, i);
        auto* mark = static_cast<UnkLockonMark_ovb0092*>(e->target);
        if (!(e->time <= 0.0f)) {
            if (mark)
                continue;
        } else if (mark) {
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(mark->item), 0);
            e->target = nullptr;
        }
        if (marks->count > (u32)i) {
            if (marks->count - 1 > (u32)i) {
                u64 j = (u32)i;
                do {
                    marks->data[j] = marks->data[j + 1];
                    ++j;
                } while (j < marks->count - 1);
            }
            marks->count--;
        }
    }
    u32 n = marks->count;
    if (n == 0)
        return;
    f32 step = dt * 4.0f;
    UnkLockonEntry_ovb0092* e = marks->data;
    for (UnkLockonEntry_ovb0092* end = e + n; e != end; ++e) {
        auto* mark = static_cast<UnkLockonMark_ovb0092*>(e->target);
        if (!mark)
            continue;
        UnkColor_ovb0092 color;
        fn_71001bff28(mark->model, 0, &color);
        f32 t = e->time - step;
        t = t < 0.0f ? 0.0f : t;
        e->time = t;
        color.a = t <= 0.0f ? 0.0f : t;
        fn_71001bfea4(mark->model, 0, &color);
    }
}

// Adds a fully visible mark.
void fn_710031c7b8(UnkMultiLockon_ovb0092* self, void* mark) {
    if (!mark)
        return;
    UnkLockonEntry_ovb0092 entry;
    entry.time = 1.0f;
    entry.target = mark;
    self->marks.PushBack(entry);
}

// Locks `actor`: its lock timelines (the second one by `flag`), then its handle appended to the list.
void fn_710031a13c(UnkMultiLockon_ovb0092* self, UnkActor* actor, bool flag) {
    if (!actor)
        return;
    fn_7100d7c94c(actor, &g_unk_7101648df8);
    fn_7100d7c94c(actor, &g_unk_7101648e08);
    fn_7100d7c828(actor, flag ? &g_unk_7101648e20 : &g_unk_7101648dc0, false);
    UnkHandleRef_ovb0092 ref(fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(actor)));
    base::UnkVector<UnkHandlePtr_ovb0092>& v = self->locked;
    u32 n = v.count;
    u32 capacity = v.capacity;
    if (capacity <= n && v.step != 0)
        capacity = (n / v.step + 1) * v.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_710031da18(reinterpret_cast<UnkSlotArray<UnkTag_710031da18>*>(&v), capacity);
    UnkWeakHandle** slot = &v.data[v.count].p;
    // Cleared as raw memory: the original reloads the count after it.
    __builtin_memset(slot, 0, sizeof(*slot));
    AssignRef(slot, ref.p);
    ++v.count;
}
