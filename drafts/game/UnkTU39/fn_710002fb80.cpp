// Unit 39 (no reliable name): a list of string ids, each with a timer, counted down by a time step;
// an entry whose timer runs out is unlinked, its id released and its node freed, and the callbacks
// at +0x30 are told when the list turns empty or not. Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;

struct UnkTimedStrIdNode_l02fb80 {
    base::global::CStrId id;
    f32 time;  // 0x8; a negative time never runs out
    UnkTimedStrIdNode_l02fb80* next;  // 0x10
};

struct UnkTimedStrIdList_l02fb80 {
    u64 field_0x0;
    u32 count;
    UnkTimedStrIdNode_l02fb80* head;
    UnkTimedStrIdNode_l02fb80* tail;
    base::UnkAllocator* allocator;
    u64 field_0x28;
    UnkDeferredSet<UnkDeferredCallback<bool>> on_change;  // 0x30
};

void fn_710002fb80(void* list, f32 dt) {
    UnkTimedStrIdList_l02fb80* self = static_cast<UnkTimedStrIdList_l02fb80*>(list);
    u32 before = self->count;
    UnkTimedStrIdNode_l02fb80* node = self->head;
    while (node) {
        if (!(node->time >= 0)) {
            node = node->next;
            continue;
        }
        node->time -= dt;
        if (!(node->time <= 0)) {
            node = node->next;
            continue;
        }
        UnkTimedStrIdNode_l02fb80* prev = nullptr;
        UnkTimedStrIdNode_l02fb80* cur = self->head;
        UnkTimedStrIdNode_l02fb80* next;
        UnkTimedStrIdNode_l02fb80* last = nullptr;
        for (;;) {
            if (!cur)
                goto done;
            prev = last;
            next = cur->next;
            if (cur == node)
                break;
            last = cur;
            cur = next;
        }
        if (prev) {
            prev->next = next;
            if (self->tail == node)
                self->tail = prev;
        } else {
            self->head = next;
        }
        if (--self->count == 0) {
            self->head = nullptr;
            self->tail = nullptr;
        }
        node->id.Reset();
        self->allocator->vf_0x48(node);
        node = next;
    }
done:
    bool now = self->count != 0;
    if ((before != 0) != now)
        fn_71000303c0(&self->on_change, &now);
}
