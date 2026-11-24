// CCharClassSteamJetComponent unit (tools/tu_map.py): the steam jet's update. After a start delay it sets the
// jet up once, then cycles between blowing and stopping (timers +0xbf8, +0xc00), scaling the jet's piece by the
// fade, and while growing (+0xc98) eases its size in over +0xc88 seconds. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CActorComponent.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU285.h"
#include "game/UnkTU53.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCharClassDamageTrigger_b0119;
struct UnkSteamJet_ovb0008;
struct UnkThermalComponent_ovb0097;
struct UnkWeakTarget;

void fn_7100a45ac4(UnkCharClassDamageTrigger_b0119* self, f32 dt);
UnkThermalComponent_ovb0097* fn_710063337c(UnkWeakTarget* entity);
void fn_710089ebd0(UnkSteamJet_ovb0008* self);
void fn_710089ed88(UnkSteamJet_ovb0008* self, bool on);
void fn_710089ee54(UnkSteamJet_ovb0008* self);

extern const u64 g_unk_710164af28;
extern const u64 g_unk_710164af30;

struct UnkCountedTarget_l89e7c0 : base::UnkRefCounted {
    UnkWeakTarget* object;  // 0x10
};

struct UnkSteamJetPiece_l89e7c0 {
    u8 pad_0x0[0x28];
    UnkXformNode node;  // 0x28
};

struct UnkSteamJet_l89e7c0 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0xad8 - 0x38];
    bool visible_a;  // 0xad8
    u8 pad_0xad9[0xae3 - 0xad9];
    bool visible_b;  // 0xae3
    u8 pad_0xae4[0xbc0 - 0xae4];
    f32 delay;  // 0xbc0
    u8 pad_0xbc4[0xbec - 0xbc4];
    f32 blow_time;   // 0xbec
    f32 rest_time;   // 0xbf0
    u32 pad_0xbf4;
    f32 timer;       // 0xbf8
    bool active;     // 0xbfc
    bool blowing;    // 0xbfd
    bool stopping;   // 0xbfe
    u8 pad_0xbff;
    f32 fade;        // 0xc00
    f32 fade_time;   // 0xc04
    bool hidden;     // 0xc08
    u8 pad_0xc09[0xc18 - 0xc09];
    u8 target_source[0x28];  // 0xc18
    u32 target_state;        // 0xc40
    u32 pad_0xc44;
    base::UnkRefHandle target;  // 0xc48
    u8 pad_0xc50[0xc58 - 0xc50];
    UnkSteamJetPiece_l89e7c0* piece;  // 0xc58
    UnkSteamJetPiece_l89e7c0* idle;   // 0xc60
    u8 pad_0xc68[0xc70 - 0xc68];
    f32 scale;        // 0xc70
    f32 size;         // 0xc74
    u8 pad_0xc78[0xc80 - 0xc78];
    f32 grow_time;    // 0xc80
    f32 grown_time;   // 0xc84
    f32 grow_length;  // 0xc88
    bool keep;        // 0xc8c
    bool grown;       // 0xc8d
    u8 pad_0xc8e[0xc98 - 0xc8e];
    bool growing;     // 0xc98
};
static_assert(__builtin_offsetof(UnkSteamJet_l89e7c0, target) == 0xc48, "layout");
static_assert(__builtin_offsetof(UnkSteamJet_l89e7c0, growing) == 0xc98, "layout");

static inline UnkSteamJet_ovb0008* Jet(UnkSteamJet_l89e7c0* self) {
    return reinterpret_cast<UnkSteamJet_ovb0008*>(self);
}

static inline UnkSwitchObject* Switch(UnkSteamJetPiece_l89e7c0* piece) {
    return reinterpret_cast<UnkSwitchObject*>(piece);
}

