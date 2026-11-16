// CChozoCommanderAIComponent unit (tools/tu_map.py): once the flag at +0x15f00 is raised, clears it and
// sends the owner's animation the value of its current track under kind 0x4f924882b434779c (mode 0x81),
// first under the hash of the track's name followed by g_unk_71726c9540 and, when that is refused, under
// the name's own hash. Descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// The pooled data of a string id: the string at +0x10 (its key is the id's hash) and a counter at +0x44
// bumped whenever the string is read.
struct UnkStrIdData_l450c28 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    s32 reads;
};

// A held reference to a pooled name: taken from a string id, released without the id's null test.
struct UnkTrackName_l450c28 {
    base::global::UnkStringData* data;

    explicit UnkTrackName_l450c28(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkTrackName_l450c28() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
        }
    }

    const CRntString& Str() const {
        UnkStrIdData_l450c28* string = reinterpret_cast<UnkStrIdData_l450c28*>(data);
        ++string->reads;
        return string->str;
    }
    u64 Hash() const { return static_cast<u64>(reinterpret_cast<UnkStrIdData_l450c28*>(data)->str.key); }
};

static inline const CRntString& Str(const CStrId& id) {
    UnkStrIdData_l450c28* string = reinterpret_cast<UnkStrIdData_l450c28*>(id.data);
    ++string->reads;
    return string->str;
}

// The value descriptor of a float send.
struct UnkSendValue_l450c28 {
    u32 field_0x0;
    u32 field_0x4;
    f32 value;

    explicit UnkSendValue_l450c28(f32 v) : field_0x0(0), field_0x4(1), value(v) {}
};

struct UnkCommanderOwner_l450c28 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkCommanderAI_l450c28 {
    u8 pad_0x0[0x30];
    UnkCommanderOwner_l450c28* owner;  // 0x30
    u8 pad_0x38[0x15ec8];
    bool send_pending;  // 0x15f00
};
static_assert(__builtin_offsetof(UnkCommanderAI_l450c28, send_pending) == 0x15f00, "layout");

extern __attribute__((visibility("hidden"))) CStrId g_unk_71726c9540;
f32 fn_710117808c(UnkAnimationComponent* self, u64 key);

void fn_7100450c28(UnkCommanderAI_l450c28* self) {
    if (!self->send_pending)
        return;
    self->send_pending = false;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return;
    const u64 track = 0xf4061ff25a2163d6;
    const u64 kind = 0x4f924882b434779c;
    UnkTrackName_l450c28 name(*fn_7101177ae8(animation, track));
    f32 value = fn_710117808c(animation, track);
    u64 hash;
    {
        CRntString full = fn_710007eee0(&name.Str(), &Str(g_unk_71726c9540));
        fn_7100066630(&hash, &full);
    }
    base::reflection::UnkReflTypeIdArg mode, mask, mode2, mask2;
    bool sent;
    {
        u64 hash1 = hash;
        mode.id = 0x81;
        UnkSendValue_l450c28 desc(value);
        mask.id = UnkAll_SpitSend().value;
        sent = fn_7101176690(animation, hash1, kind, &mode, &g_unk_710164b958, &desc, &mask);
    }
    if (!sent) {
        u64 hash2 = name.Hash();
        mode2.id = 0x81;
        UnkSendValue_l450c28 desc(value);
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, hash2, kind, &mode2, &g_unk_710164b958, &desc, &mask2);
    }
}
