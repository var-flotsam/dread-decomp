// CGrabComponent unit (tools/tu_map.py unit 2709): tells the animation components of the grabber and
// of the grabbed actor about the grab (a mode-10 request under the grab's string ids, with the
// link flags both character classes allow), then marks them with the unit's "Grab" id.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "../../../src/game_5821e4/UnkStaticInit.h"

using base::global::CStrId;
using unk_static_init::HeaderIds;
using base::global::UnkStrIdBase;

// The unit's statics (built by fn_710113f730).
struct UnkStatics_710113f730 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    u8 pad_0x28[0x18];
    u8 field_0x40[8];
};
extern UnkStatics_710113f730 g_unk_7172a090e8 __attribute__((visibility("hidden")));

// A grab character class: the link flags at +0x90 (eight of them).
struct UnkGrabCharClass_l13c060 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x88];
    u32 link_flags;  // 0x90
};

struct UnkGrabOtherComponent_l13c060 {
    u8 pad_0x0[0x10];
    UnkGrabCharClass_l13c060* char_class;  // 0x10
};

struct UnkGrabActor_l13c060 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x38];
    UnkGrabOtherComponent_l13c060* grab;  // 0x260
};

struct UnkGrabHandle_l13c060 {
    u8 pad_0x0[0x10];
    UnkGrabActor_l13c060* target;  // 0x10
};

struct UnkGrabComponent_l13c060 {
    u8 pad_0x0[0x30];
    UnkGrabActor_l13c060* owner;  // 0x30
    u8 pad_0x38[0xb8];
    UnkGrabHandle_l13c060* grabbed;  // 0xf0
    u8 pad_0xf8[0x10];
    CStrId field_0x108;
    CStrId field_0x110;
    u8 pad_0x118[0xb0];
    UnkGrabCharClass_l13c060* char_class;  // 0x1c8
};

static inline UnkGrabCharClass_l13c060* AsGrabCharClass(UnkGrabCharClass_l13c060* obj) {
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->vf_0x0(), fn_7100790104()) ? obj : nullptr;
}

static inline bool HasText(const CStrId& id) {
    return id.data != &g_unk_7101d47108->empty;
}

// Eight link flags as bits.
struct UnkLinkFlags_l13c060 {
    u32 bits;

    bool Test(u32 i) const { return (bits >> i) & 1; }
    void Set(u32 i, bool value) {
        if (value)
            bits |= 1u << i;
        else
            bits &= ~(1u << i);
    }
};

void fn_710113c060(UnkGrabComponent_l13c060* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    UnkAnimationComponent* other_animation = nullptr;
    UnkGrabCharClass_l13c060* other_class = nullptr;
    if (self->grabbed && self->grabbed->target) {
        UnkGrabActor_l13c060* other = self->grabbed->target;
        other_animation = other->animation;
        if (other->grab)
            other_class = AsGrabCharClass(other->grab->char_class);
    }

    // The requests: mode 10, with the link flags as the mask.
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    base::reflection::UnkReflTypeIdArg other_mode;
    base::reflection::UnkReflTypeIdArg other_mask;

    if (animation) {
        if (HasText(self->field_0x108)) {
            UnkLinkFlags_l13c060 flags = {self->char_class->link_flags};
            if (other_class) {
                // Keep only the flags the grabbed actor's class allows too.
                UnkLinkFlags_l13c060 allowed = {other_class->link_flags};
                for (u32 i = 0; i < 8; ++i) {
                    if (flags.Test(i))
                        flags.Set(i, allowed.Test(i));
                    else
                        flags.Set(i, false);
                }
            }
            mode.id = 10;
            mask.id = flags.bits;
            CAnimationComponent_SendActionToTrack(animation, self->field_0x108.data->Key(), 0xf4061ff25a2163d6ull, &mode,
                          &g_unk_710164b958, &g_unk_7172769740, &mask);
        }
        fn_710117ab64(animation, true, &g_unk_7172a090e8.field_0x20);
    }

    if (other_animation && other_class) {
        const CStrId& id = HasText(self->field_0x110) ? self->field_0x110 : self->field_0x108;
        if (HasText(id)) {
            other_mode.id = 10;
            other_mask.id = other_class->link_flags;
            CAnimationComponent_SendActionToTrack(other_animation, id.data->Key(), 0xf4061ff25a2163d6ull, &other_mode,
                          &g_unk_710164b958, &g_unk_7172769740, &other_mask);
        }
        fn_710117ab64(other_animation, true, &g_unk_7172a090e8.field_0x20);
    }
}
