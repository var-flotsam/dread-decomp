// Constructor of a node: runs its base constructor, sets the vtable, initialises its fields and
// names its string ids, then runs the post-construction virtual call.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode : UnkBehaviorTreeNodeBase {
    u8 pad_0x2c[4];
    UnkStrId field_0x30;
    u8 field_0x38;
};

extern void* g_unk_7101a25380[];

template <class Base, void (*BaseCtor)(Base*), void** Vtable, class Self>
static inline void Construct(Self* self) {
    BaseCtor(reinterpret_cast<Base*>(self));
    self->vtable = &Vtable[2];
    new (&self->field_0x30) UnkStrId("", true);
    self->field_0x38 = 0;
    reinterpret_cast<void (*)(Self*)>(self->vtable[3])(self);
}

void fn_71003750e4(UnkNode* self) { Construct<UnkBehaviorTreeNodeBase, fn_7100393e68, g_unk_7101a25380>(self); }

// The same constructor for another node (vtable g_unk_7101a8b150) above its own, larger base
// (fn_7100a028c4): the id and flag sit at 0x148 and 0x150.
void fn_7100a028c4(void* self);
extern void* g_unk_7101a8b150[];

struct UnkNode_7100a07cb4 {
    void** vtable;
    u8 pad_0x8[0x140];
    UnkStrId field_0x30;  // at 0x148
    u8 field_0x38;        // at 0x150
};

void fn_7100a07cb4(UnkNode_7100a07cb4* self) { Construct<void, fn_7100a028c4, g_unk_7101a8b150>(self); }
