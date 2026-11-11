// Unit 2702 (hint CAIComponent): an AI component's list of path segments (a key, a start and an end point,
// their length and direction). Names and layouts are placeholders.
#include "base/UnkIdKindKey.h"
#include "base/UnkVector.h"
#include "types.h"

float sqrtf(float);

struct UnkPathSeg_ovb0146 {
    UnkIdKindKey key;
    f32 start[2];  // 0x4
    f32 end[2];    // 0xc
    f32 length;    // 0x14
    f32 dir[2];    // 0x18
};

struct UnkPathNode_ovb0146 : UnkPathSeg_ovb0146 {
    UnkPathNode_ovb0146* next;  // 0x20
};
static_assert(sizeof(UnkPathNode_ovb0146) == 0x28, "UnkPathNode_ovb0146 layout");

struct UnkPathList_ovb0146 {
    u8 pad_0x0[8];
    u32 count;  // 0x8
    UnkPathNode_ovb0146* head;  // 0x10
    UnkPathNode_ovb0146* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;  // 0x28
};

struct UnkPathShape_ovb0146 {
    u8 pad_0x0[0x23c];
    f32 step;  // 0x23c
};

struct UnkPathAI_ovb0146 {
    u8 pad_0x0[0x1520];
    UnkPathList_ovb0146 list;  // 0x1520
    u8 pad_0x1550[0x1610 - 0x1550];
    UnkPathShape_ovb0146* shape;  // 0x1610
};
static_assert(__builtin_offsetof(UnkPathAI_ovb0146, shape) == 0x1610, "UnkPathAI_ovb0146 layout");

void fn_7101110330(UnkPathList_ovb0146* list, f32 step);

static inline void SetEnd(UnkPathNode_ovb0146* n, const f32* p) {
    n->end[0] = p[0];
    n->end[1] = p[1];
    f32 dx = p[0] - n->start[0];
    f32 dy = p[1] - n->start[1];
    n->dir[0] = dx;
    n->dir[1] = dy;
    f32 len = sqrtf(dx * dx + dy * dy);
    n->length = len;
    n->dir[0] = n->dir[0] / len;
    n->dir[1] = n->dir[1] / len;
}

void fn_71011104c0(UnkPathAI_ovb0146* self, UnkIdKindKey* key, f32* p) {
    if (!(self->shape->step > 0.0f))
        return;
    UnkPathNode_ovb0146* head = self->list.head;
    if (head && head->key.id == key->id && head->key.kind == key->kind) {
        SetEnd(head, p);
        fn_7101110330(&self->list, self->shape->step);
        return;
    }
    UnkPathSeg_ovb0146 node = {*key, {p[0], p[1]}, {p[0], p[1]}, 0.0f, {0.0f, 0.0f}};
    if (node.key.id != 0xffff && node.key.kind != 0xff) {
        if (head && head->key.id != 0xffff && head->key.kind != 0xff)
            SetEnd(head, p);
        UnkPathNode_ovb0146* n = static_cast<UnkPathNode_ovb0146*>(
            self->list.allocator->vf_0x40(sizeof(UnkPathNode_ovb0146), 8, self->list.flags));
        if (n) {
            n->key = node.key;
            n->start[0] = node.start[0];
            n->start[1] = node.start[1];
            n->end[0] = node.end[0];
            n->end[1] = node.end[1];
            n->length = node.length;
            n->dir[0] = node.dir[0];
            n->dir[1] = node.dir[1];
            __builtin_memset(&n->next, 0, sizeof(n->next));
            u32 c;
            if (self->list.count) {
                UnkPathNode_ovb0146* old = self->list.head;
                __builtin_memcpy(&n->next, &old, sizeof(old));
                self->list.head = n;
                c = self->list.count + 1;
            } else {
                self->list.head = n;
                self->list.tail = n;
                c = 1;
            }
            self->list.count = c;
        }
        fn_7101110330(&self->list, self->shape->step);
    }
}
