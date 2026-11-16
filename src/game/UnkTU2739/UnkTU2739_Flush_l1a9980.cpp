// UnkTU2739 (unit 2739): drains the pending list at +0x180, stopping each entry and, when it has
// reached its final state, releasing it and erasing its resource's path from the map at +0x38.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "game/CFXPool/CDefinition.h"
#include "game/UnkTU2739.h"
#include "types.h"

using FileKey = base::UnkStrIdMemberOf<g_unk_7101d47200>;

struct UnkFileHashMap;
struct UnkOwner_710104de9c;
struct UnkHandler_71011a91c4;

char* fn_710104de9c(UnkOwner_710104de9c* self);
void* fn_71011aa6a0(UnkFileHashMap* m, const FileKey* key);

struct UnkPending_l1a9980 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual u32 vf_0x20();
    u8 pad_0x8[0x10];
    u32 state;  // 0x18
};

struct UnkNode_l1a9980 {
    UnkPending_l1a9980* value;
    UnkNode_l1a9980* next;
};

struct UnkOwner_l1a9980 {
    u8 pad_0x0[0x38];
    u8 map[0x148];  // 0x38
    u32 count;      // 0x180
    UnkNode_l1a9980* head;
    UnkNode_l1a9980* tail;
    base::UnkAllocator* allocator;
};

void fn_71011a9980(UnkQueueOwner* owner) {
    UnkOwner_l1a9980* self = reinterpret_cast<UnkOwner_l1a9980*>(owner);
    UnkNode_l1a9980* node = self->head;
    while (node) {
        UnkPending_l1a9980* entry = node->value;
        const FileKey* path =
            reinterpret_cast<const FileKey*>(fn_710104de9c(reinterpret_cast<UnkOwner_710104de9c*>(entry)));
        if (entry->vf_0x20())
            entry->vf_0x18();
        u32 state = fn_710104de7c(reinterpret_cast<UnkHandler_71011a91c4*>(entry));
        u32 final_state = entry->state;
        u32 active = entry->vf_0x20();
        if (state == final_state && !active) {
            entry->vf_0x8();
            fn_71011aa6a0(reinterpret_cast<UnkFileHashMap*>(self->map), path);
        }
        UnkNode_l1a9980* prev = nullptr;
        UnkNode_l1a9980* cur = self->head;
        UnkNode_l1a9980* next;
        UnkNode_l1a9980* last = nullptr;
        for (;;) {
            if (!cur)
                return;
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
        self->allocator->vf_0x48(node);
        node = next;
    }
}
