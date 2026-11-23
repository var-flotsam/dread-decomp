// CLifeComponent's unit (tools/tu_map.py unit 2715): applies a damage request. While invulnerable (+0x58) or
// playing a CCharClassIgnoreDamageTrack the damage is refused (6); otherwise it is taken off the life through
// SetLife (slot 0x168). Returns 1 when the life dropped, or the damage was meant to kill one that ignores death,
// or the enemy tunable at +0x21 is set; then the death handling (slot 0x1b8). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::reflection::UnkTunable;

extern UnkTunable g_unk_7172a09b68;

struct UnkEnemyLifeComponent_ovb0043;
struct UnkEnemyLifeRequest_ovb0043;

struct UnkLifeTunables_l16046c {
    u8 pad_0x0[0x21];
    bool field_0x21;
};

struct UnkLifeRequest_l16046c {
    u8 pad_0x0[0x20];
    bool field_0x20;
    u8 pad_0x21[0x7];
    bool field_0x28;
    u8 pad_0x29[0xcf];
    f32 damage;  // 0xf8
};

struct UnkLifeAI_l16046c {
    virtual void vf_0x0();
    bool IgnoresDeath() { return reinterpret_cast<bool (***)(UnkLifeAI_l16046c*)>(this)[0][0x2d8 / 8](this); }
};

struct UnkLifeOwner_l16046c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkLifeAI_l16046c* ai;  // 0x240
};

struct UnkLife_l16046c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLifeOwner_l16046c* owner;  // 0x30
    u8 pad_0x38[0x1c];
    f32 life;              // 0x54
    f32 invulnerable_time;  // 0x58
    bool field_0x5c;

    void SetLife(f32 value, bool flag) {
        reinterpret_cast<void (*)(UnkLife_l16046c*, f32, bool)>(vtable[0x168 / 8])(this, value, flag);
    }
    void OnDeath(f32 old_life, UnkLifeRequest_l16046c* request) {
        reinterpret_cast<void (*)(UnkLife_l16046c*, f32, UnkLifeRequest_l16046c*)>(vtable[0x1b8 / 8])(this, old_life,
                                                                                                    request);
    }
    bool IgnoresDamage() {
        UnkAnimationComponent* animation = owner->animation;
        return animation && fn_710117b348(animation, fn_7100d08f08());
    }
    bool IgnoresDeath() {
        UnkAnimationComponent* animation = owner->animation;
        if (animation && fn_710117b348(animation, fn_7100d09418()))
            return true;
        return owner->ai && owner->ai->IgnoresDeath();
    }
};

u32 fn_710116046c(UnkEnemyLifeComponent_ovb0043* component, UnkEnemyLifeRequest_ovb0043* req) {
    UnkLife_l16046c* self = reinterpret_cast<UnkLife_l16046c*>(component);
    UnkLifeRequest_l16046c* request = reinterpret_cast<UnkLifeRequest_l16046c*>(req);
    f32 old_life = self->life;
    u32 result;
    bool applied;
    if (self->invulnerable_time > 0 || self->IgnoresDamage()) {
        applied = false;
        result = 6;
    } else if (!(request->damage > 0)) {
        applied = false;
        result = 0;
    } else {
        applied = true;
        if (self->life > 0)
            self->SetLife(self->life - request->damage, true);
        result = 0;
    }
    if (old_life > self->life) {
        result = 1;
    } else {
        bool ignores_death = self->IgnoresDeath();
        if ((applied && old_life <= 0 && ignores_death) ||
            (applied && reinterpret_cast<UnkLifeTunables_l16046c*>(&g_unk_7172a09b68)->field_0x21))
            result = 1;
    }
    if ((self->field_0x5c || request->field_0x28) && !(self->life > 0))
        self->SetLife(1.0f, false);
    if (old_life > 0 && !(self->life > 0))
        self->OnDeath(old_life, request);
    if (request->field_0x20)
        result = 1;
    return result;
}
