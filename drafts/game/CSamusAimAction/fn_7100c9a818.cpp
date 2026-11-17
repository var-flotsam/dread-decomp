// CSamusAimAction's unit (tools/tu_map.py unit 2209): chooses the aim animation (aiming while firing,
// the other aim pose while the aim component holds, else the default one) and the blend time into it
// from the CCharClass*BlendTimeOverrideTrack tracks playing. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::global::CStrId;

struct UnkAimUpdate_b49;
void fn_710034a7b4(UnkAimUpdate_b49* aim_update, f32 value);
s32 fn_710034f544(UnkAimUpdate_b49* self);

// An aim animation entry (0x120 bytes): its name first.
struct UnkAimEntry_c9a818 {
    CStrId name;
    u8 pad_0x8[0x118];
};

// The action's animations, by name hash.
struct UnkAimEntries_c9a818 {
    u8 pad_0x0[0x80];
    UnkAimEntry_c9a818* entries;  // 0x80
    u64* keys;                    // 0x88
    u32 count;                    // 0x90

    UnkAimEntry_c9a818* End() { return entries + count; }
    UnkAimEntry_c9a818* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &entries[i];
        return End();
    }
    UnkAimEntry_c9a818* Get(u64 key) {
        UnkAimEntry_c9a818* entry = Find(key);
        return entry == End() ? nullptr : entry;
    }
};

// CSamusAnimationComponent as read here.
struct UnkSamusAnimation_c9a818 {
    virtual const base::reflection::CType* GetClass();
    u8 pad_0x8[0x528];
    s32 aim_state;     // 0x530
    bool was_aiming;   // 0x534
    bool aiming;       // 0x535
};

struct UnkAimInput_c9a818 {
    u8 pad_0x0[0x12a];
    bool fire;         // 0x12a
    f32 fire_amount;   // 0x12c
};

struct UnkAimMode_c9a818 {
    u8 pad_0x0[0x10];
    s32 mode;
};

// The aim component (CAimComponent) as read here.
struct UnkAimComponent_c9a818 {
    u8 pad_0x0[0x50];
    UnkAimMode_c9a818* mode;  // 0x50
    u8 pad_0x58[0x910];
    bool holding;             // 0x968
};

struct UnkAimOwner_c9a818 {
    u8 pad_0x0[0x220];
    UnkSamusAnimation_c9a818* animation;  // 0x220
    u8 pad_0x228[0x80];
    UnkAimInput_c9a818* input;            // 0x2a8
    UnkAimComponent_c9a818* aim;          // 0x2b0
};

struct UnkSamusAimAction_c9a818 {
    u8 pad_0x0[0x10];
    UnkAimEntries_c9a818* animations;  // 0x10
    UnkAimOwner_c9a818* owner;         // 0x18
    u8 pad_0x20[0x90];
    UnkAimEntry_c9a818* current;       // 0xb0
};

// A blend into an animation: the bone mask's key (~0 for none), the time, and whether it is set.
struct UnkAimBlend_c9a818 {
    u64 mask;
    f32 time;
    bool set;
};

// What the action is asked for, and what it answers.
struct UnkAimRequest_c9a818 {
    bool field_0x0;
    bool keep_blend;                  // 0x1
    UnkAimEntry_c9a818* previous;     // 0x8
    UnkAimEntry_c9a818* entry;        // 0x10
    UnkAimBlend_c9a818 blend;         // 0x18
};

// The default blend times.
struct UnkAimBlendTimes_c9a818 {
    u8 pad_0x0[0x24];
    f32 aim;          // 0x24
    f32 from_recoil;  // 0x28
    f32 to_aim;       // 0x2c
    f32 to_default;   // 0x30
};
extern UnkAimBlendTimes_c9a818 g_unk_717270e8d0;
__attribute__((visibility("hidden"))) extern const u64 g_unk_7101647910;  // the default bone mask

// The track record the blend time overrides resolve to.
struct UnkBlendTimeRecord_c9a818 {
    u8 pad_0x0[0x18];
    f32 time;
};

static const u64 kAimHash = 0x56FFAA4887B8E2E8;
static const u64 kHoldHash = 0x46CB3337BFED7326;

inline const CStrId& DefaultAimId_c9a818() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(s_table.pad_0xd88);  // the slot at 0xd88
}

