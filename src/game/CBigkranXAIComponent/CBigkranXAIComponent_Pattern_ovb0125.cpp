// CBigkranXAIComponent's unit (tools/tu_map.py unit 600): the component's destructor (fn_71003f46b0), the
// next entry of the attack pattern (fn_71003f4f3c: the entry's angle in radians, mirrored with the flag at
// +0x14ba0, plus the angle between the owner's x axis and the world's, and the entry's two other values) and
// the pattern update (fn_71003f49a8: takes the pattern fn_71003f62a8 picks, notes whether the owner's y
// angle is negative, and when the timer at +0x14b4c runs out tries the next entry with fn_71003f50d0).
// Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkXformNode.h"
#include "types.h"

// CBigkranXAIComponent's destructor (fn_71003f46b0): releases the string id at +0x14b80, frees the
// array at +0x14b58, destroys the spit attack at +0x14a20 (fn_71003f39a8) and the magma rain attack at
// +0x149c0 (its listener at +0x38, then CAttack's part), then the base component (fn_71003e59d0).
// Names and layouts are placeholders.

extern void* g_unk_7101a31448[];  // the component's vtable
extern void* g_unk_7101a31160[];  // the magma rain attack's vtable

struct UnkSpitAttackDtor;
void fn_71003f39a8(UnkSpitAttackDtor* self);
void fn_71003e59d0(void* self);

struct UnkMagmaRainAttack_ovb0125 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackListenerRaw listener;  // 0x38

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a31160 + 2;
        listener.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkBigkranXAIComponent_ovb0125 {
    void** vtable;
    u8 pad_0x8[0x149b8];
    UnkMagmaRainAttack_ovb0125 field_0x149c0;
    u8 pad_0x14a18[0x8];
    u8 field_0x14a20[0x138];  // the spit attack
    base::UnkVector<void*> field_0x14b58;
    UnkAttackStrIdRaw field_0x14b80;
};
static_assert(__builtin_offsetof(UnkBigkranXAIComponent_ovb0125, field_0x14b80) == 0x14b80, "layout");

void fn_71003f46b0(UnkBigkranXAIComponent_ovb0125* self) {
    self->vtable = g_unk_7101a31448 + 2;
    self->field_0x14b80.Release();
    self->field_0x14b58.Free();
    fn_71003f39a8(reinterpret_cast<UnkSpitAttackDtor*>(self->field_0x14a20));
    self->field_0x149c0.Destroy();
    fn_71003e59d0(self);
}

extern "C" float sqrtf(float);
extern "C" float acosf(float);

struct UnkBigkranXEntry_ovb0125 {
    f32 angle;  // degrees
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
};

struct UnkBigkranXOwner_ovb0125 {
    u8 pad_0x0[0x38];
    union {
        UnkMatrix44 transform;
        UnkXform xform;
    };
};

// The entries' array, assigned out of line (fn_71002cbddc).
struct UnkBigkranXEntries_ovb0125 : base::UnkVector<UnkBigkranXEntry_ovb0125> {
    UnkBigkranXEntries_ovb0125& operator=(const UnkBigkranXEntries_ovb0125& other) asm("fn_71002cbddc");
};

// The pattern fn_71003f62a8 picks: its entries, its id and three more words.
struct UnkBigkranXPattern_ovb0125 {
    UnkBigkranXEntries_ovb0125 entries;
    base::global::CStrId id;  // 0x28
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
};

struct UnkPicker;
struct UnkObject;
void* fn_71003f62a8(UnkPicker* self, UnkObject* object);

struct UnkBigkranXAI_ovb0125 {
    u8 pad_0x0[0x30];
    UnkBigkranXOwner_ovb0125* owner;
    u8 pad_0x38[0x14980];
    UnkPicker* picker;  // 0x149b8
    u8 pad_0x149c0[0x188];
    bool active;  // 0x14b48
    u8 pad_0x14b49[3];
    f32 timer;  // 0x14b4c
    u32 next;  // 0x14b50
    u8 pad_0x14b54[4];
    UnkBigkranXPattern_ovb0125 pattern;  // 0x14b58
    bool mirrored;  // 0x14ba0
    bool enabled;  // 0x14ba1
};
static_assert(__builtin_offsetof(UnkBigkranXAI_ovb0125, mirrored) == 0x14ba0, "layout");

bool fn_71003f4f3c(UnkBigkranXAI_ovb0125* self, u32* index, f32* out_b, f32* out_angle, f32* out_c) {
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    f32 x = m.m[1];
    f32 y = m.m[5];
    f32 z = m.m[9];
    fn_710002614c(&self->owner->transform);
    if (!self->active)
        return false;
    u32 i = *index;
    if (i >= self->pattern.entries.count) {
        i = 0;
        *index = 0;
    }
    *index = i + 1;
    UnkBigkranXEntry_ovb0125* entry = &self->pattern.entries.data[i];
    f32 base = entry->angle * 0.017453292f;
    if (self->mirrored)
        base = -base;
    f32 c = (x + y * 0.0f + z * 0.0f) / sqrtf(x * x + y * y + z * z);
    f32 angle;
    if (c >= 1.0f)
        angle = 0.0f;
    else if (c <= -1.0f)
        angle = 3.1415927f;
    else
        angle = acosf(c);
    *out_angle = base + angle;
    *out_b = entry->field_0x4;
    *out_c = entry->field_0x8;
    return true;
}

bool fn_71003f50d0(UnkBigkranXAI_ovb0125* self, u64* out, f32 b, f32 angle, f32 c);

// Counts the timer down; when it runs out, tries the next entry of the pattern and waits for its
// duration, or keeps waiting when it can't be used.
static inline void Advance(UnkBigkranXAI_ovb0125* self, f32 dt) {
    if (self->enabled && self->active && self->timer > -0.001f) {
        self->timer -= dt;
        if (self->timer < 0.001f) {
            f32 b = -1.0f;
            f32 c = -1.0f;
            f32 angle = -1.0f;
            f32 timer;
            if (fn_71003f4f3c(self, &self->next, &b, &angle, &c)) {
                u64 out = 0;
                if (fn_71003f50d0(self, &out, b, angle, c)) {
                    if (self->active) {
                        u32 last = self->pattern.entries.count - 1;
                        u32 i = self->next < last ? self->next : last;
                        timer = self->pattern.entries.data[i].field_0xc;
                    } else {
                        timer = -1.0f;
                    }
                    self->timer = timer;
                    return;
                }
            }
            self->timer += dt;
        }
    }
}

void fn_71003f49a8(UnkBigkranXAI_ovb0125* self, f32 dt) {
    if (self->picker) {
        UnkBigkranXPattern_ovb0125* pattern =
            static_cast<UnkBigkranXPattern_ovb0125*>(fn_71003f62a8(self->picker, reinterpret_cast<UnkObject*>(self)));
        self->pattern = *pattern;
        self->active = true;
        self->mirrored = self->owner->xform.GetAngle().y < 0.0f;
    }
    Advance(self, dt);
}
