// CAutectorAIComponent's pulse: when the owner's animation component (+0x220) is on the track named "fly" (the id
// table's id at +0x10c0 looked up under the usual track kind), tints the owner's slots (+0x228's +0x50) with a
// black or white by a sine of the track's value (the value to the power 2.5 for large values). Names and
// layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71001436f4;
bool fn_71001436f4(UnkStruct_71001436f4* self, u32 a, const base::math::CVector4D* b, u32 c);
const CStrId* fn_7101177ae8(UnkAnimationComponent* self, u64 key);

struct UnkGameManagerIdTable {
    u8 pad_0x0[0x10c0];
    CStrId field_0x10c0;
    u8 pad_0x10c8[0x2578 - 0x10c8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};

inline const CStrId& TrackKind_b0266() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table.field_0x10c0;
}

static inline u64 KindHash() {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(TrackKind_b0266().data) + 0x30);
}

struct UnkAutectorOwner_b0266 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x0];
    struct UnkAutectorSlots* slots;  // 0x228
};

struct UnkAutectorSlots {
    u8 pad_0x0[0x50];
    UnkStruct_71001436f4* table;  // 0x50
};

struct UnkAutector_b0266 {
    u8 pad_0x0[0x30];
    UnkAutectorOwner_b0266* owner;
};

void fn_71003bb534(UnkAutector_b0266* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    static CStrId s_fly("fly", true);
    if (animation) {
        if (fn_7101177ae8(animation, KindHash())->data == s_fly.data) {
            f32 value = fn_7101177f58(animation, KindHash());
            if (UnkAutectorSlots* slots = self->owner->slots) {
                if (slots->table) {
                    f32 a = sinf(powf(value, 2.5f) * 0.0031415929552167654f);
                    f32 b = sinf(value * 0.628318548f);
                    f32 wave = value < 20.0f ? b : a;
                    f32 shade = wave < 0.0f ? 0.0f : 1.0f;
                    base::math::CVector4D colour = {shade, shade, shade, 1.0f};
                    fn_71001436f4(slots->table, 0, &colour, 1);
                }
            }
        }
    }
}
