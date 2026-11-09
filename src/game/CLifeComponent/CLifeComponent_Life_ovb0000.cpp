// CLifeComponent's unit (tools/tu_map.py unit 2715): the life component's reset (fn_710115fc5c) and update
// (fn_7101160078: three timers count down, the queued entries are handled through vtable slot 0x188 and destroyed, and
// the time alive or dead advances with the state at +0x70), setting the life (fn_7101160278: clamped to the maximum,
// snapped down to a whole number within 0.1 and rounded to hundredths, then signalled with whether it went up), killing
// (fn_71011607ac), reacting to damage (fn_7101160f9c: the script's damage callback with the attacker's name and the
// amount, and the time of the player's last damage) and a forwarder of two flags to vtable slot 0x180
// (fn_710116089c). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkParent;
void fn_71010c4228(UnkParent*);
void fn_71010c424c(UnkParent*);

// A pending entry of the component's queue (0x100 bytes, destroyed through its vtable).
struct UnkLifeQueued_ovb0000 {
    virtual ~UnkLifeQueued_ovb0000();
    u8 pad_0x8[0xf8];
};

struct UnkLifeComponentFull_ovb0000;
void fn_710115fd10(UnkLifeComponentFull_ovb0000* self);

struct UnkLifeComponentFull_ovb0000 {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkRefObject* field_0x48;
    f32 max_life;  // 0x50
    f32 life;      // 0x54
    f32 timer_0x58;
    u8 pad_0x5c[0x4];
    f32 timer_0x60;
    bool field_0x64;
    f32 timer_0x68;
    bool field_0x6c;
    u32 state;          // 0x70
    f32 alive_time;     // 0x74
    f32 dead_time;      // 0x78
    u8 pad_0x7c[0x74];
    f32 field_0xf0;
    u8 pad_0xf4[0x18];
    bool field_0x10c;
    u32 field_0x110;
    u8 pad_0x114[0x104];
    UnkLifeQueued_ovb0000* queued;  // 0x218
    u32 queued_count;               // 0x220

    void vf_0x188(UnkLifeQueued_ovb0000* entry) {
        reinterpret_cast<void (*)(UnkLifeComponentFull_ovb0000*, UnkLifeQueued_ovb0000*)>(vtable[0x188 / 8])(this,
                                                                                                          entry);
    }

    void ClearQueued() {
        for (u32 i = 0; i < queued_count; ++i)
            queued[i].~UnkLifeQueued_ovb0000();
        queued_count = 0;
    }
};
static_assert(__builtin_offsetof(UnkLifeComponentFull_ovb0000, queued) == 0x218, "layout");

// The signatures as src/game/CChozoCommanderXLifeComponent's files declare them.
struct UnkCommanderLife_A5_b63;

void fn_710115fc5c(char* component) {
    UnkLifeComponentFull_ovb0000* self = reinterpret_cast<UnkLifeComponentFull_ovb0000*>(component);
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    self->state = 1;
    self->alive_time = 0.0f;
    self->dead_time = 0.0f;
    self->field_0x64 = false;
    self->timer_0x68 = 0.0f;
    self->field_0x6c = false;
    self->field_0xf0 = 0.0f;
    ResetRef(&self->field_0x48);
    self->field_0x10c = false;
    self->field_0x110 = 0;
    self->ClearQueued();
    fn_710115fd10(self);
}

static inline void Tick(f32& timer, f32 dt) {
    if (timer > 0.0f) {
        timer -= dt;
        if (timer < 0.0f)
            timer = 0.0f;
    }
}

void fn_7101160078(UnkCommanderLife_A5_b63* component, f32 dt) {
    UnkLifeComponentFull_ovb0000* self = reinterpret_cast<UnkLifeComponentFull_ovb0000*>(component);
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    Tick(self->timer_0x58, dt);
    Tick(self->timer_0x60, dt);
    Tick(self->timer_0x68, dt);
    if (self->queued_count) {
        for (UnkLifeQueued_ovb0000 *p = self->queued, *end = p + self->queued_count; p != end; ++p)
            self->vf_0x188(p);
        self->ClearQueued();
    }
    if (self->state == 1) {
        self->alive_time += dt;
        self->dead_time = 0.0f;
    } else if (self->state == 2) {
        self->dead_time += dt;
        self->alive_time = 0.0f;
    } else {
        self->alive_time = 0.0f;
        self->dead_time = 0.0f;
    }
}

struct UnkLifeChange_ovb0000 {
    f32 life;
    bool increased;
    UnkLifeChange_ovb0000(f32 life_, bool increased_) : life(life_), increased(increased_) {}
};

void fn_710116179c(void* signal, UnkLifeChange_ovb0000* change);

struct UnkLifeState_ovb0000 {
    void** vtable;
    u8 pad_0x8[0x48];
    f32 max_life;  // 0x50
    f32 life;      // 0x54
    u8 pad_0x58[0x18];
    u32 state;  // 0x70
    u8 pad_0x74[0xa4];
    u8 on_change[0x8];  // 0x118

    void vf_0x1d0(u32 state, void* arg) {
        reinterpret_cast<void (*)(UnkLifeState_ovb0000*, u32, void*)>(vtable[0x1d0 / 8])(this, state, arg);
    }
};

