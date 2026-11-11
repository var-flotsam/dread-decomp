// CEmmyGrabSamusAttack unit (tools/tu_map.py): fn_710050d07c resets the four floats at 0x6b4..0x6c0
// (0, -1, -1, -1), builds the string id for the current kind (field_0x38's +0x10, or 14 without one)
// with fn_71005077f8, sends it with fn_710050a500 unless it is the empty id, releases it, runs
// fn_710050d150 and sets the flag at 0x6b0. fn_710050eca0 runs fn_710050cd98 and, unless the
// player's +0x1c0 object has its flag at +0x5c set, looks the id's hash up in the player's
// animation (+0x220) and stores that entry's +0x28 minus 0.3 at 0x6c0. Names are placeholders.
#include "base/global/CStrId.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// fn_71005077f8's id type (src/game/CEmmyGrabSamusAttack/fn_71005077f8.cpp): a data pointer with a
// user copy constructor, returned through x8.
struct UnkStrId_71005077f8 {
    UnkStringData* data;
    UnkStrId_71005077f8(const UnkStrId_71005077f8& other);
};
struct UnkStruct_71005077f8;
struct UnkGrabAttack_b59a3;
struct UnkGrabId_b59a3;

struct UnkKindSource_A3_b68 {
    u8 pad_0x0[0x10];
    u32 field_0x10;
};

struct UnkGrabAttack_A3_b68 {
    u8 pad_0x0[0x38];
    UnkKindSource_A3_b68* field_0x38;
    u8 pad_0x40[0x6b0 - 0x40];
    bool field_0x6b0;
    u8 pad_0x6b1[3];
    f32 field_0x6b4;
    f32 field_0x6b8;
    f32 field_0x6bc;
    f32 field_0x6c0;
};

struct UnkSelf;
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkStruct_7101176ea8;
void* fn_7101176d34(UnkStruct_7101176ea8*, void*);

struct UnkPlayerFlags_A3_b68 {
    u8 pad_0x0[0x5c];
    bool field_0x5c;
};

struct UnkPlayer_A3_b68 {
    u8 pad_0x0[0x1c0];
    UnkPlayerFlags_A3_b68* field_0x1c0;
    u8 pad_0x1c8[0x220 - 0x1c8];
    UnkStruct_7101176ea8* field_0x220;
};

struct UnkAnimEntry_A3_b68 {
    u8 pad_0x0[0x28];
    f32 field_0x28;
};

void fn_710050d07c(UnkGrabAttack_A3_b68* self) {
    self->field_0x6b4 = 0.0f;
    self->field_0x6b8 = -1.0f;
    self->field_0x6bc = -1.0f;
    self->field_0x6c0 = -1.0f;
    u32 kind = self->field_0x38 ? self->field_0x38->field_0x10 : 14;
    UnkStrId_71005077f8 id = fn_71005077f8(reinterpret_cast<UnkStruct_71005077f8*>(self), kind);
    UnkStringData* data = id.data;
    if (data != &g_unk_7101d47108->empty)
        fn_710050a500(reinterpret_cast<UnkGrabAttack_b59a3*>(self), reinterpret_cast<const UnkGrabId_b59a3*>(&id));
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (data && data != &pool->empty) {
            if (--data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47108;
            }
        }
        id.data = &pool->empty;
    }
    fn_710050d150(self);
    self->field_0x6b0 = true;
}

void fn_710050eca0(UnkGrabAttack_A3_b68* self) {
    fn_710050cd98(reinterpret_cast<UnkSelf*>(self));
    auto* player = reinterpret_cast<UnkPlayer_A3_b68*>(fn_7100dec788(g_unk_7101d47310));
    if (player->field_0x1c0->field_0x5c)
        return;
    UnkStruct_7101176ea8* anim = player->field_0x220;
    u32 kind = self->field_0x38 ? self->field_0x38->field_0x10 : 14;
    UnkStrId_71005077f8 id = fn_71005077f8(reinterpret_cast<UnkStruct_71005077f8*>(self), kind);
    UnkStringData* data = id.data;
    auto* entry = static_cast<UnkAnimEntry_A3_b68*>(
        fn_7101176d34(anim, *reinterpret_cast<void**>(reinterpret_cast<char*>(data) + 0x30)));
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (data && data != &pool->empty) {
            if (--data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47108;
            }
        }
        id.data = &pool->empty;
    }
    self->field_0x6c0 = entry->field_0x28 + -0.3f;
}
