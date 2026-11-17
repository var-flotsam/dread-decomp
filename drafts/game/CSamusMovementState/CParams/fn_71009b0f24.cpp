// CSamusMovementState::CParams's unit (tools/tu_map.py): while the movement has a pending
// morph-ball transition, lets the state fill a transition and hands it to the movement; when the
// transition asks for it, notifies the game manager and plays the to-morph-ball sound. The pending
// flag is cleared either way. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkMove_b152;
bool fn_71009e7f20(UnkMove_b152* self);
bool fn_71009e8218(UnkMove_b152* self, const UnkRequest_7101176ea8* request, bool flag);
void fn_7100df3418(UnkGameManager* manager, bool a, bool b, void* x3);

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c374(UnkEntityWithSound* self, void* a, void* b, bool c, bool d, bool e);

struct UnkMorphDesc_l9b0f24 {
    u64 words[2];
};

struct UnkMorphBlend_l9b0f24 {
    u64 kind;
    u32 value;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h, flags, a mask and
// whether the change into the morph ball is to be announced.
struct UnkMorphRequest_l9b0f24 {
    s64 id;
    u64 desc[2];      // 0x8
    u64 blend_kind;   // 0x18
    u32 blend_value;  // 0x20
    u32 flags;        // 0x24
    u32 mask;         // 0x28
    u8 pad_0x2c[4];
    bool announce;    // 0x30
};

struct UnkMorphMovement_l9b0f24 {
    u8 pad_0x0[0x51a4];
    bool pending;  // 0x51a4
};

struct UnkMorphState_l9b0f24 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkEntityWithSound* owner;           // 0x30
    UnkMorphMovement_l9b0f24* movement;  // 0x38

    bool vf_0x120(UnkMorphRequest_l9b0f24* request) {
        return reinterpret_cast<bool (*)(UnkMorphState_l9b0f24*, UnkMorphRequest_l9b0f24*)>(vtable[0x120 / 8])(
            this, request);
    }
    void vf_0x128() { reinterpret_cast<void (*)(UnkMorphState_l9b0f24*)>(vtable[0x128 / 8])(this); }
};

void fn_71009b0f24(UnkMorphState_l9b0f24* self) {
    if (fn_71009e7f20(reinterpret_cast<UnkMove_b152*>(self->movement)) && self->movement->pending) {
        const UnkMorphDesc_l9b0f24* desc = reinterpret_cast<const UnkMorphDesc_l9b0f24*>(&g_unk_710164b958);
        u64 desc0 = desc->words[0];
        u64 desc1 = desc->words[1];
        const UnkMorphBlend_l9b0f24* blend = reinterpret_cast<const UnkMorphBlend_l9b0f24*>(&g_unk_7172769740);
        u64 kind = blend->kind;
        u32 value = blend->value;
        u32 mask = UnkAll_SpitSend().value;
        UnkMorphRequest_l9b0f24 request;
        request.id = -1;
        request.desc[0] = desc0;
        request.desc[1] = desc1;
        request.blend_kind = kind;
        request.blend_value = value;
        request.flags = 0;
        request.mask = mask;
        request.announce = false;
        if (self->vf_0x120(&request)) {
            fn_71009e8218(reinterpret_cast<UnkMove_b152*>(self->movement),
                          reinterpret_cast<const UnkRequest_7101176ea8*>(&request), true);
            self->vf_0x128();
            if (request.announce) {
                fn_7100df3418(g_unk_7101d47310, true, false, nullptr);
                static CFilePathStrId s_sound("actors/samus/samus_to_mb_03.wav", true);
                static UnkSoundDesc s_desc;
                s_desc.kind.field_0x0 = 0;
                s_desc.path = s_sound;
                s_desc.field_0x6d = true;
                s_desc.field_0x38 = 0.4f;
                s_desc.field_0x3c = 500.0f;
                s_desc.field_0x40 = 3000.0f;
                s_desc.field_0x44 = 1.3f;
                fn_7100d7c374(self->owner, &s_desc, &g_unk_7101d470e0, false, true, true);
            }
        }
    }
    self->movement->pending = false;
}
