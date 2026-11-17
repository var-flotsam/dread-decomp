// CShakernautDoubleGroundShockAttack's unit (tools/tu_map.py unit 1041): sends the target at +0x58 a
// mode-2 request under `id`, or, when `plain` is clear, under the hash of a suffix id (chosen by the
// side at +0x34) followed by `id`'s text. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// The two suffix ids, by side.
extern CStrId g_unk_71727057e0[2] __attribute__((visibility("hidden")));

static inline const CRntString* Address(const CRntString& s) {
    return &s;
}

struct UnkShockAttack_l6a4d74 {
    u8 pad_0x0[0x34];
    s32 side;  // 0x34
    u8 pad_0x38[0x20];
    void* target;  // 0x58
};

void fn_71006a4d74(UnkShockAttack_l6a4d74* self, const CStrId* id, bool plain) {
    // Declared in the switch before the later label: the plain request's arguments keep slots of their own.
    switch (static_cast<int>(plain)) {
    case 1:
        void* plain_target;
        base::reflection::UnkReflTypeIdArg plain_mode;
        base::reflection::UnkReflTypeIdArg plain_mask;
        u64 plain_hash;
        plain_target = self->target;
        plain_hash = id->data->Key();
        plain_mode.id = 2;
        plain_mask.id = UnkAll_SpitSend().value;
        fn_7101176690(plain_target, plain_hash, 0xf4061ff25a2163d6ull, &plain_mode, &g_unk_710164b958,
                      &g_unk_7172769740, &plain_mask);
        break;
    default: {
        UnkTunableLine name;
        fn_710007eb6c(name.Str(), id);
        const CStrId* suffix_id = &g_unk_71727057e0[self->side == 1 ? 0 : 1];
        UnkTunableLine suffix;
        fn_710007eb6c(suffix.Str(), suffix_id);
        u64 key;
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        fn_7100066630(&key, Address(fn_710007eee0(suffix.Str(), name.Str())));
        void* target = self->target;
        u64 hash = key;
        mode.id = 2;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash, 0xf4061ff25a2163d6ull, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
        break;
    }
    }
}
