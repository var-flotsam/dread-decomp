// CGooplotAIComponent (tools/tu_map.py unit 851): takes the last point off the AI's point list and
// returns it; with no points left, the up axis of the owner's transform. Names and layouts are
// placeholders.
#include "base/UnkLinkedList.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGooplotPoint_l59b7ec {
    UnkVec3f value;
    u8 pad_0xc[0x4];
    UnkGooplotPoint_l59b7ec* next;  // 0x10
};

struct UnkGooplotPointList_l59b7ec : base::UnkLinkedList<UnkGooplotPoint_l59b7ec> {
    // Unlinks and frees the node at `index`; nothing happens past the end.
    void RemoveAt(u32 index) {
        UnkGooplotPoint_l59b7ec* prev = nullptr;
        u32 i = 0;
        for (UnkGooplotPoint_l59b7ec* node = head; node;) {
            UnkGooplotPoint_l59b7ec* next = node->next;
            if (i == index) {
                if (prev) {
                    prev->next = next;
                    if (tail == node)
                        tail = prev;
                } else {
                    head = next;
                }
                if (--count == 0) {
                    head = nullptr;
                    tail = nullptr;
                }
                allocator->vf_0x48(node);
                return;
            }
            prev = node;
            node = next;
            ++i;
        }
    }
};

struct UnkGooplotOwner_l59b7ec {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkGooplotAI_l59b7ec {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_l59b7ec* owner;  // 0x30
    u8 pad_0x38[0x147d0];
    UnkGooplotPointList_l59b7ec points;  // 0x14808
};
static_assert(__builtin_offsetof(UnkGooplotAI_l59b7ec, points) == 0x14808, "points");

UnkVec3f fn_710059b7ec(UnkGooplotAI_l59b7ec* self) {
    UnkGooplotPointList_l59b7ec& points = self->points;
    if (points.count == 0) {
        const UnkMatrix44& m = self->owner->xform.GetMatrix();
        return UnkVec3f(m.m[1], m.m[5], m.m[9]);
    }
    UnkVec3f point = points.tail->value;
    points.RemoveAt(points.count - 1);
    return point;
}
