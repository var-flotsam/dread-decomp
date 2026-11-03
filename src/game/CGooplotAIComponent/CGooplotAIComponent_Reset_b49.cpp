// CGooplotAIComponent (tools/tu_map.py): the reset. After the behaviour-tree AI's own steps
// (fn_71003695b8) it takes the tunable's value (+0x2c of g_unk_71726e72b8) into +0x147f8, sets two flags
// on the owner's (+0x30) component at +0x228, frees the nodes of the list at +0x14810 through its
// allocator (+0x14820, slot 0x48), clears the list and three flags, and copies each part's colour
// (+0x160..+0x168, alpha 0) into its +0x15c and its renderer's +0x30 (the parts of +0x78 of +0x138 of
// the component's +0x50). Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::math::CVector4D;

struct UnkFlagTarget;
void fn_7101172780(UnkFlagTarget* target, const char* name);
void fn_7101172764(UnkFlagTarget* target, const char* name);
extern UnkTunable g_unk_71726e72b8;
extern char g_unk_710164a040[];
extern char g_unk_710164a048[];

struct UnkGooplotNode_b49 {
    u8 pad_0x0[0x10];
    UnkGooplotNode_b49* next;
};

struct UnkGooplotAllocator_b49 {
    void** vtable;
    void Free(UnkGooplotNode_b49* node) {
        reinterpret_cast<void (*)(UnkGooplotAllocator_b49*, UnkGooplotNode_b49*)>(vtable[0x48 / 8])(this, node);
    }
};

struct UnkGooplotState_b49 {
    u8 field_0x0;
    u8 pad_0x1[0x12f];
    u32 field_0x130;  // +0x147f8 of the component
    u8 pad_0x134[0xc];
    u32 count;
    u8 pad_0x144[4];
    UnkGooplotNode_b49* head;
    UnkGooplotNode_b49* tail;
    UnkGooplotAllocator_b49* allocator;
    u8 pad_0x160[0x49];
    u8 field_0x1a9;
    u8 pad_0x1aa;
    u8 field_0x1ab;
};

struct UnkGooplotRenderer_b49 {
    u8 pad_0x0[0x30];
    CVector4D* color;
};

struct UnkGooplotPart_b49 {
    u8 pad_0x0[0x15c];
    CVector4D color;
    u8 pad_0x16c[0x8c];
    UnkGooplotRenderer_b49* renderer;
};

struct UnkGooplotParts_b49 {
    u8 pad_0x0[0x2c];
    u32 count;
    u8 pad_0x30[0x18];
    UnkGooplotPart_b49** data;
};
struct UnkGooplotModel_b49 {
    u8 pad_0x0[0x78];
    UnkGooplotParts_b49* parts;
};
struct UnkGooplotScene_b49 {
    u8 pad_0x0[0x138];
    UnkGooplotModel_b49* model;
};
struct UnkGooplotComp_b49 {
    u8 pad_0x0[0x50];
    UnkGooplotScene_b49* scene;
};
struct UnkGooplotOwner_b49 {
    u8 pad_0x0[0x228];
    UnkGooplotComp_b49* comp;
};

struct UnkGooplotAI_b49 {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_b49* owner;
    u8 pad_0x38[0x14690];
    UnkGooplotState_b49 state;
};

void fn_710059a0a8(UnkGooplotAI_b49* self) {
    UnkGooplotState_b49* state = &self->state;
    fn_71003695b8(reinterpret_cast<UnkBehaviorTreeAI*>(self));
    state->field_0x130 = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&g_unk_71726e72b8) + 0x2c);
    if (UnkGooplotComp_b49* comp = self->owner->comp) {
        fn_7101172780(reinterpret_cast<UnkFlagTarget*>(comp), g_unk_710164a040);
        fn_7101172764(reinterpret_cast<UnkFlagTarget*>(comp), g_unk_710164a048);
    }
    for (UnkGooplotNode_b49* node = state->head; node;) {
        UnkGooplotNode_b49* next = node->next;
        state->allocator->Free(node);
        node = next;
    }
    state->count = 0;
    __builtin_memset(&state->head, 0, 16);
    state->field_0x1ab = 0;
    state->field_0x0 = 0;
    state->field_0x1a9 = 1;
    UnkGooplotScene_b49* scene = self->owner->comp->scene;
    u32 n = scene->model->parts->count;
    for (u32 i = 0; i < n; ++i) {
        UnkGooplotPart_b49* part = scene->model->parts->data[i];
        // Stored as words that may alias anything: the original reloads the part after the copy.
        typedef u32 __attribute__((may_alias)) word;
        CVector4D color;
        const word* src = reinterpret_cast<const word*>(&part->color);
        word* dst = reinterpret_cast<word*>(&color);
        dst[0] = 0;
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        part = scene->model->parts->data[i];
        fn_710007b518(&part->color, &color);
        if (part->renderer)
            *part->renderer->color = color;
    }
}
