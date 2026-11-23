// CScorpiusAIComponent unit: tints the owner's part named by a fixed hash with a colour blended between two
// colours by the cubed amount, when the owner's world accepts the part. Names and layouts are placeholders.
#include "game/CFXComponent.h"
#include "game/UnkTU134.h"
#include "game/UnkTU39.h"
#include "game/UnkV4_b47.h"
#include "types.h"

static inline UnkV4_b47 operator*(const UnkV4_b47& a, f32 s) { return fn_7100030830(&a, s); }
static inline UnkV4_b47 operator+(const UnkV4_b47& a, const UnkV4_b47& b) { return fn_7100030770(&a, &b); }

// Linear blend from a (t = 0) to b (t = 1).
static inline UnkV4_b47 Lerp(const UnkV4_b47& a, const UnkV4_b47& b, f32 t) { return a * (1.0f - t) + b * t; }

struct UnkGlowPart_l692a98 {
    u8 pad_0x0[0xb];
    u8 field_0xb;
};

struct UnkGlowOwner_l692a98 {
    u8 pad_0x0[0x280];
    UnkWorld* world;  // 0x280
};

struct UnkScorpiusAI_l692a98 {
    u8 pad_0x0[0x30];
    UnkGlowOwner_l692a98* owner;  // 0x30
};

struct UnkMissilePart;

void fn_7100692a98(UnkScorpiusAI_l692a98* self, f32 amount) {
    UnkWorld* world = self->owner->world;
    UnkGlowPart_l692a98* part = reinterpret_cast<UnkGlowPart_l692a98*>(fn_7101152b90(world, 0x7eced1a36c46400full));
    if (!part)
        return;
    if (!fn_710114e668(reinterpret_cast<UnkObject*>(world), part->field_0xb))
        return;
    f32 t = amount * amount * amount - 1.0f + 1.0f;
    UnkV4_b47 high(1.0f, 0.66f, 0.12f, 1.0f);
    UnkV4_b47 low(1.0f, 0.1f, 0.1f, 1.0f);
    UnkV4_b47 color = t >= 1.0f ? high : t < 0.0f ? low : Lerp(low, high, t);
    fn_7100146c50(reinterpret_cast<UnkMissilePart*>(part), 0, &color, 1);
}
