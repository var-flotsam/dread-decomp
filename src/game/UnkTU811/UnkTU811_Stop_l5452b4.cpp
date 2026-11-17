// UnkTU811 (CEmmyAIComponent's unit): stops the E.M.M.I.'s AI: drops the animation listener and the
// navigation-mesh listeners, switches the alerts off, saves the record, stops its effects and its rumble,
// and runs the base stop and the timeline reset. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU39.h"
#include "game/UnkTU50.h"
#include "game/UnkTU811.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkRecord;
struct UnkCentralUnit_b49;
struct UnkEmmyAI_b60;
UnkRecord* fn_710054715c(UnkRecord* self, const UnkRecord* other);
void fn_71007717c0(UnkCentralUnit_b49* self);
void fn_7100541fd4(UnkEmmyAI_b60* self);
void fn_7100369710(void* self);

struct UnkMeshListeners_l5452b4;
struct UnkMeshListener_l5452b4;
void fn_7100566080(UnkMeshListeners_l5452b4* list, UnkMeshListener_l5452b4* listener);

struct UnkTimeline_l5452b4;
void fn_7100705a64(UnkTimeline_l5452b4* self);

struct UnkMeshListener_l5452b4 {
    UnkMeshListeners_l5452b4* list;
    u8 pad_0x8[0x10];

    void Detach() {
        if (list) {
            fn_7100566080(list, this);
            list = nullptr;
        }
    }
};

// A playing rumble and the handle to it: current while the target's serial is the handle's.
struct UnkRumble_l5452b4 {
    u8 pad_0x0[0x40];
    base::global::CFilePathStrId path;  // 0x40
};
struct UnkRumbleTarget_l5452b4 {
    UnkRumble_l5452b4* object;
    u64 serial;
};
struct UnkRumbleHandle_l5452b4 {
    UnkRumbleTarget_l5452b4* target;
    u64 serial;

    UnkRumble_l5452b4* Get() const {
        if (target && target->serial == serial && serial != ~0ull)
            return target->object;
        return nullptr;
    }
};

struct UnkSwitch_l5452b4 {
    void** vtable;

    void vf_0x20(bool on) { reinterpret_cast<void (*)(UnkSwitch_l5452b4*, bool)>(vtable[0x20 / 8])(this, on); }
};

// The alert switches and the record they keep.
struct UnkAlerts_l5452b4 {
    UnkSwitch_l5452b4 first;
    u8 pad_0x8[0xf8];
    UnkSwitch_l5452b4 second;  // 0x100
    u8 pad_0x108[0x2b8];
    u8 record[0x8];            // 0x3c0
    u32 recordTime;            // 0x3c8
};

struct UnkContext_l5452b4 {
    u8 pad_0x0[0x610];
    bool showsEffects;  // 0x610
};

struct UnkCentralSlot_l5452b4 {
    u8 pad_0x0[0x10];
    UnkCentralUnit_b49* unit;  // 0x10
};

struct UnkOwner_l5452b4 {
    u8 pad_0x0[0x280];
    UnkWorld* effects;  // 0x280
};

struct UnkGameManagerView_l5452b4 {
    u8 pad_0x0[0x1fa8];
    base::UnkDeferredSet<base::UnkDeferredCallback<bool>> onEmmyStop;  // 0x1fa8
};

