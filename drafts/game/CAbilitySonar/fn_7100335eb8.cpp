// CAbilitySonar's unit (tools/tu_map.py unit 434): gathers every sonar target (fn_7100a107e4) into the
// list at +0x3a0, sorts it nearest first, offers each target's component the pulse until 64 are found,
// then empties the list. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include <new>

#include "types.h"

template <class Tag>
struct UnkSlotArray;
struct UnkTag_7100336b54;
void fn_7100336b54(UnkSlotArray<UnkTag_7100336b54>* self, u32 capacity);

extern base::reflection::UnkTunable g_unk_71726b0c48;

struct UnkSonarTunable_l335eb8 {
    u8 pad_0x0[0x2c];
    f32 range;  // 0x2c
};

struct UnkSonarVec3_l335eb8 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkSonarVec2_l335eb8 {
    f32 x;
    f32 y;
};

struct UnkSonarTargetComponent_l335eb8 {
    struct VTable {
        const base::reflection::CType* (*GetClass)(UnkSonarTargetComponent_l335eb8* self);
        void* slots[0x168 / 8 - 1];
        bool (*vf_0x168)(UnkSonarTargetComponent_l335eb8* self, const UnkSonarVec2_l335eb8* pos, f32 range);
        void (*vf_0x170)(UnkSonarTargetComponent_l335eb8* self, const UnkSonarVec2_l335eb8* pos, f32 range,
                         void* arg, u32* found);
    };
    VTable* vtable;
};

struct UnkSonarTarget_l335eb8 {
    struct VTable {
        void* slots[2];
        void (*Delete)(UnkSonarTarget_l335eb8* self);
    };
    VTable* vtable;
    s32 ref_count;
    UnkSonarTargetComponent_l335eb8* component;  // 0x10
};

struct UnkSonarTargetList_l335eb8 {
    UnkSonarTarget_l335eb8** data;
    u32 count;

    UnkSonarTarget_l335eb8** begin() { return data; }
    UnkSonarTarget_l335eb8** end() { return data + count; }
};

UnkSonarTargetList_l335eb8* fn_7100a107e4();

// A counted reference to a target.
struct UnkSonarSlot_l335eb8 {
    UnkSonarTarget_l335eb8* ptr;

    UnkSonarSlot_l335eb8() : ptr(nullptr) {}
    void Assign(UnkSonarTarget_l335eb8* p) {
        if (p) {
            ++p->ref_count;
            if (UnkSonarTarget_l335eb8* old = ptr) {
                if (--old->ref_count == 0)
                    old->vtable->Delete(old);
            }
            ptr = p;
        }
    }
    void Release() {
        if (UnkSonarTarget_l335eb8* old = ptr) {
            if (--old->ref_count == 0)
                old->vtable->Delete(old);
        }
    }
};

// base::global::CRntVector's layout, its reserve out of line.
struct UnkSonarSlotList_l335eb8 {
    UnkSonarSlot_l335eb8* data;
    u32 count;
    u32 capacity;
    u32 step;

    UnkSonarSlot_l335eb8* begin() { return data; }
    UnkSonarSlot_l335eb8* end() { return data + count; }

    void PushBack(UnkSonarTarget_l335eb8* const& value) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        // A capacity that doesn't grow past the count (no step) hangs, as in the original.
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_7100336b54(reinterpret_cast<UnkSlotArray<UnkTag_7100336b54>*>(this), new_capacity);
        UnkSonarSlot_l335eb8* slot = new (&data[count]) UnkSonarSlot_l335eb8();
        slot->Assign(value);
        ++count;
    }
    void Clear() {
        for (u32 i = 0; i < count; ++i)
            data[i].Release();
        count = 0;
    }
};

// Orders targets by distance to a point (fn_7100336ca0, std::sort's body).
struct UnkSonarNearer_l335eb8 {
    const UnkSonarVec3_l335eb8* target;
};
void fn_7100336ca0(UnkSonarSlot_l335eb8* first, UnkSonarSlot_l335eb8* last, UnkSonarNearer_l335eb8* comp);

struct UnkSonarOwnerData_l335eb8 {
    u8 pad_0x0[0x64];
    f32 z;  // 0x64
};
struct UnkSonarOwner_l335eb8 {
    u8 pad_0x0[0x30];
    UnkSonarOwnerData_l335eb8* data;  // 0x30
};

struct UnkAbilitySonar_l335eb8 {
    u8 pad_0x0[0xc0];
    UnkSonarOwner_l335eb8* owner;  // 0xc0
    u8 pad_0xc8[0x3a0 - 0xc8];
    UnkSonarSlotList_l335eb8 targets;  // 0x3a0
    u8 pad_0x3b8[0x6e0 - 0x3b8];
    UnkSonarVec2_l335eb8 pos;  // 0x6e0
};
static_assert(__builtin_offsetof(UnkAbilitySonar_l335eb8, targets) == 0x3a0, "layout");
static_assert(__builtin_offsetof(UnkAbilitySonar_l335eb8, pos) == 0x6e0, "layout");

inline UnkSonarTargetComponent_l335eb8* AsSonarTarget(UnkSonarTargetComponent_l335eb8* c) {
    return fn_710009d50c(c->vtable->GetClass(c), fn_7100a10580()) ? c : nullptr;
}

void fn_7100335eb8(UnkAbilitySonar_l335eb8* self, void* arg, u32* found) {
    UnkSonarTargetList_l335eb8* all = fn_7100a107e4();
    for (UnkSonarTarget_l335eb8** it = all->begin(); it != all->end(); ++it)
        self->targets.PushBack(*it);

    UnkSonarNearer_l335eb8 nearer;
    UnkSonarVec3_l335eb8 target = {self->pos.x, self->pos.y, self->owner->data->z};
    nearer.target = &target;
    fn_7100336ca0(self->targets.begin(), self->targets.end(), &nearer);

    const UnkSonarTunable_l335eb8* tunable = reinterpret_cast<const UnkSonarTunable_l335eb8*>(&g_unk_71726b0c48);
    for (UnkSonarSlot_l335eb8* it = self->targets.begin(); it != self->targets.end() && *found < 64; ++it) {
        UnkSonarTarget_l335eb8* t = it->ptr;
        UnkSonarTargetComponent_l335eb8* c = nullptr;
        if (t) {
            c = t->component;
            if (c)
                c = AsSonarTarget(c);
        }
        if (c->vtable->vf_0x168(c, &self->pos, tunable->range))
            c->vtable->vf_0x170(c, &self->pos, tunable->range, arg, found);
    }
    self->targets.Clear();
}
