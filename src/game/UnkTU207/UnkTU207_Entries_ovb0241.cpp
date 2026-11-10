// Unit 207 of tools/tu_map.py (no reliable name): a controller that tracks the targets (material
// sets and models) an effect is applied to. It keeps a vector of (target, name id) entries at +0x10
// (fn_710016b21c grows it): fn_710016aa9c and fn_710016ac04 mark the target's sub-objects as
// restoring and add an entry (under a fixed id or under the id they are given), fn_710016ad60
// restores the materials of a target and drops its entries, fn_710016b34c erases an entry, and
// fn_710016a8dc swaps the object whose two tints it saves and restores. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;
using base::math::UnkVector4D;

// ---- The controller ----

struct UnkEntry_u207 {
    void* target;
    CStrId id;
    UnkEntry_u207(const UnkEntry_u207& other) : target(other.target), id(other.id) {}
};

struct UnkModel_u207 {
    u8 pad_0x0[0xa9];
    u8 field_0xa9;
    u8 pad_0xaa[6];
    u8 field_0xb0;
};

struct UnkController_u207 {
    u8 pad_0x0[8];
    UnkModel_u207* model;                    // 0x8
    base::UnkVector<UnkEntry_u207> entries;  // 0x10
};

void fn_710016b21c(base::UnkVector<UnkEntry_u207>* self, u32 capacity);

// Erases the entry at `index`: its id is released, the later entries move down and the moved-from
// ones are released again (UnkVector::EraseAt, inlined).
void fn_710016b34c(base::UnkVector<UnkEntry_u207>* self, u32 index) {
    if (index < self->count) {
        self->data[index].id.Reset();
        for (u32 i = index; i < (u32)(self->count - 1); ++i) {
            UnkEntry_u207* cur = &self->data[i];
            new (cur) UnkEntry_u207(cur[1]);
            (&self->data[i])[1].id.Reset();
        }
        self->count = self->count - 1;
    }
}

// ---- Restoring a material set and dropping its entries ----

struct UnkMaterial;
void fn_71001a6ec8(UnkMaterial* self, u32 flags);

struct UnkMaterialSet_ad60 {
    u8 pad_0x0[0x2c];
    u32 count;  // 0x2c
    u8 pad_0x30[0x18];
    UnkMaterial** materials;  // 0x48
};

void fn_710016ad60(UnkController_u207* self, UnkMaterialSet_ad60* set) {
    if (self->model->field_0xa9 && self->model->field_0xb0) {
        u32 count = set->count;
        for (u64 i = 0; i < count; ++i)
            fn_71001a6ec8(set->materials[i], 0x10);
    }
    auto* v = &self->entries;
    for (UnkEntry_u207* it = v->data; v->data + v->count != it;) {
        if (it->target == set) {
            u32 idx = it - v->data;
            fn_710016b34c(v, idx);
            it = v->data + idx;
        } else {
            ++it;
        }
    }
}

// ---- Adding a target under an id ----

struct UnkKeyed_ac04 {
    u64 key;
};

struct UnkTargetState_ac04 {
    u8 pad_0x0[0xa9];
    u8 field_0xa9;
    u8 pad_0xaa[3];
    s32 field_0xac;
};

struct UnkTargetRef_ac04 {
    u8 pad_0x0[8];
    UnkTargetState_ac04* state;  // 0x8
    UnkKeyed_ac04* keyed;        // 0x10
    u8 pad_0x18[8];
};
static_assert(sizeof(UnkTargetRef_ac04) == 0x20, "layout");

struct UnkTarget_ac04 {
    u8 pad_0x0[0x34];
    u32 ref_count;            // 0x34
    UnkTargetRef_ac04* refs;  // 0x38
};

void fn_710016ac04(UnkController_u207* self, UnkTarget_ac04* target, const CStrId* id) {
    if (self->model->field_0xa9 && self->model->field_0xb0) {
        u32 ref_count = target->ref_count;
        for (u32 i = 0; i < ref_count; ++i) {
            UnkTargetRef_ac04* ref = &target->refs[i];
            if (ref->keyed->key == *reinterpret_cast<const u64*>(id)) {
                // Char stores: the original reloads the reference array and the id after them.
                char* state = reinterpret_cast<char*>(ref->state);
                if (state[0xa9])
                    state[0xa9] = 0;
                *reinterpret_cast<s32*>(state + 0xac) = 2;
            }
        }
    }
    CStrId tmp(*id);
    auto* v = &self->entries;
    u32 n = v->count;
    u32 cap = v->capacity;
    u32 grown = cap;
    if (cap <= n && v->step != 0)
        grown = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)grown)
        for (;;) {
        }
    fn_710016b21c(v, grown);
    UnkEntry_u207* slot = v->data;
    n = v->count;
    // the element is stored bitwise and its id then retained, as the copy constructor does
    __builtin_memcpy(&slot[n].target, &target, sizeof(target));
    __builtin_memcpy(&slot[n].id, &tmp, sizeof(tmp));
    if (&g_unk_7101d47108->empty != tmp.data)
        ++tmp.data->ref_count;
    ++v->count;
}

