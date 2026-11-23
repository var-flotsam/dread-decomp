// CLightManager unit (tools/tu_map.py): the light manager's set-up. Registers the manager for the
// "lightsdef" type, then creates its pools of directional, omni, spot and segment light objects up
// front (128, 256, 128 and 128 of them). Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkDirLightBase.h"
#include "game/UnkDirLightObject.h"
#include "game/UnkDirLightResRef.h"
#include "game/UnkLightBase.h"
#include "game/UnkLightNode.h"
#include "game/UnkLightObject.h"
#include "game/UnkLightResRef.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSegmentLightBase.h"
#include "game/UnkSegmentLightNode.h"
#include "game/UnkSegmentLightObject.h"
#include "game/UnkSegmentLightResRef.h"
#include "game/UnkSpotLightBase.h"
#include "game/UnkSpotLightObject.h"
#include "game/UnkSpotLightResRef.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU27.h"
#include "game/UnkTU290.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

static_assert(sizeof(UnkDirLightObject) == 0x140, "UnkDirLightObject layout");

static_assert(sizeof(UnkSpotLightObject) == 0x140, "UnkSpotLightObject layout");

static_assert(sizeof(UnkLightObject) == 0x220, "UnkLightObject layout");

static_assert(sizeof(UnkSegmentLightObject) == 0x140, "UnkSegmentLightObject layout");

// The render pool's maps of each kind of light (their own files define them).
struct UnkDirLightMap;
struct UnkSpotLightMap;
struct UnkLightMap;
struct UnkSegmentLightMap;

UnkDirLightBase* fn_71008ebca8(UnkDirLightMap* map, UnkCountedResource* resource, UnkDirLightObject** out,
                               const void* arg);
UnkLightBase* fn_71008f2444(UnkLightMap* map, UnkCountedResource* resource, UnkLightObject** out,
                            const void* arg);
UnkSpotLightBase* fn_71008fa8f8(UnkSpotLightMap* map, UnkCountedResource* resource, UnkSpotLightObject** out,
                                const void* arg);
UnkSegmentLightBase* fn_71008f5084(UnkSegmentLightMap* map, UnkCountedResource* resource,
                                   UnkSegmentLightObject** out, const void* arg);

// The light manager (0x198 bytes): a pair of light lists for each kind, the first filled here.
struct UnkLightManager_le09794 {
    u8 pad_0x0[0x8];
    CRntVector<UnkDirLightObject*> dir_lights;          // 0x8
    CRntVector<UnkDirLightObject*> dir_lights_2;        // 0x30
    CRntVector<UnkLightObject*> omni_lights;            // 0x58
    CRntVector<UnkLightObject*> omni_lights_2;          // 0x80
    CRntVector<UnkSpotLightObject*> spot_lights;        // 0xa8
    CRntVector<UnkSpotLightObject*> spot_lights_2;      // 0xd0
    CRntVector<UnkSegmentLightObject*> segment_lights;  // 0xf8
    CRntVector<UnkSegmentLightObject*> segment_lights_2;  // 0x120
    u8 pad_0x148[0x48];
    bool initialised;  // 0x190
};
static_assert(__builtin_offsetof(UnkLightManager_le09794, initialised) == 0x190, "UnkLightManager layout");

// The manager, created on first use.
static inline void* LightManager() {
    if (!g_unk_71726b1688) {
        void* manager = fn_71000839a0(0x198, 3);
        fn_7100359118(manager);
        g_unk_71726b1688 = manager;
    }
    return g_unk_71726b1688;
}

// A registry entry (UnkEntry_7100d92158 in src/game_s7/fn_7100d92158.cpp), built inline with the
// default id, the default path and the two default callbacks.
struct UnkLightsDefRegistration {
    const void* desc;
    void* factory;
    CStrId id;
    CFilePathStrId path;
    CFilePathStrId path_0x20;
    void* field_0x28;
    void (*callback_0x30)(void* context, void* arg);
    void* field_0x38;
    void (*callback_0x40)(void* context, void* arg);

    UnkLightsDefRegistration(const void* desc, void* factory)
        : desc(desc), factory(factory), id(g_unk_7101d470e0), path(g_unk_7101d470e8), field_0x28(nullptr),
          callback_0x30(fn_71000204c4), field_0x38(nullptr), callback_0x40(fn_71000204cc) {}
};

// Creates a light object of one kind in the render manager's pool (with no resource) and puts it on
// the default layer.
template <class Light, class Base, class Map>
static inline Light* NewLight(Base* (*create)(Map*, UnkCountedResource*, Light**, const void*)) {
    UnkRenderManager* renderer = g_unk_7101d47300;
    Light* light = nullptr;
    create(reinterpret_cast<Map*>(renderer->field_0x218), nullptr, &light, &g_unk_7101d470e0);
    fn_7100219ae0(renderer, light ? reinterpret_cast<UnkSceneNode*>(&light->base) : nullptr, 0);
    return light;
}

void fn_7100e09794(void* self) {
    auto* manager = static_cast<UnkLightManager_le09794*>(self);
    {
        UnkRegistry_7100d92158* registry = g_unk_7101d4bbc0;
        UnkLightsDefRegistration entry(g_unk_71727b9f30, LightManager());
        fn_710002002c(registry, reinterpret_cast<const UnkEntry_7100d92158&>(entry));
    }

    manager->dir_lights.Reserve(128);
    manager->dir_lights_2.Reserve(128);
    for (u32 i = 0; i < 128; ++i) {
        UnkDirLightObject* light = NewLight(fn_71008ebca8);
        fn_710020c8ac(&light->node, false);
        manager->dir_lights.PushBack(light);
    }

    manager->omni_lights.Reserve(256);
    manager->omni_lights_2.Reserve(256);
    for (u32 i = 0; i < 256; ++i) {
        UnkLightObject* light = NewLight(fn_71008f2444);
        fn_710020c8ac(&light->node, false);
        manager->omni_lights.PushBack(light);
    }

    manager->spot_lights.Reserve(128);
    manager->spot_lights_2.Reserve(128);
    for (u32 i = 0; i < 128; ++i) {
        UnkSpotLightObject* light = NewLight(fn_71008fa8f8);
        fn_710020c8ac(&light->node, false);
        manager->spot_lights.PushBack(light);
    }

    manager->segment_lights.Reserve(128);
    manager->segment_lights_2.Reserve(128);
    for (u32 i = 0; i < 128; ++i) {
        UnkSegmentLightObject* light = NewLight(fn_71008f5084);
        fn_710020c8ac(&light->node, false);
        manager->segment_lights.PushBack(light);
    }

    manager->initialised = true;
}
