// CLightManager unit (tools/tu_map.py): the light manager's set-up. Registers the manager for the
// "lightsdef" type, then creates its pools of directional, omni, spot and segment light objects up
// front (128, 256, 128 and 128 of them). Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU27.h"
#include "game/UnkTU290.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

// The light objects, as the light components' files define them (src/game/CDirLightComponent.cpp,
// src/game/CSpotLightComponent.cpp, src/game/COmniLightComponent.cpp,
// src/game/CSegmentLightComponent.cpp).
// The scene object's part at +0x20 with a virtual update (as COmniLightComponent.cpp's).
struct UnkLightNode {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
};

// A counted resource reference (base/UnkCountedResource.h).
struct UnkDirLightResRef {
    UnkCountedResource* ptr;

    void Set(UnkCountedResource* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
};

// The object's base at +8 (what the map is keyed by and the caller gets back).
struct UnkDirLightBase {
    u64 field_0x0;
};

// The 0x140-byte scene object.
struct UnkDirLightObject {
    void* vtable;
    UnkDirLightBase base;          // 0x8
    UnkDirLightResRef resource;    // 0x10
    u8 pad_0x18[0x8];
    UnkLightNode node;             // 0x20
    u8 pad_0x28[0x70];
    UnkDirLightResRef resource2;   // 0x98
    u8 pad_0xa0[0xa0];
};
static_assert(sizeof(UnkDirLightObject) == 0x140, "UnkDirLightObject layout");

// A counted resource reference (base/UnkCountedResource.h).
struct UnkSpotLightResRef {
    UnkCountedResource* ptr;

    void Set(UnkCountedResource* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
};

// The object's base at +8 (what the map is keyed by and the caller gets back).
struct UnkSpotLightBase {
    u64 field_0x0;
};

// The 0x140-byte scene object.
struct UnkSpotLightObject {
    void* vtable;
    UnkSpotLightBase base;          // 0x8
    UnkSpotLightResRef resource;    // 0x10
    u8 pad_0x18[0x8];
    UnkLightNode node;              // 0x20
    u8 pad_0x28[0x70];
    UnkSpotLightResRef resource2;   // 0x98
    u8 pad_0xa0[0xa0];
};
static_assert(sizeof(UnkSpotLightObject) == 0x140, "UnkSpotLightObject layout");

// A counted resource reference (base/UnkCountedResource.h), as src/game/UnkEnvironmentFade.cpp's.
struct UnkLightResRef {
    UnkCountedResource* ptr;

    void Set(UnkCountedResource* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
};

// The object's base at +8 (what the map is keyed by and the caller gets back).
struct UnkLightBase {
    u64 field_0x0;
};

// The 0x220-byte scene object.
struct UnkLightObject {
    void* vtable;
    UnkLightBase base;          // 0x8
    UnkLightResRef resource;    // 0x10
    u8 pad_0x18[0x10];
    UnkLightNode node;          // 0x28
    u8 pad_0x30[0x70];
    UnkLightResRef resource2;   // 0xa0
    u8 pad_0xa8[0x178];
};
static_assert(sizeof(UnkLightObject) == 0x220, "UnkLightObject layout");

// The scene object's part at +0x20 with a virtual update.
struct UnkSegmentLightNode {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
};

// A counted resource reference (base/UnkCountedResource.h).
struct UnkSegmentLightResRef {
    UnkCountedResource* ptr;

    void Set(UnkCountedResource* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
};

// The object's base at +8 (what the map is keyed by and the caller gets back).
struct UnkSegmentLightBase {
    u64 field_0x0;
};

// The 0x140-byte scene object.
struct UnkSegmentLightObject {
    void* vtable;
    UnkSegmentLightBase base;            // 0x8
    UnkSegmentLightResRef resource;      // 0x10
    u8 pad_0x18[0x8];
    UnkSegmentLightNode node;            // 0x20
    u8 pad_0x28[0x70];
    UnkSegmentLightResRef resource2;     // 0x98
    u8 pad_0xa0[0xa0];
};
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