// The time of the playing track of class `cls`, or `fallback` without one.
static inline const f32& OverrideTime(const UnkTrackQuery& query, const f32& fallback) {
    return query.result ? reinterpret_cast<const UnkBlendTimeRecord_c9a818*>(query.result)->time : fallback;
}

void fn_7100c9a818(UnkSamusAimAction_c9a818* self, UnkAimRequest_c9a818* request) {
    UnkAimOwner_c9a818* owner = self->owner;
    UnkSamusAnimation_c9a818* animation = nullptr;
    if (owner) {
        animation = owner->animation;
        if (animation && !fn_710009d50c(animation->GetClass(), fn_7100713bb0()))
            animation = nullptr;
    }
    owner = self->owner;
    UnkAimComponent_c9a818* aim = owner ? owner->aim : nullptr;

    UnkAimEntry_c9a818* entry = nullptr;
    s32 state = 0;
    bool aiming = false;
    if (self->animations->count != 0) {
        UnkAimInput_c9a818* input = owner ? owner->input : nullptr;
        if (aim) {
            bool fire = input && (input->fire || input->fire_amount > 0.0f);
            fn_710034a7b4(reinterpret_cast<UnkAimUpdate_b49*>(aim), 0.0f);
            state = fn_710034f544(reinterpret_cast<UnkAimUpdate_b49*>(aim));
            if (fire || state != 0 || (aim->mode && aim->mode->mode == 3) || aim->holding) {
                if (fire) {
                    entry = self->animations->Get(kAimHash);
                    aiming = true;
                } else {
                    entry = self->animations->Get(kHoldHash);
                }
            } else {
                state = 0;
            }
        }
    } else {
        entry = self->current;
    }
    if (animation) {
        animation->aim_state = entry ? state : 0;
        bool was_aiming = animation->aiming;
        animation->aiming = aiming;
        animation->was_aiming = was_aiming;
    }

    if (!entry)
        entry = self->animations->Get(DefaultAimId_c9a818().data->Key());

    if (aim && aim->holding) {
        request->blend = reinterpret_cast<const UnkAimBlend_c9a818&>(g_unk_710164b968);
    } else if (entry) {
        u64 no_recoil_mask = 0x424D34332A067652;
        bool mask = animation && !fn_710117b348(reinterpret_cast<UnkAnimationComponent*>(animation),
                                               fn_7100c32c18());
        if (!request->keep_blend) {
            if (entry->name.data->Key() == kAimHash) {
                const u64& key = mask ? no_recoil_mask : g_unk_7101647910;
                request->blend = UnkAimBlend_c9a818{key, g_unk_717270e8d0.aim, true};
            } else {
                bool from_aim = false;
                if (entry->name == DefaultAimId_c9a818()) {
                    from_aim = request->previous && request->previous->name.data->Key() == kAimHash;
                    if (!from_aim) {
                        UnkTrackQuery query = {fn_7100c531a8(), nullptr, -1.0f};
                        fn_710117b08c(reinterpret_cast<UnkAnimationComponent*>(self->owner->animation), &query);
                        request->blend = UnkAimBlend_c9a818{~0ull, OverrideTime(query, g_unk_717270e8d0.to_default),
                                                            true};
                    }
                } else if (entry->name.data->Key() == kHoldHash) {
                    from_aim = request->previous && request->previous->name.data->Key() == kAimHash;
                    if (!from_aim) {
                        UnkTrackQuery query = {fn_7100c52bb8(), nullptr, -1.0f};
                        fn_710117b08c(reinterpret_cast<UnkAnimationComponent*>(self->owner->animation), &query);
                        request->blend = UnkAimBlend_c9a818{~0ull, OverrideTime(query, g_unk_717270e8d0.to_aim),
                                                            true};
                    }
                }
                if (from_aim) {
                    UnkTrackQuery query = {fn_7100c1f838(), nullptr, -1.0f};
                    fn_710117b08c(reinterpret_cast<UnkAnimationComponent*>(self->owner->animation), &query);
                    if (query.result) {
                        request->blend = UnkAimBlend_c9a818{
                            ~0ull, reinterpret_cast<const UnkBlendTimeRecord_c9a818*>(query.result)->time, true};
                    } else {
                        const u64& key = mask ? no_recoil_mask : g_unk_7101647910;
                        request->blend = UnkAimBlend_c9a818{key, g_unk_717270e8d0.from_recoil, true};
                    }
                }
            }
        }
    }
    request->entry = entry;
}
