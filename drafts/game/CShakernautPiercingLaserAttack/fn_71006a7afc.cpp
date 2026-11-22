// CShakernautPiercingLaserAttack's unit: plays an animation id on the attack's animation component (+0x58)
// in mode 2. With `plain` set the id is sent as it is; otherwise it is prefixed with the facing side's
// name (one of two ids, chosen by +0xb0) and the joined string's key is sent. Names and layouts are
// placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// The ids of the two sides' names, picked by the side the attack faces.
extern CStrId g_unk_7172705cd8[2] __attribute__((visibility("hidden")));

using UnkLaserName_l6a7afc = UnkBufferString<0x100, g_unk_7101a194b8>;

struct UnkShakernautLaserPlay_l6a7afc {
    u8 pad_0x0[0x58];
    void* animation;  // 0x58
    u8 pad_0x60[0x50];
    s32 side;  // 0xb0
};
static_assert(__builtin_offsetof(UnkShakernautLaserPlay_l6a7afc, side) == 0xb0, "layout");

void fn_71006a7afc(UnkShakernautLaserPlay_l6a7afc* self, CStrId id, bool plain) {
    switch (static_cast<int>(plain)) {
    case 1:
        // Declared before the next case label: no lifetime markers, so these keep slots of their own.
        base::reflection::UnkReflTypeIdArg mode_plain;
        base::reflection::UnkReflTypeIdArg mask_plain;
        u64 hash_plain;
        void* animation_plain;
        hash_plain = id.data->Key();
        animation_plain = self->animation;
        mode_plain.id = 2;
        mask_plain.id = UnkAll_SpitSend().value;
        fn_7101176690(animation_plain, hash_plain, 0xf4061ff25a2163d6, &mode_plain, &g_unk_710164b958,
                      &g_unk_7172769740, &mask_plain);
        break;
    default: {
        UnkLaserName_l6a7afc name;
        CRntString_AssignStrId(name.Str(), &id);
        const CStrId* side_id = self->side == 1 ? &g_unk_7172705cd8[0] : &g_unk_7172705cd8[1];
        UnkLaserName_l6a7afc side;
        CRntString_AssignStrId(side.Str(), side_id);
        u64 hash;
        {
            CRntString joined = CRntString_Concat(side.Str(), name.Str());
            fn_7100066630(&hash, &joined);
        }
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        void* animation = self->animation;
        mode.id = 2;
        u64 key = hash;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, key, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
        break;
    }
    }
}
