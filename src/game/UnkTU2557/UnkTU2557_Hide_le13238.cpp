// Unit 2557 (loading screen): hides the loading screen. In the mode flagged at +0xb8 it unregisters
// the screen's GUI and render objects, then clears the progress and hides the sixteen icon pairs.
// Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2557.h"
#include "game/UnkTU2747.h"
#include "types.h"

struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};

// The parent link of a GUI object, 0x120 bytes into it.
struct UnkGuiParentLink_le13238 {
    u8 pad_0x0[0x120];
    void* field_0x120;
};

struct UnkLoadingOwner_le13238 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkLoadingIcons_le13238 {
    UnkGuiDisplayObject* field_0x0;
    UnkGuiDisplayObject* field_0x8;
    u64 field_0x10;
};

struct UnkLoadingScreen_le13238 {
    s32 field_0x0;
    u8 pad_0x4[0xc];
    UnkLoadingOwner_le13238* field_0x10;
    u8 pad_0x18[0x18];
    UnkLoadingIcons_le13238 icons[16];
    u64 field_0x1b0;
    u32 field_0x1b8;
    u8 pad_0x1bc[0xc];
    nn::os::MutexType mutex;
    u8 field_0x1e8;
};
static_assert(__builtin_offsetof(UnkLoadingScreen_le13238, field_0x1b0) == 0x1b0, "placeholder layout");
static_assert(__builtin_offsetof(UnkLoadingScreen_le13238, mutex) == 0x1c8, "placeholder layout");
static_assert(__builtin_offsetof(UnkLoadingScreen_le13238, field_0x1e8) == 0x1e8, "placeholder layout");

#define HIDDEN __attribute__((visibility("hidden"))) extern
HIDDEN void* g_unk_71727c0888;
HIDDEN void* g_unk_71727c0890;
HIDDEN void* g_unk_71727c0898;
HIDDEN void* g_unk_71727c08a0;
HIDDEN void* g_unk_71727c08a8;
HIDDEN void* g_unk_71727c08b0;
HIDDEN void* g_unk_71727c08d8;
HIDDEN void* g_unk_71727c08e0;
HIDDEN void* g_unk_71727c08e8;
HIDDEN void* g_unk_71727c08f0;
HIDDEN void* g_unk_71727c08f8;
HIDDEN void* g_unk_71727c0900;

static inline void RemoveGui(void*& object) {
    if (object) {
        fn_71011b57c4(g_unk_717271e8f8, object);
        object = nullptr;
    }
}

static inline void RemoveRender(void*& object) {
    if (object) {
        fn_71002199d8(g_unk_7101d47300, static_cast<UnkItemBase_71011a91c4*>(object), 0);
        object = nullptr;
    }
}

static inline void ResetProgress(UnkLoadingScreen_le13238* self) {
    fn_710006691c(&self->mutex);
    if (!self->field_0x1e8) {
        self->field_0x1b0 = 0;
        self->field_0x1b8 = 0;
    }
    fn_7100066920(&self->mutex);
}

static inline void HideIcon(UnkLoadingScreen_le13238* self, UnkGuiDisplayObject* icon) {
    fn_71011cdce8(icon, false);
    fn_71011cdc30(icon, false);
    void* parent = reinterpret_cast<UnkGuiParentLink_le13238*>(icon)->field_0x120;
    void* mine = self->field_0x10;
    if (mine)
        mine = self->field_0x10->field_0x10;
    if (mine == parent)
        fn_71011cef74(icon, nullptr);
}

void fn_7100e13238(UnkLoadingScreen_le13238* self) {
    fn_710006691c(&self->mutex);
    if (self->field_0x0 == 0) {
        if (g_unk_71726bc570->field_0xb8) {
            RemoveGui(g_unk_71727c0890);
            RemoveGui(g_unk_71727c0888);
            RemoveGui(g_unk_71727c0898);
            RemoveGui(g_unk_71727c08a0);
            RemoveGui(g_unk_71727c08a8);
            RemoveGui(g_unk_71727c08b0);
            RemoveRender(g_unk_71727c08e8);
            RemoveRender(g_unk_71727c0708);
            RemoveGui(g_unk_71727c08f0);
            RemoveGui(g_unk_71727c08f8);
            RemoveGui(g_unk_71727c0900);
            RemoveGui(g_unk_71727c08d8);
            RemoveGui(g_unk_71727c08e0);
        }
        ResetProgress(self);
        for (int i = 0; i < 16; i++) {
            HideIcon(self, self->icons[i].field_0x0);
            HideIcon(self, self->icons[i].field_0x8);
        }
    }
    fn_7100066920(&self->mutex);
}
