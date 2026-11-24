// UnkTU437's unit (tools/tu_map.py unit 437, CAimComponent): binds the aim to its owner. Asks the
// owner's animation (+0x220) for six tracks (keys from the reflection singleton), moves the input
// listener (+0x2e0) to the owner's input (+0x1d8), and hooks the links at +0x530 and +0x550 up to the
// owner's gun (+0x2a8) and animation. Names and layouts are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CGunComponent.h"
#include "game/CInputComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflStatic;

struct UnkAimTrackDesc_l344190;
__attribute__((visibility("hidden"))) extern UnkAimTrackDesc_l344190 g_unk_71726b1740;
__attribute__((visibility("hidden"))) extern UnkAimTrackDesc_l344190 g_unk_71726b174c;
__attribute__((visibility("hidden"))) extern UnkAimTrackDesc_l344190 g_unk_71726b1758;
__attribute__((visibility("hidden"))) extern UnkAimTrackDesc_l344190 g_unk_71726b1764;
__attribute__((visibility("hidden"))) extern UnkAimTrackDesc_l344190 g_unk_71726b1770;
__attribute__((visibility("hidden"))) extern UnkAimTrackDesc_l344190 g_unk_71726b177c;

// Asks an animation for a track under `key` (of kind `kind`), described by `desc`.
void fn_710117b598(void* animation, u64 key, u64 kind, const UnkAimTrackDesc_l344190* desc, bool flag);
void fn_710083b0b4(void*, void*);
void CAbilityOpticCamouflage_GetWeakHandle(UnkWeakTarget* self, UnkWeakHandle** out);

// The string ids in the reflection singleton that name the tracks and their kind.
struct UnkAimTrackKind_l344190;
struct UnkAimTrackA_l344190;
struct UnkAimTrackB_l344190;
struct UnkAimTrackC_l344190;
struct UnkAimTrackD_l344190;
struct UnkAimTrackE_l344190;
template <>
struct base::reflection::UnkReflStatic<UnkAimTrackKind_l344190> {
    static constexpr u32 offset = 0xd30;
};
template <>
struct base::reflection::UnkReflStatic<UnkAimTrackA_l344190> {
    static constexpr u32 offset = 0x1d90;
};
template <>
struct base::reflection::UnkReflStatic<UnkAimTrackB_l344190> {
    static constexpr u32 offset = 0xcb8;
};
template <>
struct base::reflection::UnkReflStatic<UnkAimTrackC_l344190> {
    static constexpr u32 offset = 0xfe0;
};
template <>
struct base::reflection::UnkReflStatic<UnkAimTrackD_l344190> {
    static constexpr u32 offset = 0x12e8;
};
template <>
struct base::reflection::UnkReflStatic<UnkAimTrackE_l344190> {
    static constexpr u32 offset = 0x17c0;
};

template <class G>
static inline u64 SingletonKey_l344190() {
    return static_cast<base::global::CStrId*>(base::reflection::refl_GetSingletonPart<G>())->data->Key();
}

// A listener node and the weak handle of the object it is hooked up to.
struct UnkAimLink_l344190 {
    u8 node[0x18];
    UnkWeakHandle* handle;  // 0x18

    UnkWeakTarget* Target() const { return handle ? handle->target : nullptr; }

    // Drops the handle; `unhook` first takes the node off the old target.
    template <void (*Unhook)(UnkWeakTarget*, void*)>
    void Release() {
        if (UnkWeakHandle* h = handle) {
            if (UnkWeakTarget* target = h->target)
                Unhook(target, node);
            ResetRef(&handle);
        }
    }
};

static inline void UnhookGun_l344190(UnkWeakTarget* target, void* node) {
    fn_710083b0bc(target, static_cast<UnkListener_b40*>(node));
}

struct UnkAimOwner_l344190 {
    u8 pad_0x0[0x1d8];
    UnkEnergyWaveTargetComp_b49* input;  // 0x1d8
    u8 pad_0x1e0[0x220 - 0x1e0];
    UnkWeakTarget* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkWeakTarget* gun;  // 0x2a8
};

struct UnkAimBind_l344190 {
    u8 pad_0x0[0x30];
    UnkAimOwner_l344190* owner;  // 0x30
    u8 pad_0x38[0x2e0 - 0x38];
    u8 input_listener[0x48];        // 0x2e0
    UnkAimBind_l344190* listening;  // 0x328: the component whose owner's input has the listener
    u8 pad_0x330[0x530 - 0x330];
    UnkAimLink_l344190 gun_link;        // 0x530
    UnkAimLink_l344190 animation_link;  // 0x550
    u8 pad_0x570[0x848 - 0x570];
    u32 field_0x848;
};
static_assert(__builtin_offsetof(UnkAimBind_l344190, field_0x848) == 0x848, "layout");

void fn_7100344190(UnkAimBind_l344190* self) {
    self->field_0x848 = 0;
    if (self->owner->animation) {
        using Kind = UnkAimTrackKind_l344190;
        fn_710117b598(self->owner->animation, SingletonKey_l344190<UnkAimTrackA_l344190>(),
                      SingletonKey_l344190<Kind>(), &g_unk_71726b1740, false);
        fn_710117b598(self->owner->animation, SingletonKey_l344190<UnkAimTrackB_l344190>(),
                      SingletonKey_l344190<Kind>(), &g_unk_71726b174c, false);
        fn_710117b598(self->owner->animation, SingletonKey_l344190<UnkAimTrackC_l344190>(),
                      SingletonKey_l344190<Kind>(), &g_unk_71726b1758, false);
        fn_710117b598(self->owner->animation, SingletonKey_l344190<UnkAimTrackD_l344190>(),
                      SingletonKey_l344190<Kind>(), &g_unk_71726b1764, false);
        fn_710117b598(self->owner->animation, SingletonKey_l344190<UnkAimTrackE_l344190>(),
                      SingletonKey_l344190<Kind>(), &g_unk_71726b1770, false);
        fn_710117b598(self->owner->animation, 0x548ed9651a046f0b, SingletonKey_l344190<Kind>(),
                      &g_unk_71726b177c, false);
    }

    void* listener = self->input_listener;
    if (UnkAimBind_l344190* old = self->listening) {
        if (UnkAimOwner_l344190* owner = old->owner) {
            if (UnkEnergyWaveTargetComp_b49* input = owner->input)
                fn_7100872908(input, listener);
        }
        self->listening = nullptr;
    }
    if (UnkAimOwner_l344190* owner = self->owner) {
        if (UnkEnergyWaveTargetComp_b49* input = owner->input)
            fn_7100872860(input, listener);
    }
    self->listening = self;

    if (UnkWeakTarget* gun = self->owner->gun) {
        self->gun_link.Release<UnhookGun_l344190>();
        CAbilityOpticCamouflage_GetWeakHandle(gun, &self->gun_link.handle);
        fn_710083b0b4(self->gun_link.Target(), self->gun_link.node);
    }
    if (UnkWeakTarget* animation = self->owner->animation) {
        self->animation_link.Release<fn_710117b3f0>();
        fn_7100316018(animation, &self->animation_link.handle);
        fn_710117b3e8(self->animation_link.Target(), self->animation_link.node);
    }
}
