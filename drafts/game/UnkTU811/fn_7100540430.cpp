// UnkTU811 (CEmmyAIComponent's unit): sets the E.M.M.I.'s AI up after the base step: resets its members
// and switches, names the current area "default", sets a model flag, watches the owner's life part, takes
// its ranges from the tunables and puts 200 into the animation's bag. Names and layouts are placeholders.
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/global/CStrId.h"
#include "game/CCharClassTweakPlayRateTrack.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU794.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;

struct UnkEmmyCaveAI_b49;
struct UnkEmmyActor_afc;
struct UnkEmmyAI_b0194;
struct UnkStepper;
struct UnkHeatableShieldMap_A5_b66;
void fn_7100540810(UnkEmmyActor_afc* self, bool flag);
f32 fn_71005414c8(UnkEmmyAI_b0194* self);
bool fn_71005416bc(UnkStepper* self, bool flag);
void fn_7100565d38(UnkTutoHandle* handle, UnkTutoHandle** slot);
void fn_710086c7a8(UnkHeatableShieldMap_A5_b66* self, const u64* key, bool value);
extern char g_unk_71726e1f90[] __attribute__((visibility("hidden")));
extern char g_unk_71726e2008[] __attribute__((visibility("hidden")));  // the bag key
void fn_7100565eec();

struct UnkEmmyMember_l540430;
void fn_7100500620(UnkEmmyMember_l540430* self);
void fn_71004f35c4(UnkEmmyMember_l540430* self);
void fn_7100705288(UnkEmmyMember_l540430* self);
struct UnkEmmyAI_l540430;
void fn_7100540ad4(UnkEmmyAI_l540430* self);

// The tweak object the E.M.M.I.s share.
struct UnkEmmyTweak_l540430 {
    u8 pad_0x0[0x94];
    f32 field_0x94;
};

struct UnkTunables_l540430 {
    u8 pad_0x0[0x1e0];
    f32 nearRange;  // 0x1e0
    f32 farRange;   // 0x1e4
    u32 field_0x1e8;
    u32 field_0x1ec;
};

struct UnkTunableSource_l540430 {
    void** vtable;

    UnkTunables_l540430* Tunables() {
        return reinterpret_cast<UnkTunables_l540430* (*)(UnkTunableSource_l540430*)>(vtable[0xd8 / 8])(this);
    }
};

struct UnkSwitch_l540430 {
    void** vtable;

    void Reset() { reinterpret_cast<void (*)(UnkSwitch_l540430*)>(vtable[0x10 / 8])(this); }
};

struct UnkTimedSwitch_l540430 {
    UnkSwitch_l540430 base;
    u8 pad_0x8[0x2c];
    bool enabled;  // 0x34
    u8 pad_0x35[0x3b];
};

struct UnkSwitches_l540430 {
    UnkSwitch_l540430 first;
    u8 pad_0x8[0xf8];
    UnkSwitch_l540430 second;  // 0x100
    u8 pad_0x108[0xf8];
};

struct UnkTimedSwitches_l540430 {
    UnkTimedSwitch_l540430 first;
    UnkTimedSwitch_l540430 second;  // 0x70
    UnkSwitch_l540430 third;        // 0xe0
    u8 pad_0xe8[0x68];
};

struct UnkSwitchState_l540430 {
    UnkSwitch_l540430 base;
    u8 pad_0x8[0x134];
    f32 field_0x13c;  // 0x13c
    u8 pad_0x140[0x28];
    u64 field_0x168;  // 0x168
};

// The owner's life part as this unit sees it: four lists to watch and the callback it runs.
struct UnkLifePart_l540430 {
    u8 pad_0x0[0x68];
    struct {
        UnkTutoHandle* list;
        u8 pad_0x8[0x78];
    } lists[4];                  // 0x68
    void* owner;                 // 0x268
    void (*callback)();          // 0x270

    UnkTutoHandle* List(u32 i) { return reinterpret_cast<UnkTutoHandle*>(&lists[i]); }
};

// A watch on one of the life part's lists.
struct UnkLifeWatch_l540430 {
    UnkTutoHandle* handle;
    u8 pad_0x8[0x10];

    // Leaves the list it watches.
    void Leave() {
        if (handle) {
            fn_7100565ef0(handle, &handle);
            handle = nullptr;
        }
    }
};
static_assert(__builtin_offsetof(UnkLifePart_l540430, owner) == 0x268, "UnkLifePart_l540430 layout");

struct UnkOwner_l540430 {
    u8 pad_0x0[0x220];
    char* animation;         // 0x220: its +0x80 is the animation's property bag
    UnkFlagTarget* model;    // 0x228
};

