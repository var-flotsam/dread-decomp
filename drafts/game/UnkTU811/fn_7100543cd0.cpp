// UnkTU811 (CEmmyAIComponent): samples a position at a fixed interval into a bounded history, keeps the
// mean of the history and its spread (the largest squared distance from the mean), and times how long the
// spread stays under two thresholds. Names and layouts are placeholders.
#include "base/UnkLinkedList.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPointNode_l543cd0 {
    f32 x;
    f32 y;
    UnkPointNode_l543cd0* next;
};

struct UnkValueNode_l543cd0 {
    f32 value;
    UnkValueNode_l543cd0* next;
};

// A bounded history: its capacity and the list of samples.
template <class Node>
struct UnkHistory_l543cd0 {
    u32 capacity;
    u8 pad_0x4[0xc];
    base::UnkLinkedList<Node> list;  // 0x10
    u8 pad_0x38[0x38];

    Node* NewNode() { return static_cast<Node*>(list.allocator->vf_0x40(sizeof(Node), 8, list.flags)); }

    void PopFront() {
        if (Node* node = list.head) {
            list.head = node->next;
            list.allocator->vf_0x48(node);
            if (--list.count == 0)
                list.tail = list.head;
        }
    }

    Node* At(u32 index) const {
        Node* node = list.head;
        for (u32 i = 0; i < index; ++i)
            node = node->next;
        return node;
    }
};

struct UnkEmmyTracker_l543cd0 {
    UnkHistory_l543cd0<UnkPointNode_l543cd0> positions;  // 0x0
    UnkHistory_l543cd0<UnkValueNode_l543cd0> spreads;    // 0x70
    UnkHistory_l543cd0<UnkPointNode_l543cd0> means;      // 0xe0
    u8 pad_0x150[0x10];
    f32 field_0x160;
    f32 field_0x164;
    f32 interval;  // 0x168
    f32 timer;     // 0x16c
    f32 field_0x170;
    f32 field_0x174;

    // The largest squared distance of a sample from the last mean; huge while there is no mean.
    f32 Spread() const {
        if (means.list.count == 0)
            return 1.7446744e19f;
        f32 spread = 0.0f;
        for (u32 i = 0; i < positions.list.count; ++i) {
            const UnkPointNode_l543cd0* p = positions.At(i);
            const UnkPointNode_l543cd0* mean = means.list.tail;
            f32 dx = p->x - mean->x;
            f32 dy = p->y - mean->y;
            f32 d = dx * dx + dy * dy;
            spread = d > spread ? d : spread;
        }
        return spread;
    }
};
static_assert(__builtin_offsetof(UnkEmmyTracker_l543cd0, field_0x160) == 0x160, "layout");

void fn_7100543cd0(UnkEmmyTracker_l543cd0* self, const CVector2D* pos, f32 dt) {
    self->timer += dt;
    if (self->interval < self->timer) {
        u32 count = self->positions.list.count;
        self->timer = 0.0f;
        if (count < self->positions.capacity) {
            ++count;
        } else {
            self->positions.PopFront();
            self->spreads.PopFront();
            self->means.PopFront();
        }

        if (UnkPointNode_l543cd0* node = self->positions.NewNode()) {
            node->x = pos->x;
            node->y = pos->y;
            node->next = nullptr;
            self->positions.list.PushBack(node);
        }

        f32 sum_x = 0.0f;
        f32 sum_y = 0.0f;
        for (u32 i = 0; i < count; ++i) {
            const UnkPointNode_l543cd0* p = self->positions.At(i);
            sum_x += p->x;
            sum_y += p->y;
        }
        if (UnkPointNode_l543cd0* node = self->means.NewNode()) {
            node->x = sum_x / count;
            node->y = sum_y / count;
            node->next = nullptr;
            self->means.list.PushBack(node);
        }

        f32 spread = self->Spread();
        if (UnkValueNode_l543cd0* node = self->spreads.NewNode()) {
            node->value = spread;
            node->next = nullptr;
            self->spreads.list.PushBack(node);
        }
    }

    f32 spread = self->Spread();
    if (spread < self->field_0x164) {
        self->field_0x174 += dt;
        if (spread < self->field_0x160)
            self->field_0x170 += dt;
        // The original adds and then clears it.
        self->field_0x170 = 0.0f;
    } else {
        self->field_0x170 = 0.0f;
        self->field_0x174 = 0.0f;
    }
}
