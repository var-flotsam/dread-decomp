// CCharClassBaseDamageTriggerComponent's unit (tools/tu_map.py unit 1561): fn_7100a45cf0, an update
// step. After fn_7100a470e8, while the timer at +0xbc0 is zero, the game manager accepts the argument
// (fn_7100df5c38) and the flag at +0xae9 is set, the value the manager's +0x38 object gives for the
// owner's character class (+0x30, its +0x28; +0x98 of it, fn_7100f0ec68) less the frame time
// (g_unk_7101d47328's +0x30), at least zero, goes to fn_7100a45e00; then, when slot 0x1e8 says so,
// slot 0x1f8 runs with the argument. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassBaseTriggerComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "types.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::UnkReflObject;

struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;

struct UnkDamageTrigger_b0119;
void fn_7100a470e8(UnkDamageTrigger_b0119* self, void* arg);
f32 fn_7100f0ec68(void* table, void* key);
struct UnkBaseDamageTrigger2;
void fn_7100a45e00(UnkBaseDamageTrigger2* self, f32 dt);

struct UnkDamageTriggerOwner_b0119 {
    u8 pad_0x0[0x28];
    UnkReflObject* char_class;  // 0x28
};

struct UnkDamageTrigger_b0119 {
    virtual void vf_0x0();
    u8 pad_0x8[0x28];
    UnkDamageTriggerOwner_b0119* owner;  // 0x30
    u8 pad_0x38[0xab1];
    bool field_0xae9;
    u8 pad_0xaea[0xd6];
    f32 field_0xbc0;

    bool vf_0x1e8() { return (reinterpret_cast<bool (***)(UnkDamageTrigger_b0119*)>(this))[0][0x1e8 / 8](this); }
    void vf_0x1f8(void* arg) {
        (reinterpret_cast<void (***)(UnkDamageTrigger_b0119*, void*)>(this))[0][0x1f8 / 8](this, arg);
    }
};

static inline UnkReflObject* CharClass(UnkReflObject* obj) {
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->vf_0x0(), fn_71011abbd4()) ? obj : nullptr;
}

void fn_7100a45cf0(UnkDamageTrigger_b0119* self, void* arg) {
    fn_7100a470e8(self, arg);
    if (self->field_0xbc0 != 0.0f)
        return;
    if (!fn_7100df5c38(g_unk_7101d47310, arg))
        return;
    if (!self->field_0xae9)
        return;
    char* table = reinterpret_cast<char*>(g_unk_7101d47310) + 0x38;
    UnkReflObject* cls = CharClass(self->owner->char_class);
    f32 t = fn_7100f0ec68(table, reinterpret_cast<char*>(cls) + 0x98);
    t -= *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(g_unk_7101d47328) + 0x30);
    fn_7100a45e00(reinterpret_cast<UnkBaseDamageTrigger2*>(self), !(t < 0.0f) ? t : 0.0f);
    if (self->vf_0x1e8())
        self->vf_0x1f8(arg);
}

// CCharClassBaseDamageTriggerComponent's unit (tools/tu_map.py unit 1561): fn_7100a45ac4, the
// trigger's update by `dt` (after fn_7100a474f8). While the cooldown at +0xbc0 runs it counts down,
// and once it ends slot 0x1f0 runs. Otherwise, with the state at +0x9b8 at least 1 and slot 0x1e8
// true: the delay at +0xbd8 counts down, and once it has run out the damage request (+0xac0) gets
// min(+0xbb8 + +0xbd0, +0xbd4) (fn_71008c1538), the flag at +0xbe0 goes to +0xae1 and the delay
// restarts from the interval (+0xbc8); then the hit timer at +0xbdc counts down, and once it has run
// out it restarts and the player's (fn_7100dec788) component at +0x1c0, unless its +0x349 is set,
// is hit: slot 0x1f8 with the player, the request's source handle (+0xac8) set to the owner's
// (fn_7100d78378), a copy of the request sent (slot 0x188) and, with +0xae9 set, fn_710116a168
// called with the owner's +0x30. Names and layouts are placeholders.
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkFan_A4_b74;
struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);
struct UnkGooplotRequest_b63a3;
struct UnkBase_71003cc474;
void fn_710116a168(void* target, void* key);