struct UnkEmmyAI_l5452b4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l5452b4* owner;  // 0x30
    u8 pad_0x38[0x14780 - 0x38];
    UnkAlerts_l5452b4 alerts;  // 0x14780
    u8 pad_0x14b50[0x14b60 - 0x14b50];
    u8 savedRecord[0x20];      // 0x14b60
    u8 pad_0x14b80[0x14c68 - 0x14b80];
    UnkMeshListener_l5452b4 meshListeners[2];  // 0x14c68
    u8 pad_0x14c98[0x29418 - 0x14c98];
    u8 field_0x29418[0x120];   // 0x29418
    UnkRumbleHandle_l5452b4 rumble;  // 0x29538
    u8 pad_0x29548[0x10];
    UnkCentralSlot_l5452b4* central;  // 0x29558
    u8 pad_0x29560[0x29ee0 - 0x29560];
    UnkContext_l5452b4* context;      // 0x29ee0
    u8 pad_0x29ee8[0x29f08 - 0x29ee8];
    u32 field_0x29f08;         // 0x29f08
    u8 pad_0x29f0c[0x2a268 - 0x29f0c];
    UnkTimeline_l5452b4* timeline() { return reinterpret_cast<UnkTimeline_l5452b4*>(field_0x2a268); }
    u8 field_0x2a268[0x100];   // 0x2a268
    bool notifyOnStop;         // 0x2a368
    u8 pad_0x2a369[0x2a428 - 0x2a369];
    u8 animLink[0x18];         // 0x2a428
    UnkWeakHandle* animHandle;  // 0x2a440

    void vf_0x7d0() { reinterpret_cast<void (*)(UnkEmmyAI_l5452b4*)>(vtable[0x7d0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyAI_l5452b4, rumble) == 0x29538, "UnkEmmyAI_l5452b4 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l5452b4, field_0x29f08) == 0x29f08, "UnkEmmyAI_l5452b4 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l5452b4, notifyOnStop) == 0x2a368, "UnkEmmyAI_l5452b4 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l5452b4, animHandle) == 0x2a440, "UnkEmmyAI_l5452b4 layout");

void fn_71005452b4(UnkEmmyAI_l5452b4* self) {
    fn_710002fa90(self->field_0x29418);

    if (UnkWeakHandle* handle = self->animHandle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->animLink);
        if (UnkWeakHandle* p = self->animHandle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->animHandle = nullptr;
        }
    }
    self->meshListeners[0].Detach();
    self->meshListeners[1].Detach();
    self->field_0x29f08 = 0;

    self->alerts.first.vf_0x20(false);
    self->alerts.second.vf_0x20(false);
    const UnkRecord* current = reinterpret_cast<UnkRecord*>(self->alerts.record);
    UnkRecord* saved = reinterpret_cast<UnkRecord*>(self->savedRecord);
    fn_710054715c(saved, current);
    self->alerts.recordTime = 0;
    self->vf_0x7d0();

    if (self->context->showsEffects) {
        UnkWorld* effects = self->owner->effects;
        fn_710114fe8c(effects, 0x7ff940855c402436, 0, 0);
        fn_710114fe8c(effects, 0xe83eac0c8c30776c, 0, 0);
        fn_710114fe8c(effects, 0xaf057066b81fccd4, 0, 0);
    }
    fn_7100705a64(self->timeline());
    fn_7100540dd0(reinterpret_cast<char*>(self));
    fn_7100d7820c(reinterpret_cast<char*>(self->owner), 0, 0, 1, 1);

    if (UnkRumble_l5452b4* rumble = self->rumble.Get()) {
        UnkStringData* path = g_unk_7101d470e8.data;
        if (path == &g_unk_7101d47200->empty || rumble->path.data != path) {
            fn_7100072328(rumble, 0.2f, 0.0f);
            // The null handle, declared as game/UnkTU50.h's UnkHandle.
            self->rumble = reinterpret_cast<const UnkRumbleHandle_l5452b4&>(g_unk_7101d586c0);
        }
    }

    fn_7100369710(self);
    if (self->central && self->central->unit)
        fn_71007717c0(self->central->unit);
    if (self->notifyOnStop) {
        self->notifyOnStop = false;
        UnkGameManagerView_l5452b4* manager = reinterpret_cast<UnkGameManagerView_l5452b4*>(g_unk_7101d47310);
        bool value = true;
        fn_71000303c0(&manager->onEmmyStop, &value);
    }
    fn_7100541fd4(reinterpret_cast<UnkEmmyAI_b60*>(self));
}
