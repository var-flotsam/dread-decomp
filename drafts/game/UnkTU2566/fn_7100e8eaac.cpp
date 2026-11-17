// build-flags: no-strict-aliasing
// CTunableMinimapManager's unit (2566): creates the minimap's scene objects: the camera, the two draw-pass
// objects (with the map's material) and the "MINIMAP" model, each added to its render layer.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU140.h"
#include "game/UnkTU223.h"
#include "game/UnkTU252.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSceneObj_71010e2278;
struct UnkModelInstance;
struct UnkCountedResource;
struct UnkObject_71001c3504;

void fn_71002198cc(UnkRenderManager* self, void* a, void* b, void* c, u32 layer, u32 flag);
void fn_7100e13928(void* set_, void* model_, UnkSceneObj_71010e2278** out, const CStrId* name);
UnkModelInstance* fn_710021d3d4(UnkRenderPool* pool, UnkCountedResource* resource, UnkModelInstance** out,
                                const base::global::CStrId* name);
void fn_71001c3504(UnkObject_71001c3504* self, const base::UnkStrIdMember* id);
void fn_710014543c(void* scene, bool enabled);
void fn_71001bab1c(void* sub, f32 scale);
void fn_7100148130(void* scene, u32 slot, u32 a, u32 b, u32 c);

// The part of a scene object at +0x130.
struct UnkMinimapSceneSub_le8eaac {
    u8 pad_0x0[0x18];
    u32 field_0x18;
};

struct UnkMinimapSceneBase_le8eaac {
    virtual void vf_0x0();
};

// A scene object: its node at +8.
struct UnkMinimapScene_le8eaac : UnkMinimapSceneBase_le8eaac, UnkSceneNode {
    u8 pad_0x20[0xfb];
    bool field_0x11b;
    u8 pad_0x11c[0x14];
    UnkMinimapSceneSub_le8eaac* sub;  // 0x130
};
static_assert(__builtin_offsetof(UnkMinimapScene_le8eaac, sub) == 0x130, "UnkMinimapScene_le8eaac layout");

struct UnkMinimapLayer_le8eaac {
    u8 pad_0x0[0x181];
    bool field_0x181;
};

struct UnkMinimapRender_le8eaac {
    u8 pad_0x0[0x78];
    UnkMinimapLayer_le8eaac* layer;  // 0x78
};

// A model: its node first.
struct UnkMinimapModel_le8eaac : UnkSceneNode {
    u8 field_0x18[0x160];
    UnkMinimapRender_le8eaac* render;  // 0x178
};

struct UnkMinimapScenes_le8eaac {
    UnkMinimapScene_le8eaac* camera;
    UnkMinimapModel_le8eaac* model;
    u8 pad_0x10[0x58];
    UnkMinimapScene_le8eaac* pass_objects[2];  // 0x68
};

// The map's material: its name and file.
struct UnkMinimapMaterial_le8eaac {
    CStrId name;
    CFilePathStrId path;
};

struct UnkMinimapDef_le8eaac {
    u8 pad_0x0[0x90];
    UnkMinimapMaterial_le8eaac material;  // 0x90
};

struct UnkMinimapSelf_le8eaac {
    u8 pad_0x0[0x60];
    UnkMinimapDef_le8eaac* def;  // 0x60
    u8 pad_0x68[0xd63f48 - 0x68];
    UnkMinimapScenes_le8eaac scenes;  // 0xd63f48
};

// A new effect object from the render pool, put on `layer`.
static inline UnkMinimapScene_le8eaac* CreateEffect(u32 layer) {
    UnkRenderManager* manager = g_unk_7101d47300;
    UnkMinimapScene_le8eaac* object = nullptr;
    fn_7100149930(manager->field_0x218, nullptr, reinterpret_cast<UnkSceneObject**>(&object),
                  &g_unk_7101d470e0);
    fn_7100219ae0(manager, object, layer);
    return object;
}