// The damage request (0x100 bytes): its source handle at +0x8.
struct UnkDamageRequest_b0119 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    u8 pad_0x10[0x11];
    bool field_0x21;
    u8 pad_0x22[0x7];
    bool field_0x29;
    u8 pad_0x2a[0xce];
    f32 field_0xf8;
    u8 pad_0xfc[0x4];

    UnkDamageRequest_b0119(const UnkDamageRequest_b0119& other) {
        fn_71008c1090(reinterpret_cast<UnkGooplotRequest_b63a3*>(this),
                      reinterpret_cast<const UnkGooplotRequest_b63a3*>(&other));
    }
    ~UnkDamageRequest_b0119() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkDamageRequest_b0119) == 0x100, "UnkDamageRequest_b0119 size");

struct UnkDamageTarget_b0119 {
    void** vtable;
    u8 pad_0x8[0x341];
    bool field_0x349;

    void vf_0x188(UnkDamageRequest_b0119* request) {
        reinterpret_cast<void (*)(UnkDamageTarget_b0119*, UnkDamageRequest_b0119*)>(vtable[0x188 / 8])(this,
                                                                                                   request);
    }
};

struct UnkDamagePlayer_b0119 {
    u8 pad_0x0[0x1c0];
    UnkDamageTarget_b0119* target;  // 0x1c0
};

struct UnkDamageOwner_b0119 {
    u8 pad_0x0[0x30];
    u8 field_0x30[0x8];
};

struct UnkCharClassDamageTrigger_b0119 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkDamageOwner_b0119* owner;  // 0x30
    u8 pad_0x38[0x980];
    s32 state;  // 0x9b8
    u8 pad_0x9bc[0x104];
    UnkDamageRequest_b0119 request;  // 0xac0
    f32 cooldown;  // 0xbc0
    u8 pad_0xbc4[0x4];
    u32 interval;  // 0xbc8
    u8 pad_0xbcc[0x4];
    f32 field_0xbd0;
    f32 field_0xbd4;
    f32 delay;      // 0xbd8
    f32 hit_timer;  // 0xbdc
    bool field_0xbe0;

    bool vf_0x1e8() { return reinterpret_cast<bool (*)(UnkCharClassDamageTrigger_b0119*)>(vtable[0x1e8 / 8])(this); }
    void vf_0x1f0() { reinterpret_cast<void (*)(UnkCharClassDamageTrigger_b0119*)>(vtable[0x1f0 / 8])(this); }
    void vf_0x1f8(UnkPlayer* player) {
        reinterpret_cast<void (*)(UnkCharClassDamageTrigger_b0119*, UnkPlayer*)>(vtable[0x1f8 / 8])(this, player);
    }
};

void fn_7100a45ac4(UnkCharClassDamageTrigger_b0119* self, f32 dt) {
    fn_7100a474f8(reinterpret_cast<UnkFan_A4_b74*>(self), dt);
    if (self->cooldown > 0.0f) {
        self->cooldown -= dt;
        if (!(self->cooldown <= 0.0f))
            return;
        self->cooldown = 0.0f;
        self->vf_0x1f0();
        return;
    }
    if (self->state < 1)
        return;
    if (!self->vf_0x1e8())
        return;
    if (self->delay > 0.0f) {
        self->delay -= dt;
    } else {
        f32 amount = self->request.field_0xf8 + self->field_0xbd0;
        f32 limit = self->field_0xbd4;
        *reinterpret_cast<u32*>(&self->delay) = self->interval;
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&self->request), amount < limit ? amount : limit);
        self->request.field_0x21 = self->field_0xbe0;
    }
    if (self->hit_timer > 0.0f) {
        self->hit_timer -= dt;
        return;
    }
    *reinterpret_cast<u32*>(&self->hit_timer) = self->interval;
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (!player)
        return;
    UnkDamageTarget_b0119* target = reinterpret_cast<UnkDamagePlayer_b0119*>(player)->target;
    if (!target || target->field_0x349)
        return;
    self->vf_0x1f8(player);
    UnkWeakHandle* source = fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner));
    UnkDamageRequest_b0119* own = &self->request;
    AssignRef(&own->source, source);
    UnkDamageRequest_b0119 request(*own);
    target->vf_0x188(&request);
    if (self->request.field_0x29)
        fn_710116a168(target, self->owner->field_0x30);
}
