// CAlternativeActionPlayerComponent's unit (tools/tu_map.py): appends an entry: the string id goes to
// the id array (+0x50) and (a, b, a, b) to the parallel 16-byte array (+0x48), both grown together by
// fn_710071263c when full (count +0x58, capacity +0x5c, step +0x60). Then hooks the link at +0x70
// (handle at +0x88) up to the object the owner (+0x30) tracks (+0x220), as
// src/game/CObsydomithonAttack/CObsydomithonAttack_Start_A3_b73.cpp, and on first use registers the
// entry at +0x90 in the deferred set at +0x118 of the owner's +0x1b8. Names and layouts are
// placeholders. The id's pointer is copied with memcpy and the 16-byte entries are a union (both
// may alias the count, which the original reloads after each store).
#include "base/UnkDeferredSet.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkClone_fn_7100712798;
using UnkAltActionSet_A3_b74 = base::UnkDeferredSet<UnkClone_fn_7100712798>;
extern template void UnkAltActionSet_A3_b74::Add(UnkClone_fn_7100712798*);

struct UnkAltActionOwner_A3_b74 {
    u8 pad_0x0[0x1b8];
    char* field_0x1b8;
    u8 pad_0x1c0[0x60];
    UnkWeakTarget* field_0x220;
};

struct UnkAltActionRect_A3_b74 {
    union {
        f32 v[4];
    };
};

struct UnkAltActionArrays_A3_b74 {
    UnkAltActionRect_A3_b74* rects;  // 0x48
    base::global::CStrId* ids;       // 0x50
    u32 count;                       // 0x58
    u32 capacity;                    // 0x5c
    u32 step;                        // 0x60
};

void fn_710071263c(UnkAltActionArrays_A3_b74* arrays, u32 capacity);

struct UnkAltActionLink_A3_b74 {
    u8 pad_0x0[0x18];
};

struct UnkAltAction_A3_b74 {
    u8 pad_0x0[0x30];
    UnkAltActionOwner_A3_b74* owner;  // 0x30
    u8 pad_0x38[0x10];
    UnkAltActionArrays_A3_b74 arrays;  // 0x48
    u8 pad_0x68[0x8];
    UnkAltActionLink_A3_b74 link;  // 0x70
    UnkWeakHandle* handle;         // 0x88
    UnkAltActionSet_A3_b74* set;   // 0x90
};

void fn_7100711e20(UnkAltAction_A3_b74* self, const base::global::CStrId* id, f32 a, f32 b) {
    UnkAltActionArrays_A3_b74* arrays = &self->arrays;
    if (arrays->count == arrays->capacity)
        fn_710071263c(arrays, arrays->step + arrays->count);
    base::global::CStrId* dst = &arrays->ids[arrays->count];
    base::global::UnkStringData* data = id->data;
    __builtin_memcpy(dst, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
    UnkAltActionLink_A3_b74* link = &self->link;
    UnkAltActionRect_A3_b74* r = &arrays->rects[arrays->count];
    r->v[0] = a;
    r->v[1] = b;
    r->v[2] = a;
    r->v[3] = b;
    ++arrays->count;
    UnkWeakTarget* track = self->owner->field_0x220;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    asm("" : "+r"(self));
    if (track) {
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    if (!self->set) {
        if (char* p = self->owner->field_0x1b8) {
            self->set = reinterpret_cast<UnkAltActionSet_A3_b74*>(p + 0x118);
            self->set->Add(reinterpret_cast<UnkClone_fn_7100712798*>(&self->set));
        }
    }
}
