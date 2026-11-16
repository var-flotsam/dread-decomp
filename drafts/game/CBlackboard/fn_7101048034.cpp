// CBlackboard's unit: removes every section whose id is not on the list `keep`. The ids to remove
// are gathered on a list of their own first, then each section is removed through vf_0x30.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "types.h"

using base::global::CStrId;

struct UnkStrIdNode_l048034 {
    CStrId id;
    UnkStrIdNode_l048034* next;
};

// A list of string ids after a word of its own.
struct UnkStrIdList_l048034 {
    u64 field_0x0;
    base::UnkLinkedList<UnkStrIdNode_l048034> list;

    bool Contains(const CStrId& id) const {
        for (UnkStrIdNode_l048034* node = list.head; node; node = node->next)
            if (node->id == id)
                return true;
        return false;
    }
};

// A local list of string ids (nodes from the default allocator, flags 0x13); destroying it
// releases the ids and frees the nodes.
struct UnkStrIdLocalList_l048034 : base::UnkLinkedList<UnkStrIdNode_l048034> {
    UnkStrIdLocalList_l048034() {
        allocator = fn_7100083990();
        flags = 0x13;
        head = nullptr;
        tail = nullptr;
        count = 0;
    }
    ~UnkStrIdLocalList_l048034() {
        for (UnkStrIdNode_l048034* node = head; node;) {
            UnkStrIdNode_l048034* next = node->next;
            node->id.Reset();
            allocator->vf_0x48(node);
            node = next;
        }
    }

    void PushBack(const CStrId& id) {
        UnkStrIdNode_l048034* node =
            static_cast<UnkStrIdNode_l048034*>(allocator->vf_0x40(sizeof(UnkStrIdNode_l048034), 8, flags));
        if (!node)
            return;
        new (&node->id) CStrId(id);
        node->next = nullptr;
        PushBackTailFirst(node);
    }
};

// The blackboard with the virtual function after vf_0x28: removes the section in `slot`.
struct UnkBlackboardRemove_l048034 : CBlackboard {
    virtual void vf_0x30(CSection** slot);
};

void fn_7101048034(CBlackboard* self, const UnkStrIdList_l048034* keep) {
    UnkStrIdLocalList_l048034 removed;
    for (CBlackboard::CSection** it = self->sections; &self->sections[self->count] != it; ++it) {
        CStrId id = self->ids[u32(it - self->sections)];
        if (!keep->Contains(id))
            removed.PushBack(id);
    }
    for (UnkStrIdNode_l048034* node = removed.head; node; node = node->next)
        static_cast<UnkBlackboardRemove_l048034*>(self)->vf_0x30(UnkBlackboardFindSlot(self, node->id));
}
