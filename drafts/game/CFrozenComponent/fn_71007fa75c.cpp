// CFrozenComponent's unit (tools/tu_map.py unit 1260): starts the unfreeze. It stops the frozen track, and
// unless a freeze-in-frame track plays, plays the tweak-unfreeze action request (filled in by the track, by
// the component's filter or from the char class's ids), first starting the thaw blend layer's fade in.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::UnkReflObject;

extern char g_unk_7172721120[] __attribute__((visibility("hidden")));
extern char g_unk_7172721128[] __attribute__((visibility("hidden")));

struct UnkUnfreezeDesc_l7fa75c {
    u64 words[2];
};

struct UnkUnfreezeBlend_l7fa75c {
    u64 kind;
    u32 value;
};

struct UnkUnfreezeRequest_l7fa75c {
    s64 id;
    u64 desc[2];     // 0x8
    u64 blend_kind;  // 0x18
    u32 blend_value; // 0x20
    u32 flags;       // 0x24
    u32 mask;        // 0x28
};

// The track that fills the request in.
struct UnkUnfreezeTrack_l7fa75c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkUnfreezeRequest_l7fa75c* request);
};

// The animation's tree nodes by name (parallel key and value arrays).
struct UnkUnfreezeNodeMap_l7fa75c {
    u8 pad_0x0[0x78];
    UnkReflObject** values;  // 0x78
    UnkStringData** keys;    // 0x80
    u32 count;               // 0x88
};

static inline UnkReflObject** FindNode_l7fa75c(UnkUnfreezeNodeMap_l7fa75c* map, UnkStringData* const& key) {
    for (u32 i = 0; i < map->count; ++i)
        if (map->keys[i] == key)
            return &map->values[i];
    return &map->values[map->count];
}

struct UnkUnfreezeCharClass_l7fa75c {
    u8 pad_0x0[0x80];
    CStrId unfreeze_id;         // 0x80
    CStrId unfreeze_moving_id;  // 0x88
};

struct UnkUnfreezeOwner_l7fa75c {
    u8 pad_0x0[0x218];
    void* movement;                    // 0x218
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkFrozenComponent_l7fa75c {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkUnfreezeOwner_l7fa75c* owner;  // 0x30
    u8 pad_0x38[0x2d8 - 0x38];
    UnkUnfreezeCharClass_l7fa75c* char_class;  // 0x2d8
    u8 pad_0x2e0[0x388 - 0x2e0];
    void* filter_object;                                               // 0x388
    bool (*filter)(void* object, UnkUnfreezeRequest_l7fa75c* request);  // 0x390
    bool playing;   // 0x398
    f32 value;      // 0x39c
    f32 start;      // 0x3a0
    f32 target;     // 0x3a4
    f32 duration;   // 0x3a8
    f32 time;       // 0x3ac

    bool GetTrackIds(CStrId* track, CStrId* blend) {
        typedef bool (*Fn)(UnkFrozenComponent_l7fa75c*, CStrId*, CStrId*);
        return (*reinterpret_cast<Fn**>(this))[0x1f8 / 8](this, track, blend);
    }
};
static_assert(__builtin_offsetof(UnkFrozenComponent_l7fa75c, char_class) == 0x2d8, "layout");
static_assert(__builtin_offsetof(UnkFrozenComponent_l7fa75c, filter) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkFrozenComponent_l7fa75c, value) == 0x39c, "layout");

// The key of the empty id.
static constexpr u64 kEmptyKey = 0xf4061ff25a2163d6ull;

inline bool IsEmpty_l7fa75c(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

inline u64 Key_l7fa75c(const CStrId& id) {
    return IsEmpty_l7fa75c(id) ? kEmptyKey : id.data->Key();
}

void fn_71007fa75c(UnkFrozenComponent_l7fa75c* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation || !self->char_class)
        return;
    void* movement = self->owner->movement;
    CStrId track = g_unk_7101d470e0;
    CStrId blend = track;
    self->GetTrackIds(&track, &blend);
    fn_710117abfc(animation, false, g_unk_7172721120, Key_l7fa75c(track));
    if (!fn_710117b348(animation, fn_7100cfb828())) {
        UnkUnfreezeRequest_l7fa75c request;
        const UnkUnfreezeDesc_l7fa75c* desc = reinterpret_cast<const UnkUnfreezeDesc_l7fa75c*>(&g_unk_710164b958);
        u64 desc0 = desc->words[0];
        u64 desc1 = desc->words[1];
        const UnkUnfreezeBlend_l7fa75c* blend_desc =
            reinterpret_cast<const UnkUnfreezeBlend_l7fa75c*>(&g_unk_7172769740);
        u64 kind = blend_desc->kind;
        u32 value = blend_desc->value;
        u32 mask = UnkAll_SpitSend().value;
        request.id = -1;
        request.desc[0] = desc0;
        request.desc[1] = desc1;
        request.blend_kind = kind;
        request.blend_value = value;
        request.flags = 2;
        request.mask = mask;

        UnkTrackQuery query = {fn_7100c63528(), nullptr, -1.0f};
        fn_710117b08c(animation, &query);
        if (query.result) {
            reinterpret_cast<UnkUnfreezeTrack_l7fa75c*>(query.result)->vf_0x18(&request);
        } else if (!self->filter || !self->filter(self->filter_object, &request)) {
            if (movement && (fn_71010cc3a4(movement) == 2 || fn_71010cc3a4(movement) == 3) &&
                !IsEmpty_l7fa75c(self->char_class->unfreeze_moving_id))
                request.id = self->char_class->unfreeze_moving_id.data->Key();
            else
                request.id = self->char_class->unfreeze_id.data->Key();
        }
        if (request.id == -1)
            return;
        if (!IsEmpty_l7fa75c(track) && !IsEmpty_l7fa75c(blend)) {
            UnkUnfreezeNodeMap_l7fa75c* map = reinterpret_cast<UnkUnfreezeNodeMap_l7fa75c*>(animation->field_0x40);
            UnkReflObject** it = FindNode_l7fa75c(map, blend.data);
            if (it != &map->values[map->count]) {
                UnkReflObject* node = *it;
                if (node && fn_710009d50c(node->vf_0x0(), fn_71002df4ec()) &&
                    fn_710009d50c(node->vf_0x0(), fn_71002e43a4())) {
                    self->start = self->value;
                    self->target = 1.0f;
                    self->duration = 0.1f;
                    self->time = 0.0f;
                }
            }
        }
        self->playing = fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                                      reinterpret_cast<const UnkRequest_7101176ea8*>(&request), Key_l7fa75c(track));
    } else {
        fn_710117abfc(animation, true, g_unk_7172721128, kEmptyKey);
        if (!IsEmpty_l7fa75c(track))
            fn_710117abfc(animation, true, g_unk_7172721128, track.data->Key());
    }
}