void fn_7100e8eaac(UnkMinimapSelf_le8eaac* self) {
    UnkMinimapScenes_le8eaac& scenes = self->scenes;
    fn_71002198cc(g_unk_7101d47300, nullptr, nullptr, nullptr, 6, 1);

    {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkMinimapScene_le8eaac* camera = nullptr;
        fn_7100e13928(manager->field_0x218, nullptr, reinterpret_cast<UnkSceneObj_71010e2278**>(&camera),
                      &g_unk_7101d470e0);
        fn_7100219ae0(manager, camera, 6);
        scenes.camera = camera;
        fn_710014543c(camera, true);
    }
    UnkMinimapScene_le8eaac* camera = scenes.camera;
    fn_71001bab1c(camera->sub, 1.0f);
    camera->field_0x11b = true;
    fn_7100219bb4(g_unk_7101d47300, reinterpret_cast<UnkSceneObj_71010e2278*>(scenes.camera), 6);

    // The two draw-pass objects: their two slots sized, switched off, and given the map's material.
    scenes.pass_objects[0] = CreateEffect(6);
    fn_7100148130(scenes.pass_objects[0], 0, 0x28, 0x50, 0x1c);
    fn_7100148130(scenes.pass_objects[0], 1, 0x28, 0x50, 0x1c);
    fn_7100149490(reinterpret_cast<UnkSceneObject*>(scenes.pass_objects[0]), false);
    fn_71001beee4(reinterpret_cast<UnkSceneObjectSub*>(scenes.pass_objects[0]->sub), true);
    scenes.pass_objects[0]->sub->field_0x18 = 4;
    fn_71001be430(reinterpret_cast<UnkSceneObjectSub*>(scenes.pass_objects[0]->sub), 0, false);
    fn_71001be430(reinterpret_cast<UnkSceneObjectSub*>(scenes.pass_objects[0]->sub), 1, false);
    {
        UnkMinimapScene_le8eaac* pass = scenes.pass_objects[0];
        UnkMinimapMaterial_le8eaac material = self->def->material;
        fn_71001be020(reinterpret_cast<UnkSceneObjectSub*>(pass->sub), reinterpret_cast<const u64*>(&material.path));
    }

    scenes.pass_objects[1] = CreateEffect(6);
    fn_7100148130(scenes.pass_objects[1], 0, 0x1e, 0x3c, 0x1c);
    fn_7100148130(scenes.pass_objects[1], 1, 0x1e, 0x3c, 0x1c);
    fn_7100149490(reinterpret_cast<UnkSceneObject*>(scenes.pass_objects[1]), false);
    fn_71001beee4(reinterpret_cast<UnkSceneObjectSub*>(scenes.pass_objects[1]->sub), true);
    scenes.pass_objects[1]->sub->field_0x18 = 4;
    fn_71001be430(reinterpret_cast<UnkSceneObjectSub*>(scenes.pass_objects[1]->sub), 0, false);
    fn_71001be430(reinterpret_cast<UnkSceneObjectSub*>(scenes.pass_objects[1]->sub), 1, false);
    {
        UnkMinimapScene_le8eaac* pass = scenes.pass_objects[1];
        UnkMinimapMaterial_le8eaac material = self->def->material;
        fn_71001be020(reinterpret_cast<UnkSceneObjectSub*>(pass->sub), reinterpret_cast<const u64*>(&material.path));
    }

    {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkMinimapModel_le8eaac* model = nullptr;
        fn_710021d3d4(manager->field_0x218, nullptr, reinterpret_cast<UnkModelInstance**>(&model),
                      &g_unk_7101d470e0);
        fn_7100219ae0(manager, model, 2);
        scenes.model = model;
    }
    CStrId name("MINIMAP", true);
    fn_71001c3504(reinterpret_cast<UnkObject_71001c3504*>(scenes.model->render),
                  reinterpret_cast<const base::UnkStrIdMember*>(&name));
    fn_710020ac34(reinterpret_cast<UnkToggle*>(scenes.model->field_0x18), false);
    UnkMinimapLayer_le8eaac* layer = scenes.model->render->layer;
    layer->field_0x181 = true;
    fn_710019c284(reinterpret_cast<UnkBlendLayer_710019c284*>(layer));
}
