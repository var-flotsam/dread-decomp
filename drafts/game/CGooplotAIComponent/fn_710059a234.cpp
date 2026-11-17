// CGooplotAIComponent (tools/tu_map.py): a start step. Hands the owner's (+0x30) object at +0x218 a kind-3
// parameter block (vtable g_unk_7101a2e208) unless it is in mode 3, runs the behaviour-tree AI's step
// (fn_7100369d80), keeps only the owner's y angle, frees the nodes of the list at +0x14810 and counts the
// timer at +0x147f8 down by the tunable's +0x48 (to 1 at least). Names and layouts are placeholders.
#include "game/CAutsharpAIComponent.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CCharClassGooplotAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGooplotKind3Params_l59a234 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 kind;  // 0x38
    u8 pad_0x3c;
    u8 field_0x3d;

    UnkGooplotKind3Params_l59a234()
        : field_0xc(0), field_0x14(0), field_0x18(), field_0x28() {
        vtable = g_unk_7101a2e208 + 2;
        field_0x3d = 0;
        kind = 3;
    }
};

// The tunable's values this step reads.
struct UnkGooplotTunable_l59a234 {
    u8 pad_0x0[0x48];
    s32 field_0x48;
};

struct UnkGooplotNode_l59a234 {
    u8 pad_0x0[0x10];
    UnkGooplotNode_l59a234* next;
};

struct UnkGooplotAllocator_l59a234 {
    void** vtable;
    void Free(UnkGooplotNode_l59a234* node) {
        reinterpret_cast<void (*)(UnkGooplotAllocator_l59a234*, UnkGooplotNode_l59a234*)>(vtable[0x48 / 8])(this,
                                                                                                          node);
    }
};

struct UnkGooplotState_l59a234 {
    s32 timer;  // 0x147f8 of the component
    u8 pad_0x4[0xc];
    u32 count;
    u8 pad_0x14[4];
    UnkGooplotNode_l59a234* head;
    UnkGooplotNode_l59a234* tail;
    UnkGooplotAllocator_l59a234* allocator;
    u8 pad_0x30[0x49];
    bool field_0x79;
    bool field_0x7a;
};

struct UnkGooplotOwner_l59a234 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x218 - 0x94];
    UnkEffectSink* sink;  // 0x218
    u8 pad_0x220[0x3c0 - 0x220];
    bool field_0x3c0;  // set when the transform changed

    UnkVec3f GetAngle() { return xform.GetAngle(); }
    void SetAngle(const UnkVec3f& angle) {
        xform.SetAngle(angle);
        field_0x3c0 = true;
    }
};

struct UnkGooplotAI_l59a234 {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_l59a234* owner;
    u8 pad_0x38[0x147f8 - 0x38];
    UnkGooplotState_l59a234 state;  // 0x147f8
};

void fn_710059a234(UnkGooplotAI_l59a234* self, void* arg) {
    if (UnkEffectSink* sink = self->owner->sink) {
        if (fn_71010cc3a4(sink) != 3) {
            UnkGooplotKind3Params_l59a234 params;
            fn_71010c7e6c(sink, &params, 1);
        }
    }
    fn_7100369d80(reinterpret_cast<UnkWarriorSend_b58*>(self), arg);
    UnkGooplotState_l59a234* state = &self->state;
    UnkVec3f angle = self->owner->GetAngle();
    self->owner->SetAngle(UnkVec3f(0.0f, angle.y, 0.0f));
    for (UnkGooplotNode_l59a234* node = state->head; node;) {
        UnkGooplotNode_l59a234* next = node->next;
        state->allocator->Free(node);
        node = next;
    }
    state->count = 0;
    state->head = nullptr;
    state->tail = nullptr;
    state->field_0x7a = false;
    s32 timer = state->timer - reinterpret_cast<UnkGooplotTunable_l59a234*>(&g_unk_71726e72b8)->field_0x48;
    state->field_0x79 = timer > 1;
    state->timer = timer > 1 ? timer : 1;
}
