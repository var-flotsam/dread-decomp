// The map menu's unit (tools/tu_map.py unit 1689, hint GUI::CMapMenuController): a message handler. After the
// base handler, and only while the current (or pending) entry is not entry 0: a message of kind 2 fades out the
// two sounds, shows the widget at +0x2f8, refreshes the text block at +0x618, hands its items back to the GUI
// root, clears their ids, leaves through fn_7100b3feb8 and drops the unit's two keys from the root's list; any
// other message from one of the two buttons (+0x240, +0x238) goes on through fn_7100b3feb8. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiHandle.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU1689.h"
#include "game/UnkTU2567.h"
#include "types.h"

using base::global::CStrId;

// The unit's two keys in the GUI root's list, side by side (the second is g_unk_7172763d98).
extern CStrId g_unk_7172763d90[2] __attribute__((visibility("hidden")));

struct UnkMapMessage_lb44bf4 {
    UnkGuiHandleObj* sender;  // 0x0
    u8 pad_0x8[0x18];
    u64 kind;  // 0x20
};

struct UnkMapEntry_lb44bf4 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkMapSoundHandle_lb44bf4 {
    UnkSound* sound;
    u64 id;
};

struct UnkMapSoundRef_lb44bf4 {
    UnkMapSoundHandle_lb44bf4* handle;
    u64 handle_id;
};

// The items handed back to the GUI root, with an id each; a range from data to data + count.
struct UnkMapItems_lb44bf4 {
    void** data;
    CStrId* ids;
    u32 count;
    void** begin() const { return data; }
    void** end() const { return data + count; }
};

struct UnkMapMenu_lb44bf4 {
    u8 pad_0x0[0x188];
    bool field_0x188;  // 0x188
    u8 pad_0x189[0x238 - 0x189];
    UnkGuiHandleObj* button_0x238;  // 0x238
    UnkGuiHandleObj* button_0x240;  // 0x240
    u8 pad_0x248[0x2f8 - 0x248];
    UnkGuiHandleObj* handle_0x2f8;  // 0x2f8
    u8 pad_0x300[0x398 - 0x300];
    UnkMapItems_lb44bf4 items;  // 0x398
    u8 pad_0x3b0[0x618 - 0x3b0];
    u8 text_0x618[0xd0];  // 0x618
    u8 pad_0x6e8[0x928 - 0x6e8];
    UnkMapEntry_lb44bf4* entry;  // 0x928
    u8 pad_0x930[0x9bc - 0x930];
    s32 pending;  // 0x9bc
    u8 pad_0x9c0[0x11b0 - 0x9c0];
    UnkMapSoundRef_lb44bf4 sound_0x11b0;  // 0x11b0
    UnkMapSoundRef_lb44bf4 sound_0x11c0;  // 0x11c0
};
static_assert(__builtin_offsetof(UnkMapMenu_lb44bf4, sound_0x11c0) == 0x11c0, "layout");

// As src/game/GUI/CMapMenuController_Timers_b43.cpp declares it.
struct UnkMapTimersController_b43a3;

// Moves on to the next step (fn_7100b3feb8).
static inline void Step(UnkMapMenu_lb44bf4* self, s32 a, u32 b) {
    fn_7100b3feb8(reinterpret_cast<UnkMapTimersController_b43a3*>(self), a, b);
}

static inline s32 KindOf(const UnkMapEntry_lb44bf4* entry) {
    return entry ? entry->kind : 0x7fffffff;
}

static inline UnkGuiDisplayObject* TargetOf(const UnkGuiHandleObj* handle) {
    return handle ? handle->target : nullptr;
}

static inline void FadeOut(const UnkMapSoundRef_lb44bf4& ref) {
    UnkMapSoundHandle_lb44bf4* handle = ref.handle;
    if (handle && handle->id == ref.handle_id && ref.handle_id != ~0ull && handle->sound)
        fn_7100105a80(handle->sound, 0.2f, 0.0f);
}

void fn_7100b44bf4(UnkMapMenu_lb44bf4* self, UnkMapMessage_lb44bf4* msg) {
    fn_71011d9a44(reinterpret_cast<UnkSmBase_A6_b62*>(self), reinterpret_cast<UnkSmArg_A6_b62*>(msg));
    if (KindOf(self->entry) == 0)
        return;
    s32 next = self->pending == 0x7fffffff ? KindOf(self->entry) : self->pending;
    if (next == 0)
        return;
    if (msg->kind == 2) {
        FadeOut(self->sound_0x11b0);
        FadeOut(self->sound_0x11c0);
        fn_71011cdc30(TargetOf(self->handle_0x2f8), true);
        fn_7100eb7358(self->text_0x618, 0, 1);
        if (self->field_0x188)
            self->field_0x188 = false;
        for (void* item : self->items)
            fn_71011b57c4(g_unk_717271e8f8, item);
        if (self->items.ids) {
            for (u64 i = 0; i < self->items.count; ++i)
                self->items.ids[i].Reset();
        }
        self->items.count = 0;
        Step(self, 0, 0);
        CStrId* keys = g_unk_7172763d90;
        fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &keys[1]);
        fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &keys[0]);
        return;
    }
    UnkGuiDisplayObject* sender = TargetOf(msg->sender);
    if (sender == TargetOf(self->button_0x240))
        Step(self, 1, 0);
    else if (TargetOf(msg->sender) == TargetOf(self->button_0x238))
        Step(self, 1, 1);
}
