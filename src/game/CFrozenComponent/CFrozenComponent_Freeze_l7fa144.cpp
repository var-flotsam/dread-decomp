// CFrozenComponent's unit (tools/tu_map.py unit 1260): starts the frozen animation. With a freeze-in-frame
// track playing it only blends the frozen track in; otherwise it plays the tweak-freeze action request,
// filled in by the track, by the component's filter or from the char class's id. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7172721120[] __attribute__((visibility("hidden")));
extern char g_unk_7172721128[] __attribute__((visibility("hidden")));

struct UnkFreezeDesc_l7fa144 {
    u64 words[2];
};

struct UnkFreezeBlend_l7fa144 {
    u64 kind;
    u32 value;
};

struct UnkFreezeRequest_l7fa144 {
    s64 id;
    u64 desc[2];     // 0x8
    u64 blend_kind;  // 0x18
    u32 blend_value; // 0x20
    u32 flags;       // 0x24
    u32 mask;        // 0x28
};

// The track that fills the request in.
struct UnkFreezeTrack_l7fa144 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkFreezeRequest_l7fa144* request);
};

struct UnkFreezeCharClass_l7fa144 {
    u8 pad_0x0[0x78];
    CStrId freeze_id;  // 0x78
};

struct UnkFreezeOwner_l7fa144 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkFrozenComponent_l7fa144 {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkFreezeOwner_l7fa144* owner;  // 0x30
    u8 pad_0x38[0x2d8 - 0x38];
    UnkFreezeCharClass_l7fa144* char_class;  // 0x2d8
    u8 pad_0x2e0[0x378 - 0x2e0];
    void* filter_object;                                             // 0x378
    bool (*filter)(void* object, UnkFreezeRequest_l7fa144* request);  // 0x380

    bool GetTrackIds(CStrId* track, CStrId* blend) {
        typedef bool (*Fn)(UnkFrozenComponent_l7fa144*, CStrId*, CStrId*);
        return (*reinterpret_cast<Fn**>(this))[0x1f8 / 8](this, track, blend);
    }
};
static_assert(__builtin_offsetof(UnkFrozenComponent_l7fa144, char_class) == 0x2d8, "layout");
static_assert(__builtin_offsetof(UnkFrozenComponent_l7fa144, filter) == 0x380, "layout");

// The key of the empty id.
static constexpr u64 kEmptyKey = 0xf4061ff25a2163d6ull;

inline bool IsEmpty_l7fa144(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

// Blends the layer named `layer` to the empty track, then to `track` when there is one.
inline void Blend_l7fa144(UnkAnimationComponent* animation, char* layer, const CStrId& track) {
    fn_710117abfc(animation, true, layer, kEmptyKey);
    if (!IsEmpty_l7fa144(track))
        fn_710117abfc(animation, true, layer, track.data->Key());
}

struct UnkFrozenComponent;

// `time` is never read: the declaration in CFrozenComponent.cpp passes it.
void fn_71007fa144(UnkFrozenComponent* component, f32 time) {
    UnkFrozenComponent_l7fa144* self = reinterpret_cast<UnkFrozenComponent_l7fa144*>(component);
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation || !self->char_class)
        return;
    CStrId track = g_unk_7101d470e0;
    CStrId blend = track;
    self->GetTrackIds(&track, &blend);
    if (fn_710117b348(animation, fn_7100cfb828())) {
        Blend_l7fa144(animation, g_unk_7172721128, track);
        return;
    }

    UnkFreezeRequest_l7fa144 request;
    const UnkFreezeDesc_l7fa144* desc = reinterpret_cast<const UnkFreezeDesc_l7fa144*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkFreezeBlend_l7fa144* blend_desc = reinterpret_cast<const UnkFreezeBlend_l7fa144*>(&g_unk_7172769740);
    u64 kind = blend_desc->kind;
    u32 value = blend_desc->value;
    u32 mask = UnkAll_SpitSend().value;
    request.id = -1;
    request.desc[0] = desc0;
    request.desc[1] = desc1;
    request.blend_kind = kind;
    request.blend_value = value;
    request.flags = 0;
    request.mask = mask;

    UnkTrackQuery query = {fn_7100c58af8(), nullptr, -1.0f};
    fn_710117b08c(self->owner->animation, &query);
    if (query.result) {
        reinterpret_cast<UnkFreezeTrack_l7fa144*>(query.result)->vf_0x18(&request);
    } else if (!self->filter || !self->filter(self->filter_object, &request)) {
        request.id = self->char_class->freeze_id.data->Key();
        if (request.id == -1) {
            Blend_l7fa144(animation, g_unk_7172721120, track);
            return;
        }
    }
    UnkStruct_7101176ea8* player = reinterpret_cast<UnkStruct_7101176ea8*>(animation);
    const UnkRequest_7101176ea8* play = reinterpret_cast<const UnkRequest_7101176ea8*>(&request);
    if (!IsEmpty_l7fa144(track))
        fn_7101176ea8(player, play, track.data->Key());
    else
        fn_7101176ea8(player, play, kEmptyKey);
}
