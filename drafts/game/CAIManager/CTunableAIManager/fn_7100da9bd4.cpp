// CTunableAIManager's unit (tools/tu_map.py unit 2540): when the AI tunables enable it, gives an object a
// debug label: a list node (pooled, at most the pool's size) holding a weak handle to the object and a
// scene object drawn in the "digital_small" font, scaled by the object's AI component (0.8 without one).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU257.h"
#include "game/UnkTU2744.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkSceneMap_lc93ff8;
struct UnkCountedResource;
struct UnkSceneObject_lc93ff8;
UnkSceneObject_lc93ff8* fn_7100c93ff8(UnkSceneMap_lc93ff8* map, UnkCountedResource* resource,
                                      UnkSceneObject_lc93ff8** out, const void* arg);

// From src/game/UnkUnit224/fn_710019db08.cpp (the same definition).
struct UnkBlendLayer224 {
    u8 pad_0x0[0x148];
    s32 kind;  // 0x148
    u8 pad_0x14c[0x58];
    u32 mode;  // 0x1a4
    u8 pad_0x1a8[0x7];
    bool additive;  // 0x1af
};
static_assert(__builtin_offsetof(UnkBlendLayer224, additive) == 0x1af, "UnkBlendLayer224 layout");

void fn_710019dbf4(UnkBlendLayer224* self);
void fn_710019db74(UnkBlendLayer224* self, f32 r, f32 g, f32 b, f32 a);
void fn_710020c248(void* self, s32 priority);

// The AI tunables: whether labels are shown (+0x30).
struct UnkAITunables_lda9bd4 {
    u8 pad_0x0[0x30];
    bool show_labels;
};

// The label's text: its layer (+0x78) and its scale (+0x94).
struct UnkAILabelText_lda9bd4 {
    u8 pad_0x0[0x78];
    UnkBlendLayer224* layer;  // 0x78
    u8 pad_0x80[0x14];
    f32 scale_x;  // 0x94
    f32 scale_y;  // 0x98
};

// The label's scene object.
struct UnkAILabel_lda9bd4 {
    u8 pad_0x0[0x20];
    u8 field_0x20[0xfb];
    bool dirty;  // 0x11b
    u8 pad_0x11c[0x14];
    UnkAILabelText_lda9bd4* text;  // 0x130
    void* font;  // 0x138
};
static_assert(__builtin_offsetof(UnkAILabel_lda9bd4, text) == 0x130, "UnkAILabel_lda9bd4 layout");

struct UnkAILabelNode_lda9bd4 {
    UnkAILabel_lda9bd4* label;
    UnkWeakHandle* handle;
    UnkAILabelNode_lda9bd4* next;

    UnkAILabelNode_lda9bd4() : label(nullptr), handle(nullptr), next(nullptr) {}
};

// The object's AI component holder (+0x240): the component at +0x10.
struct UnkAIComponentRef_lda9bd4 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* component;
};

// A CCharClassAIComponent: the label scale at +0x4e0.
struct UnkCharClassAIComponent_lda9bd4 {
    u8 pad_0x0[0x4e0];
    f32 label_scale;
};

struct UnkAITarget_da92b0 {
    u8 pad_0x0[0x240];
    UnkAIComponentRef_lda9bd4* ai;  // 0x240
};

struct UnkAIManager_da92b0 {
    u8 pad_0x0[0x2530];
    base::UnkLinkedList<UnkAILabelNode_lda9bd4> labels;  // 0x2530
    u8 pad_0x2558[0x10];
    u64 label_pool_size;  // 0x2568

    void PushLabel() {
        void* mem = labels.allocator->vf_0x40(sizeof(UnkAILabelNode_lda9bd4), 8, labels.flags);
        if (mem)
            labels.PushBackTailFirst(new (mem) UnkAILabelNode_lda9bd4());
    }
};
static_assert(__builtin_offsetof(UnkAIManager_da92b0, label_pool_size) == 0x2568, "UnkAIManager_da92b0 layout");

static inline UnkAITarget_da92b0* Target(UnkWeakHandle* handle) {
    return reinterpret_cast<UnkAITarget_da92b0*>(handle->target);
}

void fn_7100da9bd4(UnkAIManager_da92b0* self, UnkAITarget_da92b0* object) {
    if (!object || !reinterpret_cast<UnkAITunables_lda9bd4*>(&g_unk_71727bc558)->show_labels)
        return;
    for (UnkAILabelNode_lda9bd4* node = self->labels.head; node; node = node->next) {
        if ((node->handle ? Target(node->handle) : nullptr) == object)
            return;
    }
    if (self->labels.count >= self->label_pool_size / sizeof(UnkAILabelNode_lda9bd4))
        return;

    self->PushLabel();
    UnkAILabelNode_lda9bd4* node = self->labels.tail;
    AssignRef(&node->handle, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(object)));

    {
        UnkRenderManager* render = g_unk_7101d47300;
        UnkSceneObject_lc93ff8* scene = nullptr;
        fn_7100c93ff8(reinterpret_cast<UnkSceneMap_lc93ff8*>(render->field_0x218), nullptr, &scene,
                      &g_unk_7101d470e0);
        fn_7100219ae0(render, reinterpret_cast<UnkSceneNode*>(scene), 2);
        node->label = reinterpret_cast<UnkAILabel_lda9bd4*>(scene);
    }
    if (!node->label)
        return;

    UnkGuiRoot* gui = g_unk_717271e8f8;
    void* font;
    {
        CStrId name("digital_small", true);
        font = CGUIManager_GetOrLoadCachedByPath(gui, &name);
    }
    if (!font)
        return;

    f32 scale = 0.8f;
    UnkAILabel_lda9bd4* label = node->label;
    UnkAILabelText_lda9bd4* text = label->text;
    label->font = font;
    fn_71001c3ec0(reinterpret_cast<UnkLabelTextNode*>(text), static_cast<char*>(font) + 0x18);
    label->dirty = true;
    fn_710020c248(node->label->field_0x20, -1000);

    if (UnkAIComponentRef_lda9bd4* ai = Target(node->handle)->ai) {
        if (base::reflection::UnkReflObject* component = ai->component) {
            if (fn_710009d50c(component->vf_0x0(), fn_7100408b2c()))
                scale = reinterpret_cast<UnkCharClassAIComponent_lda9bd4*>(component)->label_scale;
        }
    }
    label = node->label;
    label->text->scale_x = scale;
    label->text->scale_y = scale;
    label->dirty = true;
    node->label->text->layer->additive = false;
    fn_710019dbf4(node->label->text->layer);
    fn_710019db74(node->label->text->layer, 1.0f, 1.0f, 1.0f, 1.0f);
}
