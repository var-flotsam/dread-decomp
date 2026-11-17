// CSecondaryGun's unit (tools/tu_map.py unit 1316): plays the gun's timeline entry (+0x50) under the source
// id the owner's CCharClassAddWeaponTimelineSuffixTrack names, or the power bomb's entry when the player
// stands in an unfrozen lava pool. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassBaseTriggerComponent.h"
#include "game/CCharClassLavaPoolComponent.h"
#include "game/CGunComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CTimelineComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkPlayer;
struct UnkGrappleAnswer_b50;
struct UnkMagmaArea_b50;

__attribute__((visibility("hidden"))) extern const u64 g_unk_710164adc0;
__attribute__((visibility("hidden"))) extern const u64 g_unk_7172728450;
__attribute__((visibility("hidden"))) extern const u64 g_unk_7172728458;

struct UnkSecondaryGunPlayer_l85c958 {
    u8 pad_0x0[0x228];
    void* field_0x228;
};

struct UnkSecondaryGunLavaEntry_l85c958 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkSecondaryGunLavaList_l85c958 {
    UnkSecondaryGunLavaEntry_l85c958** data;
    u32 count;
};

struct UnkSecondaryGunLavaPool_l85c958 {
    u8 pad_0x0[0xcb1];
    bool frozen;  // 0xcb1
};

struct UnkSecondaryGunOwner_l85c958 {
    u8 pad_0x0[0x220];
    void* field_0x220;  // the character class
    u8 pad_0x228[0x80];
    UnkStruct_710083e9d4* guns;  // 0x2a8
    u8 pad_0x2b0[0x38];
    void* field_0x2e8;  // the timeline
};
static_assert(__builtin_offsetof(UnkSecondaryGunOwner_l85c958, field_0x2e8) == 0x2e8, "owner layout");

struct UnkSecondaryGunItem_l85c958 {
    u8 pad_0x0[0x50];
    CStrId entry;  // 0x50
};

struct UnkSecondaryGun_l85c958 {
    u8 pad_0x0[0x18];
    UnkSecondaryGunOwner_l85c958* owner;  // 0x18
    u8 pad_0x20[0x8];
    u64 field_0x28;
    u8 pad_0x30[0x20];
    CStrId entry;  // 0x50
};

struct UnkSecondaryGunArg_l85c958 {
    u32 mode;
    bool flag;  // 0x4
};

void fn_710085c958(UnkSecondaryGun_l85c958* self, const UnkSecondaryGunArg_l85c958* arg) {
    CStrId source(g_unk_7101d470e0);
    UnkTrackQuery query;
    query.cls = fn_7100beff48();
    query.result = nullptr;
    query.value = -1.0f;
    if (fn_710117b08c(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), &query) && query.result)
        source = query.result->sources;
    if (self->entry.data == &g_unk_7101d47108->empty)
        return;
    if (!arg->flag) {
        auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
        if (!timeline)
            return;
        {
            u64 key = self->entry.data->Key();
            if (!fn_7100a2c13c(timeline, &key))
                return;
        }
        {
            u64 key = self->entry.data->Key();
            fn_7100a2b7b8(timeline, &key);
        }
        // In an unfrozen lava pool, the power bomb's entry plays instead.
        if (self->field_0x28 == g_unk_7172728450) {
            bool inLava = false;
            {
                auto* player = reinterpret_cast<UnkSecondaryGunPlayer_l85c958*>(fn_7100dec788(g_unk_7101d47310));
                CVector3D pos = g_unk_710164779c;
                fn_710117364c(player->field_0x228, &g_unk_710164adc0, reinterpret_cast<UnkGrappleAnswer_b50*>(&pos));
                auto* list = reinterpret_cast<UnkSecondaryGunLavaList_l85c958*>(fn_71008fbdb4());
                for (UnkSecondaryGunLavaEntry_l85c958 **it = list->data, **end = list->data + list->count; it != end; ++it) {
                    UnkSecondaryGunLavaEntry_l85c958* lava = *it;
                    UnkReflObject* component = lava ? lava->component : nullptr;
                    UnkSecondaryGunLavaPool_l85c958* pool = nullptr;
                    if (component)
                        pool = fn_710009d50c(component->vf_0x0(), fn_71008fb79c())
                                   ? reinterpret_cast<UnkSecondaryGunLavaPool_l85c958*>(component)
                                   : nullptr;
                    if (pool->frozen)
                        continue;
                    CVector2D point;
                    fn_710007b430(&point, &pos);
                    if (fn_7100a47e00(reinterpret_cast<UnkMagmaArea_b50*>(pool), &point)) {
                        inLava = true;
                        break;
                    }
                }
            }
            // In an unfrozen lava pool, the power bomb's entry plays instead.
            if (inLava) {
                UnkStruct_710083e9d4* guns = self->owner->guns;
                if (!guns)
                    return;
                auto* item = reinterpret_cast<UnkSecondaryGunItem_l85c958*>(
                    fn_710083e9d4(guns, &g_unk_7172728458, false));
                if (!item)
                    return;
                u64 key = item->entry.data->Key();
                fn_7100a2b7b8(timeline, &key);
                fn_7100a2cddc(timeline, &item->entry, &source, -1.0f, false);
                return;
            }
        }
        fn_7100a2cddc(timeline, &self->entry, &source, -1.0f, false);
    }
}
