// CEmmyGrabSamusAttack unit (tools/tu_map.py): picks the distance band of the grab. Of the two
// distances at +0x788 and +0x78c (each at most the farthest band), it takes the first for kind 13 when
// it is the larger, else the second; the id at +0x7a0 becomes "_samus" or "_emmy" followed by the
// band's name ("4m".."1m"), and the result scales the vector at +0x758 into the player's +0x260 object.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/CEmmyGrabSamusAttackStatics.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// Room for a string id that the out-of-line constructor (fn_7100000850) builds; it is left
// without a destructor, since its string is always handed on.
union UnkStrIdSlot_l5096b4 {
    CStrId id;
    UnkStrIdSlot_l5096b4() {}
    ~UnkStrIdSlot_l5096b4() {}
};

struct UnkGrabVec_l5096b4 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkGrabTarget_l5096b4 {
    u8 pad_0x0[0x180];
    UnkGrabVec_l5096b4 field_0x180;
};

struct UnkGrabPlayer_l5096b4 {
    u8 pad_0x0[0x260];
    UnkGrabTarget_l5096b4* field_0x260;
};

struct UnkGrabAttack_l5096b4 {
    u8 pad_0x0[0x758];
    UnkGrabVec_l5096b4 field_0x758;
    u8 pad_0x764[0x788 - 0x764];
    f32 samus_distance;
    f32 emmy_distance;
    u8 pad_0x790[0x7a0 - 0x790];
    CStrId id;
};

// Releases `id` and takes `other`'s string, leaving `other` the pool's empty id (CStrId's move
// assignment, with the take written out on both paths).
inline void Take_l5096b4(CStrId& id, CStrId& other) {
    base::global::UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                StringPool_Release(pool, id.data);
                pool = g_unk_7101d47108;
            }
        }
        id.data = other.data;
        other.data = &pool->empty;
    } else {
        id.data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
}

inline f32 Min_l5096b4(f32 a, f32 b) { return a < b ? a : b; }

void fn_71005096b4(UnkStruct_71005077f8* self_, u32 kind) {
    auto* self = reinterpret_cast<UnkGrabAttack_l5096b4*>(self_);
    UnkEmmyGrabStatics& g = g_unk_71726de558;
    f32 samus_distance = Min_l5096b4(g.bands[0], self->samus_distance);
    f32 emmy_distance = Min_l5096b4(g.bands[0], self->emmy_distance);
    UnkInlineStr50 name;
    self->id = g_unk_7101d470e0;
    bool samus = samus_distance >= emmy_distance && kind == 13;
    CRntString_Assign(name.Str(), samus ? g.samus_suffix.Str() : g.emmy_suffix.Str());
    f32 value = samus ? samus_distance : emmy_distance;
    f32* chosen = samus ? &samus_distance : &emmy_distance;
    f32 step = 0.0f;
    for (u32 i = 0; i < 4; ++i) {
        if (value >= g.bands[i]) {
            step = g.bands[i];
            {
                UnkStrIdSlot_l5096b4 slot;
                CRntString text = CRntString_Concat(name.Str(), g.band_names[i].Str());
                CStrId_ConstructFromRntString(&slot.id, text, true);
                Take_l5096b4(self->id, slot.id);
            }
            *chosen -= step;
            break;
        }
    }
    samus_distance = Min_l5096b4(Min_l5096b4(samus_distance, g.bands[3]), g.bands[0] - step);
    f32 scale = samus_distance;
    auto* player = reinterpret_cast<UnkGrabPlayer_l5096b4*>(fn_7100dec788(g_unk_7101d47310));
    if (UnkGrabTarget_l5096b4* target = player->field_0x260) {
        const UnkGrabVec_l5096b4& v = self->field_0x758;
        UnkGrabVec_l5096b4 scaled = {scale * v.x, scale * v.y, scale * v.z};
        target->field_0x180 = scaled;
    }
}