void fn_7101160278(UnkLifeState_ovb0000* self, f32 value) {
    f32 old = self->life;
    f32 life;
    if (value <= 0.0f)
        life = 0.0f;
    else
        life = self->max_life <= value ? self->max_life : value;
    f32 whole = __builtin_floorf(life);
    if (whole + 0.1f > life)
        life = whole;
    self->life = (f32)(s32)(life * 100.0f) / 100.0f;
    if (self->state == 2 && self->life > 0.0f)
        self->vf_0x1d0(1, nullptr);
    UnkLifeChange_ovb0000 change(self->life, old < self->life);
    fn_710116179c(self->on_change, &change);
}

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
void fn_71008c0c74(UnkStruct_71008c0c74* self);
void fn_71008c1558(UnkBase_71003cc474* self);

struct UnkDeathRequest_ovb0000 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    u8 pad_0x10[0x38];
    bool field_0x48;
    u8 pad_0x49[0xb7];

    UnkDeathRequest_ovb0000() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkDeathRequest_ovb0000() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkDeathRequest_ovb0000) == 0x100, "request layout");

struct UnkLifeKill_ovb0000 {
    void** vtable;
    u8 pad_0x8[0x68];
    u32 state;  // 0x70

    void vf_0x168(bool notify, f32 value) {
        reinterpret_cast<void (*)(UnkLifeKill_ovb0000*, bool, f32)>(vtable[0x168 / 8])(this, notify, value);
    }
    void vf_0x1d0(u32 state, UnkDeathRequest_ovb0000* request) {
        reinterpret_cast<void (*)(UnkLifeKill_ovb0000*, u32, UnkDeathRequest_ovb0000*)>(vtable[0x1d0 / 8])(
            this, state, request);
    }
};

void fn_71011607ac(UnkLifeKill_ovb0000* self, bool flag, bool by_player) {
    if (self->state == 2)
        return;
    self->vf_0x168(true, 0.0f);
    UnkDeathRequest_ovb0000 request;
    request.field_0x48 = !flag;
    if (by_player)
        AssignRef(&request.source,
                  fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(fn_7100dec788(g_unk_7101d47310))));
    self->vf_0x1d0(2, &request);
}


struct UnkScriptCaller;
bool fn_7100305088(UnkScriptCaller* self, const char* name, const char* fmt, ...);

// A shared record with a name; the use count is a union member.
struct UnkDamageNamed_ovb0000 {
    u8 unk_0x0[0x10];
    CRntString name;
    u8 unk_pad[0x44 - 0x10 - sizeof(CRntString)];
    union {
        u32 use_count;
    };
};

struct UnkDamageSourceTarget_ovb0000 {
    u8 unk_0x0[0x30];
    UnkDamageNamed_ovb0000* named;
};

struct UnkDamageSource_ovb0000 {
    u8 unk_0x0[0x10];
    UnkDamageSourceTarget_ovb0000* target;
};

struct UnkDamageInfo_ovb0000 {
    u8 unk_0x0[0x8];
    UnkDamageSource_ovb0000* source;  // 0x8
    u8 unk_0x10[0xa0];
    s32 kind;  // 0xb0
    u8 unk_0xb4[0x44];
    f32 amount;  // 0xf8
};

struct UnkLifeActor_ovb0000 {
    u8 unk_0x0[0x1e0];
    UnkScriptCaller* script;  // 0x1e0
};

struct UnkLifeDamaged_ovb0000 {
    u8 unk_0x0[0x30];
    UnkLifeActor_ovb0000* actor;  // 0x30
    u8 unk_0x38[0x78];
    const char* on_damaged;  // 0xb0
    u8 unk_0xb8[0x38];
    f32 last_damage_time;  // 0xf0
};

struct UnkGameManagerTime_ovb0000 {
    u8 unk_0x0[0x2444];
    f32 time;  // 0x2444
};

void fn_7101160f9c(UnkLifeDamaged_ovb0000* self, UnkDamageInfo_ovb0000* info, bool silent) {
    if (!silent) {
        UnkScriptCaller* script = self->actor->script;
        const char* function;
        if (script && (function = self->on_damaged) && function[0]) {
            const CRntString* name = &g_unk_7101d58d10;
            if (info->source && info->source->target) {
                UnkDamageSourceTarget_ovb0000* target = info->source->target;
                ++target->named->use_count;
                name = &target->named->name;
            }
            fn_7100305088(script, function, "sf", name->data ? name->data : "", (f64)info->amount);
        }
    }
    if (info->kind == 2) {
        UnkLifeActor_ovb0000* actor = self->actor;
        if (reinterpret_cast<void*>(actor) == reinterpret_cast<void*>(fn_7100dec788(g_unk_7101d47310))) {
            f32 now = reinterpret_cast<UnkGameManagerTime_ovb0000*>(g_unk_7101d47310)->time;
            if (!(now - self->last_damage_time < 1.0f))
                self->last_damage_time = now;
        }
    }
}

struct UnkLifeFlags_ovb0000 {
    u8 pad_0x0[0xc];
    bool field_0xc;
    bool field_0xd;
};

struct UnkLifeComponent_ovb0000 {
    void** vtable;

    void vf_0x180(bool a, bool b) {
        reinterpret_cast<void (*)(UnkLifeComponent_ovb0000*, bool, bool)>(vtable[0x180 / 8])(this, a, b);
    }
};

void fn_710116089c(UnkLifeComponent_ovb0000* self, const UnkLifeFlags_ovb0000* flags) {
    self->vf_0x180(flags->field_0xc, flags->field_0xd);
}