static inline void ResolveTarget(UnkSteamJet_l89e7c0* self) {
    if (self->target_state == 1) {
        {
            UnkRefPtr ref = fn_710023b580(self->target_source);
            fn_7100234f78(&self->target, reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        self->target_state = 2;
    }
}

static inline void SetPieceScale(UnkSteamJetPiece_l89e7c0* piece, f32 scale) {
    UnkVec3f v(scale, scale, scale);
    fn_7100208fa4(&piece->node, &v);
}

static inline void Show(UnkSteamJet_l89e7c0* self) {
    self->visible_a = self->visible_b = !self->hidden;
}

// t clamped to [0, 1].
static inline f32 Clamp01(f32 t) {
    return !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
}

// From `from` to `to` by t, held at the ends.
static inline f32 Mix(f32 from, f32 to, f32 t) {
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + t * to;
}

void fn_710089e7c0(UnkSteamJet_l89e7c0* self, f32 dt) {
    fn_7100a45ac4(reinterpret_cast<UnkCharClassDamageTrigger_b0119*>(self), dt);
    f32 delay = self->delay;
    if (delay > 0.0f) {
        self->delay = delay - dt;
        return;
    }
    if (!self->piece && delay <= 0.0f) {
        fn_710089ebd0(Jet(self));
        ResolveTarget(self);
        auto* c = static_cast<UnkCountedTarget_l89e7c0*>(self->target.ptr);
        if (UnkWeakTarget* target = c ? c->object : nullptr) {
            if (UnkThermalComponent_ovb0097* component = fn_710063337c(target))
                fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(component), true);
        }
        self->blowing = true;
        fn_7100d7c828(self->owner, &g_unk_710164af28, false);
        fn_710089ed88(Jet(self), true);
        Show(self);
        self->fade = 0.0f;
    }
    if (self->piece) {
        if (self->blowing) {
            self->fade -= dt;
            if (self->fade <= 0.0f) {
                self->blowing = false;
                fn_710089ed88(Jet(self), true);
                fn_7100d7c828(self->owner, &g_unk_710164af30, false);
                if (self->idle) {
                    fn_7100146e60(Switch(self->idle), false);
                    fn_7100146e60(Switch(self->piece), true);
                    fn_7100208a40(&self->piece->node);
                }
            }
            f32 t = (self->fade_time - self->fade) / self->fade_time;
            SetPieceScale(self->piece, self->scale * self->size * Clamp01(t));
        } else if (self->stopping) {
            self->fade -= dt;
            if (self->fade <= 0.0f) {
                self->stopping = false;
                self->visible_a = self->visible_b = false;
                fn_7100146e60(Switch(self->idle), false);
            }
            f32 t = self->fade / self->fade_time;
            SetPieceScale(self->piece, self->scale * self->size * Clamp01(t));
        } else {
            self->timer += dt;
            if (!self->active) {
                if (self->rest_time > 0.0f && self->timer >= self->rest_time) {
                    self->timer = 0.0f;
                    self->blowing = true;
                    fn_7100d7c828(self->owner, &g_unk_710164af28, false);
                    fn_7100146e60(Switch(self->idle), true);
                    fn_7100208a40(&self->idle->node);
                    Show(self);
                    self->fade = self->fade_time;
                }
            } else if (self->blow_time > 0.0f && self->timer >= self->blow_time) {
                self->timer = 0.0f;
                self->stopping = true;
                fn_710089ed88(Jet(self), false);
                fn_710089ee54(Jet(self));
                self->fade = self->fade_time;
            }
        }
    }
    if (self->growing) {
        f32 elapsed = self->grow_time;
        f32 length = self->grow_length;
        f32 t = !(elapsed <= 0.0f) ? (elapsed >= length ? length : elapsed) : 0.0f;
        f32 size = Mix(1.0f, 0.0f, SmoothStepCubic(t / length));
        if (self->grow_time <= self->grow_length) {
            self->size = size;
            f32 scale = self->blowing || self->stopping ? self->fade * self->scale : self->scale;
            SetPieceScale(self->piece, size * scale);
        } else {
            if (!self->grown) {
                if (!self->keep)
                    fn_710089ee54(Jet(self));
                self->grown = true;
            }
            if (self->grown_time >= self->rest_time && !self->keep)
                fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), false);
            self->grown_time += dt;
        }
        self->grow_time += dt;
    }
}