struct UnkEmmyAI_l540430 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l540430* owner;  // 0x30
    u8 pad_0x38[0x14780 - 0x38];
    UnkSwitches_l540430 field_0x14780;
    UnkTimedSwitches_l540430 field_0x14980;
    UnkSwitchState_l540430 field_0x14ad0;
    u8 pad_0x14c40[0x29ee0 - 0x14c40];
    UnkTunableSource_l540430* tunables;  // 0x29ee0
    u8 pad_0x29ee8[0x8];
    u32 grabMode;                        // 0x29ef0
    u8 pad_0x29ef4[0x3c];
    UnkLifeWatch_l540430 lifeWatches[4];  // 0x29f30
    u8 pad_0x29f90[0x2a190 - 0x29f90];
    u32 field_0x2a190;
    u32 field_0x2a194;
    f32 nearRangeSq;                     // 0x2a198
    f32 farRangeSq;                      // 0x2a19c
    u8 pad_0x2a1a0[0x30];
    UnkEmmyMember_l540430* field_0x2a1d0() { return reinterpret_cast<UnkEmmyMember_l540430*>(member_0x2a1d0); }
    UnkEmmyMember_l540430* field_0x2a218() { return reinterpret_cast<UnkEmmyMember_l540430*>(member_0x2a218); }
    UnkEmmyMember_l540430* field_0x2a268() { return reinterpret_cast<UnkEmmyMember_l540430*>(member_0x2a268); }
    u8 member_0x2a1d0[0x48];             // 0x2a1d0
    u8 member_0x2a218[0x50];             // 0x2a218
    u8 member_0x2a268[0x108];            // 0x2a268
    f32 field_0x2a370;                   // 0x2a370
    u8 pad_0x2a374[0x2a60c - 0x2a374];
    bool field_0x2a60c;
    u8 pad_0x2a60d;
    bool field_0x2a60e;

    void vf_0x850() { reinterpret_cast<void (*)(UnkEmmyAI_l540430*)>(vtable[0x850 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyAI_l540430, field_0x14ad0) == 0x14ad0, "UnkEmmyAI_l540430 layout");
static_assert(sizeof(UnkTimedSwitches_l540430) == 0x150, "UnkTimedSwitches_l540430 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l540430, lifeWatches) == 0x29f30, "UnkEmmyAI_l540430 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l540430, field_0x2a190) == 0x2a190, "UnkEmmyAI_l540430 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l540430, field_0x2a370) == 0x2a370, "UnkEmmyAI_l540430 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l540430, field_0x2a60e) == 0x2a60e, "UnkEmmyAI_l540430 layout");


// game/CEmmyCaveAIComponent/CEmmyCaveAIComponent_Blackboard_b49.cpp declares this with its own
// UnkEmmyCaveAI_b49 (the same object).
void fn_7100540430(UnkEmmyAI_l540430* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    fn_7100540810(reinterpret_cast<UnkEmmyActor_afc*>(self), false);
    fn_7100500620(self->field_0x2a1d0());
    fn_71004f35c4(self->field_0x2a218());
    fn_7100705288(self->field_0x2a268());
    PassEmmyBarrierStrIdToManager(reinterpret_cast<UnkEmmyAI2_b57a3*>(self));
    fn_7100540ad4(self);

    self->field_0x14780.first.Reset();
    self->field_0x14780.second.Reset();
    self->field_0x14980.first.base.Reset();
    self->field_0x14980.second.base.Reset();
    self->field_0x14980.third.Reset();
    self->field_0x14ad0.base.Reset();
    self->field_0x14980.second.enabled = true;
    self->field_0x14980.first.enabled = true;
    fn_7100540dd0(reinterpret_cast<char*>(self));
    self->field_0x14ad0.field_0x13c = fn_71005414c8(reinterpret_cast<UnkEmmyAI_b0194*>(self));

    static const CStrId s_default("default", true);
    g_unk_7101d47310->field_0x2230 = s_default;

    u64 flag = 0x4a4b2869cf501b40;
    fn_7101172780(self->owner->model, reinterpret_cast<const char*>(&flag));

    if (UnkLifePart_l540430* part = reinterpret_cast<UnkLifePart_l540430*>(
            fn_71005012bc(reinterpret_cast<UnkDoorLifeOwner_b56*>(self->owner)))) {
        self->lifeWatches[0].Leave();
        self->lifeWatches[0].handle = part->List(0);
        fn_7100565d38(self->lifeWatches[0].handle, &self->lifeWatches[0].handle);
        self->lifeWatches[1].Leave();
        self->lifeWatches[1].handle = part->List(1);
        fn_7100565d38(self->lifeWatches[1].handle, &self->lifeWatches[1].handle);
        self->lifeWatches[2].Leave();
        self->lifeWatches[2].handle = part->List(2);
        fn_7100565d38(self->lifeWatches[2].handle, &self->lifeWatches[2].handle);
        self->lifeWatches[3].Leave();
        self->lifeWatches[3].handle = part->List(3);
        fn_7100565d38(self->lifeWatches[3].handle, &self->lifeWatches[3].handle);
        part->owner = self;
        part->callback = fn_7100565eec;
        if (self->grabMode) {
            self->grabMode = 0;
            self->vf_0x850();
        }
        fn_710086c7a8(reinterpret_cast<UnkHeatableShieldMap_A5_b66*>(part),
                      reinterpret_cast<const u64*>(g_unk_71726e1f90), true);
    }

    fn_71005416bc(reinterpret_cast<UnkStepper*>(self), true);
    self->field_0x14ad0.field_0x168 = 0;
    self->field_0x2a60c = false;
    self->field_0x2a60e = false;

    f32 nearRange = self->tunables->Tunables()->nearRange;
    f32 farRange = self->tunables->Tunables()->farRange;
    u32 b = self->tunables->Tunables()->field_0x1ec;
    self->field_0x2a190 = self->tunables->Tunables()->field_0x1e8;
    self->nearRangeSq = nearRange * nearRange;
    self->farRangeSq = farRange * farRange;
    self->field_0x2a194 = b;

    if (self->owner->animation) {
        self->field_0x2a370 = 200.0f;
        unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(self->owner->animation + 0x80),
                                    *reinterpret_cast<const CStrId*>(g_unk_71726e2008), 200.0f);
        reinterpret_cast<UnkEmmyTweak_l540430*>(&g_unk_7172790a08)->field_0x94 = 1.54f;
    }
}
