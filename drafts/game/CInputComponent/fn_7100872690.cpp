// CInputComponent (tools/tu_map.py unit 1326): feeds a list of input values to the block at +0xad0 for a
// positive time step, first resetting the component and clearing its held state when asked.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/CInputComponent.h"
#include "types.h"

// A singly linked list of u32 values whose nodes come from an allocator.
struct UnkInputValueList_l872690 {
    struct Node {
        u32 value;
        Node* next;
    };
    void* vtable;
    u32 count;
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkInputValueList_l872690(const UnkInputValueList_l872690& other)
        : vtable(g_unk_7101a46040 + 2), count(0), head(nullptr), tail(nullptr), allocator(other.allocator),
          flags(other.flags) {
        for (Node* it = other.head; it; it = it->next)
            PushBack(it->value);
    }

    ~UnkInputValueList_l872690() {
        vtable = g_unk_7101a46040 + 2;
        for (Node* n = head; n;) {
            Node* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }

    void PushBack(const u32& v) {
        Node* n = static_cast<Node*>(allocator->vf_0x40(sizeof(Node), 8, flags));
        if (!n)
            return;
        n->value = v;
        n->next = nullptr;
        if (count == 0) {
            head = tail = n;
            count = 1;
        } else {
            tail->next = n;
            n->next = nullptr;
            tail = n;
            ++count;
        }
    }
};

struct UnkInputTarget_l872690 {
    u8 pad_0x0[0x3c];
    u32 field_0x3c;
};

struct UnkInputTargetNode_l872690 {
    UnkInputTarget_l872690* target;
    UnkInputTargetNode_l872690* next;
};

struct UnkInputComponent_l872690 {
    u8 pad_0x0[0x9e0];
    u64 field_0x9e0;
    u32 field_0x9e8;
    u8 pad_0x9ec[0xab0 - 0x9ec];
    UnkInputTargetNode_l872690* targets;  // 0xab0
    u8 pad_0xab8[0xad0 - 0xab8];
    u8 field_0xad0[0x34];
    bool held[5];  // 0xb04

    bool AnyHeld() const { return held[0] || held[4] || held[3] || held[1] || held[2]; }
    void ClearHeld() {
        held[0] = false;
        held[1] = false;
        held[2] = false;
        held[3] = false;
        held[4] = false;
    }
};
static_assert(__builtin_offsetof(UnkInputComponent_l872690, held) == 0xb04, "UnkInputComponent_l872690 layout");

void fn_710086fef0(void* block, void* arg, UnkInputValueList_l872690 values, bool b, bool a, f32 dt);

void fn_7100872690(UnkInputComponent_l872690* self, void* arg, const UnkInputValueList_l872690* values, bool a,
                   bool b, bool reset, bool clear, f32 dt) {
    if (!(dt > 0.0f))
        return;
    if (values->count != 0 || a || b) {
        if (reset)
            fn_7100872268(reinterpret_cast<UnkInputComponent*>(self));
        if (clear) {
            self->field_0x9e8 = 0;
            self->field_0x9e0 = 0;
            for (UnkInputTargetNode_l872690* n = self->targets; n; n = n->next)
                n->target->field_0x3c = 0;
            if (self->AnyHeld())
                self->ClearHeld();
        }
    }

    fn_710086fef0(self->field_0xad0, arg, *values, b, a, dt);
}
