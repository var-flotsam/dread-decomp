// CEmmyAIComponent's unit (tools/tu_map.py unit 811, no reliable name): steps the AI state 2. Pushes the
// target (+0x4) onto one of two stacks and refreshes via fn_7100560b98; when the second stack takes it,
// asks the source for items (fn_7100560d14) and handles each node (fn_7100560e8c). Names and layouts
// are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU811.h"
#include "types.h"

struct UnkEmmyTarget_b0275 {
    u8 pad_0x0[8];
    void* field_0x8;
    u8 pad_0x10[8];
    f32 field_0x18;
    f32 field_0x1c;
};

struct UnkEmmyHolder_b0275 {
    u8 pad_0x0[8];
    void* field_0x8;
};

struct UnkEmmyNode_b0275 {
    u8 pad_0x0[0x20];
    UnkEmmyNode_b0275* next;  // 0x20
};

struct UnkEmmyList_b0275 {
    void* vtable;
    u32 count;
    u32 pad_0xc;
    UnkEmmyNode_b0275* head;
    UnkEmmyNode_b0275* tail;
    base::UnkAllocator* allocator;
    u32 tag;
};

#pragma pack(push, 4)
struct UnkEmmyComp_b0275 {
    u32 active;                                 // 0x0
    UnkEmmyTarget_b0275* target;                // 0x4
    u8 pad_0xc[0x324 - 0xc];
    u32 countB;                                 // 0x324
    UnkEmmyTarget_b0275* itemsB[0x64];          // 0x328
    u8 pad_0x648[0x12cc - 0x648];
    u32 countA;                                 // 0x12cc
    UnkEmmyTarget_b0275* itemsA[1];             // 0x12d0
    u8 pad_0x12d8[0x15f0 - 0x12d8];
    s32 state;                                  // 0x15f0
    u8 pad_0x15f4[0x1608 - 0x15f4];
    UnkEmmyHolder_b0275* holder;                // 0x1608
    u8 pad_0x1610[0x1628 - 0x1610];
    u8 flag;                                    // 0x1628
    u8 pad_0x1629[3];
    u32 mode;                                   // 0x162c
};
#pragma pack(pop)

void fn_7100560b98(UnkEmmyComp_b0275* self);
void fn_7100560d14(UnkEmmyTarget_b0275* target, u8 flag, u32 mode, UnkEmmyList_b0275* list);
void fn_7100560e8c(UnkEmmyComp_b0275* self, UnkEmmyNode_b0275* node);

static inline __attribute__((always_inline)) bool SameOwner(UnkEmmyComp_b0275* self, UnkEmmyTarget_b0275* target) {
    UnkEmmyHolder_b0275* holder = self->holder;
    void* owner = target->field_0x8;
    return owner == holder->field_0x8;
}

bool fn_71005608ec(UnkEmmyComp_b0275* self) {
    if (self->state == 2) {
        UnkEmmyTarget_b0275* target = nullptr;
        u32 n;
        bool pick;
        if (self->active == 0) {
            n = self->countA;
            pick = true;
        } else {
            target = self->target;
            n = self->countA;
            pick = false;
            if (n != 0) {
                UnkEmmyTarget_b0275* last = (&self->itemsA[n])[-1];
                pick = target->field_0x18 + target->field_0x1c > last->field_0x18 + last->field_0x1c;
            }
        }
        if (pick) {
            self->state = n == 0 ? 4 : 3;
        } else if (SameOwner(self, target)) {
            reinterpret_cast<UnkEmmyTarget_b0275**>(reinterpret_cast<u8*>(&self->countA) + 4)[n] = target;
            self->countA = n + 1;
            fn_7100560b98(self);
        } else {
            fn_7100560b98(self);
            u32 b = self->countB;
            if (b == 0x64) {
                self->state = 4;
            } else {
                reinterpret_cast<UnkEmmyTarget_b0275**>(reinterpret_cast<u8*>(&self->countB) + 4)[b] = target;
                self->countB = b + 1;
                void* vtable = g_unk_7101a48bc0 + 0x10;
                UnkEmmyList_b0275 list;
                list.vtable = vtable;
                list.count = 0;
                list.head = nullptr;
                list.tail = nullptr;
                list.allocator = fn_7100083990();
                list.tag = 0x13;
                fn_7100560d14(target, self->flag, self->mode, &list);
                for (UnkEmmyNode_b0275* node = list.head; node; node = node->next)
                    fn_7100560e8c(self, node);
                list.vtable = vtable;
                for (UnkEmmyNode_b0275* node = list.head; node;) {
                    UnkEmmyNode_b0275* next = node->next;
                    list.allocator->vf_0x48(node);
                    node = next;
                }
            }
        }
    }
    return self->state == 2;
}
