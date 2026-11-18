// behaviortree::CEmmyPatrolBaseAction unit (tools/tu_map.py unit 803): after the base step (fn_710051db44), an
// owner's CEmmyAIComponent forgets its two pointers (+0x14760, +0x14768) and the action stops watching (+0xf8);
// the owner's +0x298 gets a kind-5 parameter block; then the action's node list and a few fields are reset and
// the AI component's list at +0x14728 is emptied. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

void fn_710051db44(void* self);

struct UnkNodeAllocator_l51575c {
    void** vtable;
    void Free(void* node) { reinterpret_cast<void (*)(UnkNodeAllocator_l51575c*, void*)>(vtable[9])(this, node); }
};

template <class Node>
struct UnkNodeList_l51575c {
    u32 count;
    u8 pad_0x4[4];
    Node* head;
    Node* tail;
    UnkNodeAllocator_l51575c* allocator;

    void Clear() {
        for (Node* node = head; node;) {
            Node* next = node->next;
            allocator->Free(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkPatrolNode_l51575c {
    u8 pad_0x0[8];
    UnkPatrolNode_l51575c* next;
};

struct UnkEmmyNode_l51575c {
    u8 pad_0x0[0x20];
    UnkEmmyNode_l51575c* next;
};

struct UnkEmmyAI_l51575c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x14720];
    UnkNodeList_l51575c<UnkEmmyNode_l51575c> nodes;  // 0x14728
    u8 pad_0x14748[0x18];
    void* field_0x14760;
    void* field_0x14768;
};
static_assert(__builtin_offsetof(UnkEmmyAI_l51575c, field_0x14768) == 0x14768, "layout");

struct UnkPatrolOwner_l51575c {
    u8 pad_0x0[0x240];
    UnkEmmyAI_l51575c* ai;  // 0x240
    u8 pad_0x248[0x50];
    void* field_0x298;
};

struct UnkPatrolBase_l51575c {
    u8 pad_0x0[0x18];
    UnkPatrolOwner_l51575c* owner;
    u8 pad_0x20[0xc];
    s32 field_0x2c;
    u8 pad_0x30[8];
    UnkNodeList_l51575c<UnkPatrolNode_l51575c> list;  // 0x38
    u8 pad_0x58[0x50];
    u32 field_0xa8;
    u8 pad_0xac[0x24];
    u32 field_0xd0;
    u8 pad_0xd4[0x24];
    void* watch;  // 0xf8
    u8 pad_0x100[0x1c];
    bool field_0x11c;
};
static_assert(__builtin_offsetof(UnkPatrolBase_l51575c, field_0x11c) == 0x11c, "layout");

// The AI component when it is a CEmmyAIComponent, else null.
static inline UnkEmmyAI_l51575c* AsEmmy(UnkEmmyAI_l51575c* ai) {
    return ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? ai : nullptr;
}

void fn_710051575c(UnkPatrolBase_l51575c* self) {
    fn_710051db44(self);
    if (UnkEmmyAI_l51575c* ai = AsEmmy(self->owner->ai)) {
        ai->field_0x14768 = nullptr;
        ai->field_0x14760 = nullptr;
        if (self->watch) {
            fn_7100514b7c(self->watch, &self->watch);
            self->watch = nullptr;
        }
    }
    if (void* target = self->owner->field_0x298) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        fn_71006f4b0c(target, &params, 1);
    }
    self->field_0x11c = false;
    self->list.Clear();
    self->field_0x2c = -1;
    self->field_0xa8 = 0;
    self->field_0xd0 = 0;
    if (UnkEmmyAI_l51575c* emmy = AsEmmy(self->owner->ai))
        emmy->nodes.Clear();
}