// ---- Adding a target under the empty path ----

struct UnkTargetState_aa9c {
    u8 pad_0x0[0xa9];
    u8 field_0xa9;
};

struct UnkTarget_aa9c {
    u8 pad_0x0[0x2c];
    u32 count;  // 0x2c
    u8 pad_0x30[0x18];
    UnkTargetState_aa9c** states;  // 0x48
};

// A local entry whose id is released in two steps when it goes out of scope.
struct UnkLocalEntry_aa9c {
    UnkTarget_aa9c* target;
    union {
        CStrId id;
    };
    UnkLocalEntry_aa9c(UnkTarget_aa9c* t, const char* name) : target(t), id(name, true) {}
    ~UnkLocalEntry_aa9c() { id.Reset(); }
};

void fn_710016aa9c(UnkController_u207* self, UnkTarget_aa9c* target) {
    if (self->model->field_0xa9 && self->model->field_0xb0) {
        u32 count = target->count;
        for (u32 i = 0; i < count; ++i) {
            char* state = reinterpret_cast<char*>(target->states[i]);
            if (state[0xa9])
                state[0xa9] = 0;
            *reinterpret_cast<s32*>(state + 0xac) = 2;
        }
    }
    UnkLocalEntry_aa9c entry(target, g_unk_710162e2ed);
    auto* v = &self->entries;
    u32 n = v->count;
    u32 cap = v->capacity;
    u32 grown = cap;
    if (cap <= n && v->step != 0)
        grown = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)grown)
        for (;;) {
        }
    fn_710016b21c(v, grown);
    UnkEntry_u207* slot = v->data;
    n = v->count;
    __builtin_memcpy(&slot[n].target, &entry.target, sizeof(entry.target));
    base::global::UnkStringData* data = entry.id.data;
    __builtin_memcpy(&slot[n].id, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
    ++v->count;
}

// ---- Swapping the tinted object ----

struct UnkColorSlot_a8dc {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkTinted_a8dc {
    u8 pad_0x0[0x14c];
    CVector4D tint_a;  // 0x14c
    u8 pad_0x15c[0x1f0 - 0x15c];
    UnkColorSlot_a8dc* slot_a;  // 0x1f0
    u8 pad_0x1f8[0x240 - 0x1f8];
    CVector4D tint_b;  // 0x240
    u8 pad_0x250[0x2e0 - 0x250];
    UnkColorSlot_a8dc* slot_b;  // 0x2e0
};

struct UnkTintOwner_a8dc {
    u8 pad_0x0[8];
    UnkTinted_a8dc* target;  // 0x8
    u8 pad_0x10[0x28];
    CVector4D saved_a;  // 0x38
    CVector4D saved_b;  // 0x48
};

void fn_710016a8dc(UnkTintOwner_a8dc* self, UnkTinted_a8dc* next) {
    if (self->target) {
        UnkVector4D saved;
        UnkVector4D current;
        current.x = self->target->tint_a.x;
        current.y = self->target->tint_a.y;
        current.z = self->target->tint_a.z;
        current.w = self->target->tint_a.w;
        fn_710007b518(&saved, &current);
        fn_710007b518(&self->saved_a, &saved);
        UnkTinted_a8dc* t = self->target;
        fn_710007b518(&t->tint_a, &self->saved_a);
        if (t->slot_a)
            *t->slot_a->value = self->saved_a;
        t = self->target;
        current.x = t->tint_b.x;
        current.y = t->tint_b.y;
        current.z = t->tint_b.z;
        current.w = t->tint_b.w;
        fn_710007b518(&self->saved_b, &current);
        t = self->target;
        fn_710007b518(&t->tint_b, &self->saved_b);
        if (t->slot_b)
            *t->slot_b->value = self->saved_b;
    }
    self->target = next;
}
