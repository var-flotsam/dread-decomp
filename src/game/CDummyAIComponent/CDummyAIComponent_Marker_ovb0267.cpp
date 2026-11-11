// CDummyAIComponent's unit (tools/tu_map.py): refreshes the dummy's marker effect (the sub-object at
// +0x146b0, whose owner data has a flag at +0x4fc). The old effect (+0x8) is handed back to the render
// manager; then, when the actor has a collision container (+0x2a0) with a "Character" collider whose
// width is positive, a new effect is created from the manager's pool, put on layer 0, restarted with the
// colour (mode 6, 0x28, the collider width as timing), configured and moved to the actor's position.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkActor.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

void* fn_7100149930(UnkRenderPool*, void*, UnkSceneObject**, base::global::CStrId*);
void fn_71001beee4(UnkSceneObjectSub* target, u32 mode);
void fn_71001494e4(UnkSceneObject* self, bool flag);
void fn_7100149460(UnkSceneObject* object, const UnkVec3f* position);

struct UnkRGB_710050f26c {
    f32 r;
    f32 g;
    f32 b;
};

struct UnkColorArg_710050f26c {
    UnkRGB_710050f26c rgb;
    f32 a;
    UnkColorArg_710050f26c(const UnkRGB_710050f26c& rgb_, f32 a_) : rgb(rgb_), a(a_) {}
    UnkColorArg_710050f26c(const UnkColorArg_710050f26c& o) : rgb(o.rgb), a(o.a) {}
    // From four consecutive floats, word by word.
    explicit UnkColorArg_710050f26c(const f32* c) {
        ((a_u32*)&rgb)[0] = ((const a_u32*)c)[0];
        ((a_u32*)&rgb)[1] = ((const a_u32*)c)[1];
        ((a_u32*)&rgb)[2] = ((const a_u32*)c)[2];
        ((a_u32*)&a)[0] = ((const a_u32*)c)[3];
    }
};
bool fn_710014826c(UnkSceneObject*, u32, u32, UnkColorArg_710050f26c, bool, f32);

struct UnkDummyData_ovb0267 {
    u8 pad_0x0[0x4fc];
    bool field_0x4fc;
};

struct UnkDummyMarker_ovb0267 {
    UnkDummyData_ovb0267* data;
    UnkSceneObject* effect;
};

struct UnkDummyActor_ovb0267 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* colliders;
};

struct UnkDummyCollider_ovb0267 {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 field_0x40;
    u8 pad_0x44[4];
    f32 field_0x48;
    u8 pad_0x4c[0x14];
    bool field_0x60;
};

struct UnkDummyComp_ovb0267 {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
    u8 pad_0x38[0x14678];
    UnkDummyMarker_ovb0267 field_0x146b0;
};

void fn_71003b1a18(UnkDummyComp_ovb0267* self, const f32* color) {
    UnkDummyMarker_ovb0267* marker = &self->field_0x146b0;
    if (!marker->data->field_0x4fc)
        return;
    if (marker->effect) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(reinterpret_cast<char*>(marker->effect) + 8), 0);
        marker->effect = nullptr;
    }
    UnkDummyActor_ovb0267* actor = reinterpret_cast<UnkDummyActor_ovb0267*>(self->field_0x30);
    if (!actor->colliders)
        return;
    static CStrId s_name("Character", true);
    auto* collider = reinterpret_cast<UnkDummyCollider_ovb0267*>(
        fn_71008640a4(reinterpret_cast<UnkDummyActor_ovb0267*>(self->field_0x30)->colliders, &s_name));
    if (!collider)
        return;
    if (collider->field_0x60)
        reinterpret_cast<void (*)(UnkDummyCollider_ovb0267*)>(collider->vtable[0x98 / 8])(collider);
    f32 width = collider->field_0x48 - collider->field_0x40;
    if (!(width > 0.0f))
        return;
    {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkSceneObject* effect = nullptr;
        fn_7100149930(manager->field_0x218, nullptr, &effect, &g_unk_7101d470e0);
        fn_7100219ae0(manager, effect ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(effect) + 8) : nullptr, 0);
        marker->effect = effect;
        fn_710014826c(effect, 6, 0x28, UnkColorArg_710050f26c(color), true, width);
    }
    fn_71001beee4(marker->effect->field_0x130, 1);
    fn_71001494e4(marker->effect, true);
    UnkActor* actor2 = self->field_0x30;
    UnkSceneObject* node = marker->effect;
    UnkVec3f pos(actor2->field_0x44, actor2->field_0x54, actor2->field_0x64);
    fn_7100149460(node, &pos);
}
