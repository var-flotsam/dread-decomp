// CYamplotXAIComponent's unit: after the base routine (fn_7101111c24) fills `out`, adjusts its two
// vectors (+0x60, +0x6c) by the state of the component's current animation track: a boosted, rising
// start while the usual track plays, else the plain factor. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkCaterzillaAIScale_b49;
struct UnkCaterzillaOut_b49;

// A copied string id whose destructor does not test for a null id.
struct UnkYamplotId_l6f1ed8 {
    UnkStringData* data;
    UnkYamplotId_l6f1ed8(const UnkYamplotId_l6f1ed8& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkYamplotId_l6f1ed8() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data != &pool->empty && --data->ref_count == 0)
                StringPool_Release(pool, data);
        }
    }
};

struct UnkStringEntry_l6f1d30 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkFrameTimer_l6f1d30 {
    u8 pad_0x0[0x30];
    f32 delta;

    // The time the last frame took.
    const f32& Delta() const { return delta; }
};

struct UnkYamplotOut_l6f1d30 {
    u8 pad_0x0[0x60];
    f32 a[3];
    f32 b[3];
};

struct UnkYamplotOwner_l6f1d30 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkYamplot_l6f1d30 {
    u8 pad_0x0[0x30];
    UnkYamplotOwner_l6f1d30* owner;
    u8 pad_0x38[0x14758 - 0x38];
    u8 field_0x14758;
    u8 pad_0x14759[3];
    f32 field_0x1475c;
    u8 pad_0x14760[2];
    u8 field_0x14762;
    u8 pad_0x14763;
    f32 field_0x14764;
};

void fn_71006f1d30(UnkYamplot_l6f1d30* self, UnkYamplotOut_l6f1d30* out) {
    fn_7101111c24(reinterpret_cast<UnkCaterzillaAIScale_b49*>(self), reinterpret_cast<UnkCaterzillaOut_b49*>(out));
    UnkAnimationComponent* anim = self->owner->field_0x220;
    if ((anim != nullptr) & (self->field_0x14762 != 0)) {
        UnkYamplotId_l6f1ed8 id(*reinterpret_cast<const UnkYamplotId_l6f1ed8*>(fn_7101177ae8(anim, 0xf4061ff25a2163d6)));
        if (reinterpret_cast<const UnkStringEntry_l6f1d30*>(id.data)->hash == 0x56b5846baf59688c) {
            if (self->field_0x14762) {
                out->a[0] = out->a[0] * 1.6f;
                out->b[0] = out->b[0] * 1.6f;
                const UnkFrameTimer_l6f1d30* timer = reinterpret_cast<const UnkFrameTimer_l6f1d30*>(g_unk_7101d47328);
                self->field_0x14764 = timer->Delta() + self->field_0x14764;
                if (self->field_0x14764 < 0.4f) {
                    out->a[1] = timer->Delta() * 400.0f;
                    out->b[1] = timer->Delta() * 400.0f;
                }
            }
        } else {
            self->field_0x14762 = 0;
            self->field_0x14764 = 0.0f;
        }
    } else if (self->field_0x14758) {
        out->a[0] = self->field_0x1475c * out->a[0];
        out->b[0] = self->field_0x1475c * out->b[0];
        out->a[1] = 0.0f;
        out->b[1] = 0.0f;
    }
}
