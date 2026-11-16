// CGrabComponent unit (tools/tu_map.py unit 2709): advances the grabber's and the grabbed actor's
// animations by `time`; when both are in their grab tracks and the link mode at +0x128 asks for it,
// the grabbed one's track is synchronised with the grabber's ("GrabSlaveAnimSync") first.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CGrabComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;
using unk_static_init::HeaderIds;

// The unit's statics (built by fn_710113f730).
struct UnkStatics_710113f730 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x18];
    u8 field_0x40[8];
};

void fn_71011781f4(UnkAnimationComponent* self, u64 key, bool flag);
void fn_7101178274(UnkAnimationComponent* self, u64 key);

struct UnkGrabActor_l13dbb4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkGrabLink_l13dbb4 {
    u8 pad_0x0[0x10];
    UnkGrabActor_l13dbb4* target;  // 0x10
};

struct UnkGrabComponent_l13dbb4 {
    u8 pad_0x0[0x30];
    UnkGrabActor_l13dbb4* owner;  // 0x30
    u8 pad_0x38[0x98];
    UnkGrabLink_l13dbb4* field_0xd0;
    u8 pad_0xd8[0x50];
    s32 link_mode;  // 0x128
};
static_assert(__builtin_offsetof(UnkGrabComponent_l13dbb4, link_mode) == 0x128, "layout");

// The animation component's two update slots.
static inline void Update(UnkAnimationComponent* animation, f32 time) {
    void** vtable = *reinterpret_cast<void***>(animation);
    reinterpret_cast<void (*)(UnkAnimationComponent*, f32)>(vtable[0x98 / 8])(animation, time);
    vtable = *reinterpret_cast<void***>(animation);
    reinterpret_cast<void (*)(UnkAnimationComponent*, f32)>(vtable[0xa8 / 8])(animation, time);
}

void fn_710113dbb4(UnkGrabComponent_l13dbb4* self, UnkGrabActor_l13dbb4* grabber, UnkGrabActor_l13dbb4* grabbed,
                   f32 time) {
    UnkAnimationComponent* grabber_anim = grabber->animation;
    if (grabber_anim) {
        fn_710117ab64(grabber_anim, false, &g_unk_7172a090e8.field_0x20);
        Update(grabber_anim, time);
        fn_710117ab64(grabber_anim, true, &g_unk_7172a090e8.field_0x20);
    }

    UnkAnimationComponent* grabbed_anim = grabbed->animation;
    if (!grabbed_anim)
        return;
    fn_710117ab64(grabbed_anim, false, &g_unk_7172a090e8.field_0x20);

    bool sync = false;
    const u64 kind = 0xf4061ff25a2163d6ull;
    if (grabber_anim && self->field_0xd0 && self->field_0xd0->target) {
        UnkAnimationComponent* target_anim = self->field_0xd0->target->animation;
        UnkAnimationComponent* own_anim = self->owner->animation;
        if ((!own_anim || fn_710117b348(own_anim, fn_7100cff148())) &&
            (!target_anim || fn_710117b348(target_anim, fn_7100cfe868()))) {
            u64 track = fn_7101177ae8(grabber_anim, kind)->data->Key();
            u64 grabbed_track = fn_7101177ae8(grabbed_anim, kind)->data->Key();
            switch (self->link_mode) {
            case 0:
                if (track == grabbed_track) {
                    sync = true;
                } else {
                    // Ask the grabbed animation to play the grabber's track.
                    base::reflection::UnkReflTypeIdArg request;
                    request.id = 2;
                    base::reflection::UnkReflTypeIdArg mask;
                    mask.id = UnkAll_SpitSend().value;
                    sync = fn_7101176690(grabbed_anim, track, kind, &request, &g_unk_710164b958, &g_unk_7172769740,
                                         &mask);
                }
                break;
            case 1:
                sync = track == grabbed_track;
                break;
            case 2:
                sync = true;
                break;
            }
        }
    }

    if (sync) {
        fn_7101178274(grabbed_anim, kind);
        fn_710117808c(grabber_anim, kind);
        fn_71011781f4(grabbed_anim, kind, true);
        static CStrId s_sync("GrabSlaveAnimSync", true);
        fn_710117abfc(grabbed_anim, true, &s_sync, kind);
        Update(grabbed_anim, time);
        fn_710117abfc(grabbed_anim, false, &s_sync, kind);
    } else {
        Update(grabbed_anim, time);
    }
    fn_710117ab64(grabbed_anim, true, &g_unk_7172a090e8.field_0x20);
}
