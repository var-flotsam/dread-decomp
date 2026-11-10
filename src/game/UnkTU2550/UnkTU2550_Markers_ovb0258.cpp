#include "base/global/CStrId.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_71727be340[] __attribute__((visibility("hidden")));
extern char g_unk_71727be348[] __attribute__((visibility("hidden")));

struct UnkSceneObj_b0258s : UnkSceneNode {
    u8 pad_0x18[0x30];
};

void fn_7100dde5d0(UnkRenderPool* pool, const void* key, UnkSceneObj_b0258s** out, const CStrId* name);
void fn_71001f4c94(UnkToggle* self, bool on);
void fn_71001f4338(UnkToggle* self, bool on);

struct UnkMarkers_b0258s {
    UnkSceneObj_b0258s* first[20];   // 0x0
    UnkSceneObj_b0258s* second[20];  // 0xa0
    u32 field_0x140;
    u32 field_0x144;
    bool field_0x148;
};

void fn_7100dde0e4(UnkMarkers_b0258s* self) {
    for (u32 i = 0; i < 20; ++i) {
        UnkRenderManager* mgr = g_unk_7101d47300;
        UnkSceneObj_b0258s* node = nullptr;
        fn_7100dde5d0(mgr->field_0x218, g_unk_71727be340, &node, &g_unk_7101d470e0);
        fn_7100219ae0(mgr, node, 0);
        UnkSceneObj_b0258s* obj = node;
        UnkToggle* toggle = reinterpret_cast<UnkToggle*>(reinterpret_cast<char*>(obj) + 0x18);
        fn_71001f4c94(toggle, false);
        fn_71001f4338(toggle, false);
        self->first[i] = obj;
    }
    self->field_0x140 = 0;
    for (u32 i = 0; i < 20; ++i) {
        UnkRenderManager* mgr = g_unk_7101d47300;
        UnkSceneObj_b0258s* node = nullptr;
        fn_7100dde5d0(mgr->field_0x218, g_unk_71727be348, &node, &g_unk_7101d470e0);
        fn_7100219ae0(mgr, node, 0);
        UnkSceneObj_b0258s* obj = node;
        UnkToggle* toggle = reinterpret_cast<UnkToggle*>(reinterpret_cast<char*>(obj) + 0x18);
        fn_71001f4c94(toggle, false);
        fn_71001f4338(toggle, false);
        self->second[i] = obj;
    }
    self->field_0x144 = 0;
    self->field_0x148 = true;
}
