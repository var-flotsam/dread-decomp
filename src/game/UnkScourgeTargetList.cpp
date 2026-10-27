// A list of candidate targets kept as weak handles (translation unit 1038 of data/tu_map.csv, next
// to CScourgeLifeComponent's unit; no reflected class of its own). fn_71006a1ef0 picks, from the
// candidates whose CScourgeAIComponent ranks highest, the one the object follows (unless its
// current one is still held), then empties the list.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The candidate's CScourgeAIComponent: its rank at 0x1124.
struct UnkScourgeAIComponent : base::reflection::UnkReflObject {
    u8 unk_0x8[0x111c];
    f32 rank;
};

// What a weak handle refers to, as far as this unit reads it.
struct UnkScourgeTarget {
    u8 unk_0x0[0x240];
    UnkScourgeAIComponent* ai;
    u8 unk_0x248[0x30];
    void* field_0x278;  // fn_7101136308 says whether it holds the target
};

struct UnkWeakHandleNode {
    UnkWeakHandle* handle;
    UnkWeakHandleNode* next;
};

struct UnkScourgeTargetList {
    u8 unk_0x0[0x2d0];
    UnkWeakHandle* current;
    u8 unk_0x2d8[0x8];
    u32 count;
    UnkWeakHandleNode* head;
    UnkWeakHandleNode* tail;
    base::UnkAllocator* allocator;
};

bool fn_7101136308(void* object);
UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

static inline UnkScourgeTarget* TargetOf(UnkWeakHandle* handle) {
    return reinterpret_cast<UnkScourgeTarget*>(handle->target);
}

void fn_71006a1ef0(UnkScourgeTargetList* self) {
    UnkWeakHandle* current = self->current;
    if (!(current && current->target && TargetOf(current)->field_0x278 &&
          fn_7101136308(TargetOf(current)->field_0x278))) {
        f32 best = -3.4028235e+38f;
        for (UnkWeakHandleNode* node = self->head; node; node = node->next) {
            UnkWeakHandle* handle = node->handle;
            if (!handle)
                continue;
            ++handle->ref_count;
            if (handle->target) {
                UnkScourgeAIComponent* ai = TargetOf(handle)->ai;
                if (ai && fn_710009d50c(ai->vf_0x0(), fn_710069ef44()) && ai->rank > best) {
                    f32 rank = ai->rank;
                    AssignRef(&self->current, fn_7100d78378(handle->target));
                    best = rank;
                }
            }
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    for (UnkWeakHandleNode* node = self->head; node;) {
        UnkWeakHandleNode* next = node->next;
        if (UnkWeakHandle* handle = node->handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
        self->allocator->vf_0x48(node);
        node = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}
