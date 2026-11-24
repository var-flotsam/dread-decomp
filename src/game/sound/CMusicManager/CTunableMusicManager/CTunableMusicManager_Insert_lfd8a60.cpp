// sound::CMusicManager::CTunableMusicManager's unit: adds a copy of a request to a slot's list, kept in
// order: before the first node it does not come after, by the list's comparer (or by the first word
// when the list has none). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkPooledId.h"
#include "game/UnkMusicRequest_b0019.h"
#include "game/UnkNodeList_b0019.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

// A request as the list keeps it (0x60 bytes: as fn_7100fddcb4's entry).
struct UnkMusicRequestEntry_d8a60 {
    s32 priority;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u8 field_0x24;
    u8 field_0x25;
    u8 field_0x26;
    UnkMusicPathList field_0x28;
    u64 field_0x50;
    UnkPooledId<&g_unk_7101d47108> field_0x58;

    UnkMusicRequestEntry_d8a60(const UnkMusicRequestEntry_d8a60& o)
        : priority(o.priority), field_0x4(o.field_0x4), field_0x8(o.field_0x8), field_0xc(o.field_0xc),
          field_0x10(o.field_0x10), field_0x14(o.field_0x14), field_0x18(o.field_0x18),
          field_0x1c(o.field_0x1c), field_0x20(o.field_0x20), field_0x24(o.field_0x24),
          field_0x25(o.field_0x25), field_0x26(o.field_0x26), field_0x28(o.field_0x28),
          field_0x50(o.field_0x50), field_0x58(o.field_0x58) {}
};
static_assert(sizeof(UnkMusicRequestEntry_d8a60) == 0x60, "UnkMusicRequestEntry_d8a60 layout");

struct UnkMusicRequestNode_d8a60 {
    UnkMusicRequestEntry_d8a60 value;
    UnkMusicRequestNode_d8a60* next;  // 0x60
    UnkMusicRequestNode_d8a60* prev;  // 0x68

    explicit UnkMusicRequestNode_d8a60(const UnkMusicRequestEntry_d8a60& v) : value(v), next(nullptr), prev(nullptr) {}

    UnkMusicRequestNode_d8a60*& Next() { return next; }
    UnkMusicRequestNode_d8a60*& Prev() { return prev; }
};

// Orders two requests: true when the first goes before the second.
struct UnkMusicRequestCompare_d8a60 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual bool vf_0x10(UnkMusicRequestEntry_d8a60 a, UnkMusicRequestEntry_d8a60 b);
};

struct UnkMusicRequestList_d8a60 {
    char* vtable;
    u32 count;  // 0x8
    UnkMusicRequestNode_d8a60* head;  // 0x10
    UnkMusicRequestNode_d8a60* tail;  // 0x18
    u32 flags;                        // 0x20
    base::IAllocator* allocator;    // 0x28
    UnkMusicRequestCompare_d8a60* compare;  // 0x30

};

void CTunableMusicManager_InsertRequestSorted(UnkNodeList_b0019* list, UnkMusicRequest_b0019* request) {
    auto* self = reinterpret_cast<UnkMusicRequestList_d8a60*>(list);
    const auto& value = *reinterpret_cast<const UnkMusicRequestEntry_d8a60*>(request);
    auto* node = static_cast<UnkMusicRequestNode_d8a60*>(
        self->allocator->vf_0x40(sizeof(UnkMusicRequestNode_d8a60), 8, self->flags));
    if (node)
        new (node) UnkMusicRequestNode_d8a60(value);
    if (self->count == 0) {
        self->head = node;
        self->tail = node;
        node->next = nullptr;
        node->prev = nullptr;
    } else {
        for (UnkMusicRequestNode_d8a60* cur = self->head; cur; cur = cur->next) {
            // Does the node go after this one?
            bool after = (self->compare && self->compare->vf_0x10(cur->value, node->value)) ||
                         (!self->compare && cur->value.priority < node->value.priority);
            if (after) {
                if (cur == self->tail) {
                    cur->next = node;
                    node->next = nullptr;
                    node->prev = cur;
                    self->tail = node;
                    break;
                }
                continue;
            }
            if (cur == self->head) {
                self->head = node;
                node->prev = nullptr;
            }
            if (cur->Prev()) {
                cur->Prev()->Next() = node;
                node->Prev() = cur->Prev();
            }
            node->next = cur;
            cur->prev = node;
            break;
        }
    }
    ++self->count;
}
