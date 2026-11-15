// Unit 2702 (CAIComponent): a queue of timed entries (a singly linked list with head, tail and count)
// that is cut down from its tail until the total duration fits a limit: whole entries are removed,
// the last one left is shortened. Names and layouts are placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "base/global/CRntVector.h"
#include "types.h"

// What an entry holds (the part that is copied out); the destructor is the empty out-of-line one.
struct UnkTimedElem_l110330 {
    u32 field_0x0;
    f32 pos_x;  // 0x4
    f32 pos_y;  // 0x8
    u32 field_0xc;
    u32 field_0x10;
    f32 duration;  // 0x14
    f32 step_x;    // 0x18
    f32 step_y;    // 0x1c
    UnkTimedElem_l110330(const UnkTimedElem_l110330& other)
        : field_0x0(other.field_0x0),
          pos_x(other.pos_x),
          pos_y(other.pos_y),
          field_0xc(other.field_0xc),
          field_0x10(other.field_0x10),
          duration(other.duration),
          step_x(other.step_x),
          step_y(other.step_y) {}
    ~UnkTimedElem_l110330() { fn_71002874dc(this); }
};

struct UnkTimedNode_l110330 {
    s16 id;  // 0x0, -1 for none
    u8 slot;  // 0x2, 0xff for none
    u8 pad_0x3;
    f32 pos_x;  // 0x4
    f32 pos_y;  // 0x8
    u8 pad_0xc[8];
    f32 duration;  // 0x14
    f32 step_x;    // 0x18
    f32 step_y;    // 0x1c
    UnkTimedNode_l110330* next;  // 0x20
};

struct UnkTimedQueue_l110330 {
    u8 pad_0x0[8];
    u32 count;  // 0x8
    u8 pad_0xc[4];
    UnkTimedNode_l110330* head;  // 0x10
    UnkTimedNode_l110330* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
};

void fn_7101110330(UnkTimedQueue_l110330* self, f32 limit) {
    f32 total = 0.0f;
    for (UnkTimedNode_l110330* node = self->head; node; node = node->next) {
        UnkTimedElem_l110330 elem = *reinterpret_cast<UnkTimedElem_l110330*>(node);
        total += elem.duration;
    }
    while (UnkTimedNode_l110330* node = self->tail) {
        f32 excess = total - limit;
        if (!(excess > 0.0f))
            break;
        if (node->id != -1 && node->slot != 0xff && excess < node->duration) {
            node->pos_x += excess * node->step_x;
            node->pos_y += excess * node->step_y;
            node->duration -= excess;
            break;
        }
        total -= node->duration;
        UnkTimedNode_l110330* prev = nullptr;
        UnkTimedNode_l110330* next = nullptr;
        UnkTimedNode_l110330* cur = self->head;
        while (cur) {
            UnkTimedNode_l110330* after = cur->next;
            if (cur == node) {
                next = after;
                break;
            }
            prev = cur;
            cur = after;
        }
        if (!cur)
            continue;
        if (!prev) {
            self->head = next;
        } else {
            prev->next = next;
            if (self->tail == node)
                self->tail = prev;
        }
        if (--self->count == 0) {
            self->head = nullptr;
            self->tail = nullptr;
        }
        fn_71002874dc(node);
        self->allocator->vf_0x48(node);
    }
}
