// Unit 441's fade quads (tools/tu_map.py; no class hint, 0x710035d4e0 on): the pair of coloured bars
// shown for a timed fade. The first call creates both from the shared white texture path (a function
// static, guarded) and the model map at +0x218 of the render manager, giving them the layer 2 and a
// sort order; the bars' scale, colour and position follow the fade. Names and layouts are placeholders.
#include <new>
#include "base/math/CVector.h"
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

extern u32 g_unk_71726b21d4 __attribute__((visibility("hidden")));  // live pairs

struct UnkWhiteTexture_ovb0259 {
    CFilePathStrId id;
    u64 guard;
};
extern UnkWhiteTexture_ovb0259 g_unk_71726b21e8 __attribute__((visibility("hidden")));

struct UnkQuadNode_ovb0259;
void fn_710035d9e0(void* map, const CFilePathStrId* path, UnkQuadNode_ovb0259** out, const CStrId* id);
void fn_710019c564(void* target, f32 a, f32 b, f32 c, f32 d);
struct UnkMinimapVec3;
void fn_710020a4c4(void* p, const UnkMinimapVec3& pos);
struct UnkStruct_710020a5a0;
void fn_710020a5a0(UnkStruct_710020a5a0* self, const base::math::CVector2D* scale);
void fn_710020ad90(void* self, s32 value);

struct UnkQuadRender_ovb0259 {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

struct UnkQuadNode_ovb0259 {
    u8 pad_0x0[0x18];
    u8 node[0x160];  // 0x18
    UnkQuadRender_ovb0259* render;  // 0x178
};

struct UnkFadeQuads_ovb0259 {
    UnkQuadNode_ovb0259* second;  // 0x0
    UnkQuadNode_ovb0259* first;   // 0x8
    f32 field_0x10;
    f32 field_0x14;
};

void fn_710035d7bc(UnkFadeQuads_ovb0259* self) {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726b21e8.guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726b21e8.guard)) {
            new (&g_unk_71726b21e8.id) CFilePathStrId("gui/textures/white.bctex", true);
            fn_7100000250(fn_7100000c7c, &g_unk_71726b21e8.id, &g_unk_7101a19000);
            __cxa_guard_release(&g_unk_71726b21e8.guard);
        }
    }
    if (self->first || self->second)
        return;
    u32 n = g_unk_71726b21d4;
    f32 y = (f32)n * 0.04f - 0.5f;
    self->field_0x10 = 0.1f;
    self->field_0x14 = y;
    g_unk_71726b21d4 = n + 1;
    UnkRenderManager* manager = g_unk_7101d47300;
    {
        UnkQuadNode_ovb0259* created = nullptr;
        fn_710035d9e0(manager->field_0x218,
                      reinterpret_cast<CFilePathStrId*>(reinterpret_cast<u8*>(&g_unk_71726b21d4) + 0x14), &created,
                      &g_unk_7101d470e0);
        fn_7100219ae0(manager, reinterpret_cast<UnkSceneNode*>(created), 2);
        self->first = created;
    }
    fn_710020ac34(reinterpret_cast<UnkToggle*>(self->first->node), false);
    fn_710019c564(self->first->render->field_0x78, 0.0f, 1.0f, 0.0f, 1.0f);
    fn_710020ad90(self->first->node, 500);
    if (self->second)
        return;
    manager = g_unk_7101d47300;
    {
        UnkQuadNode_ovb0259* created = nullptr;
        fn_710035d9e0(manager->field_0x218, &g_unk_71726b21e8.id, &created, &g_unk_7101d470e0);
        fn_7100219ae0(manager, reinterpret_cast<UnkSceneNode*>(created), 2);
        self->second = created;
    }
    fn_710020ac34(reinterpret_cast<UnkToggle*>(self->second->node), false);
    fn_710019c564(self->second->render->field_0x78, 0.3f, 0.3f, 0.3f, 1.0f);
    {
        u32 pos[3];
        void* node = self->second->node;
        u32 lo, hi;
        __builtin_memcpy(&lo, &self->field_0x10, 4);
        __builtin_memcpy(&hi, &self->field_0x14, 4);
        pos[0] = lo;
        pos[1] = hi;
        pos[2] = 0;
        fn_710020a4c4(node, *reinterpret_cast<const UnkMinimapVec3*>(pos));
    }
    {
        f32 scale[2] = {0.4f, 0.015f};
        fn_710020a5a0(reinterpret_cast<UnkStruct_710020a5a0*>(self->second->node),
                      reinterpret_cast<const base::math::CVector2D*>(scale));
    }
    fn_710020ad90(self->second->node, 501);
}
