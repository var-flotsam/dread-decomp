// CGunComponent unit (tools/tu_map.py unit 1304, the name from reflection): the gun component's beam
// charge and weapon slots. The active weapon is the one at +0x118 while the flag at +0x128 is set and
// the one at +0x120 otherwise; the weapons of both slots are kept in two grouped lists (+0xc8, +0xf0).
// A charge (+0x178 on, +0x17c its amount) starts from the times fn_7100837220 gives for the gun mode
// (+0x134) and is released or dropped by the cancel helper below. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CStrId;
using base::reflection::UnkReflObject;

// As other units declare them.
struct UnkSamusGunComponent;
struct UnkChargeComponent_b47a3;
struct UnkSamusGun_ovb0070;
struct UnkInventory;
struct UnkFlagged;
struct UnkParent;
struct UnkGlobal2;
struct UnkGunListeners_b53;
void fn_710083f32c(UnkSamusGunComponent*, const CStrId*, bool, bool, bool);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
bool fn_71009e75e8(UnkFlagged*);
void* fn_7100df5460(UnkGameManager*);
void fn_71010c4228(UnkParent*);
void fn_71010c424c(UnkParent*);
void fn_7100841838(UnkGunListeners_b53* self, void* value);
void fn_710007ca94(CRntFile*, char*);
s64 fn_710007cf30(void*, const char*, ...);
void fn_7100839b00(char* self, bool flag);
extern UnkGlobal2 g_unk_7172790a08;

// The game manager's id table (fn_7100031894 builds it; src/game/CGameManager.cpp has the same
// definition).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x4c8];
    CStrId field_0x4c8;
    u8 pad_0x4d0[0x1240 - 0x4d0];
    CStrId field_0x1240;
    u8 pad_0x1248[0x1640 - 0x1248];
    CStrId field_0x1640;
    u8 pad_0x1648[0x2578 - 0x1648];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// A weapon of one of the slots. Its virtual methods are called by slot offset (VCall).
struct UnkGunWeapon_ovb0091 {
    void* vtable;
    u8 pad_0x8[0x18];
    CStrId id;  // 0x20
    u8 pad_0x28[0x50];
    bool ready;  // 0x78
};

using UnkGunWeaponList_ovb0091 = base::UnkVector<UnkGunWeapon_ovb0091*>;
using UnkGunGroups_ovb0091 = base::UnkVector<UnkGunWeaponList_ovb0091>;

struct UnkGunActor_ovb0091 {
    u8 pad_0x0[0x208];
    UnkFlagged* flagged;  // 0x208
    u8 pad_0x210[0x10];
    UnkReflObject* animation;  // 0x220
    u8 pad_0x228[0x38];
    void* field_0x260;  // 0x260
    u8 pad_0x268[0x8];
    UnkInventory* inventory;  // 0x270
    u8 pad_0x278[0x38];
    void* field_0x2b0;  // 0x2b0
    u8 pad_0x2b8[0x30];
    UnkTimelineComponent* timeline;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkGunActor_ovb0091, field_0x2b0) == 0x2b0, "layout");

struct UnkGunTarget_ovb0091 {
    u8 pad_0x0[0x10];
    void* field_0x10;  // 0x10
};

struct UnkGunGrab_ovb0091 {
    u8 pad_0x0[0x18];
    u8 field_0x18;  // 0x18
};

// A callback: called with the object at +0x8; kept while it returns true.
struct UnkGunListener_ovb0091 {
    u8 pad_0x0[0x8];
    void* object;                          // 0x8
    bool (*callback)(void* object, bool);  // 0x10
};

// Three pointer lists and a lock count, as CGunComponent_b53.cpp has them: while locked, additions
// and removals are recorded in the first two lists and applied to the third on unlock.
struct UnkGunListeners_ovb0091 {
    base::UnkVector<void*> added;    // 0x0
    base::UnkVector<void*> removed;  // 0x28
    base::UnkVector<void*> items;    // 0x50
    // A union member: the original reloads the holder's pointer and the counter after the
    // decrement (no type-based alias info).
    union {
        s32 lock;  // 0x78
    };
};
static_assert(__builtin_offsetof(UnkGunListeners_ovb0091, lock) == 0x78, "layout");

struct UnkGunListenersLock_ovb0091 {
    UnkGunListeners_ovb0091* listeners;
};

struct UnkGunComponent_ovb0091 {
    void* vtable;
    u8 pad_0x8[0x28];
    UnkGunActor_ovb0091* actor;  // 0x30
    u8 pad_0x38[0x8];
    UnkGunTarget_ovb0091* target;  // 0x40
    u8 pad_0x48[0x80];
    UnkGunGroups_ovb0091 primary;    // 0xc8
    UnkGunGroups_ovb0091 secondary;  // 0xf0
    UnkGunWeapon_ovb0091* alt;   // 0x118
    UnkGunWeapon_ovb0091* main;  // 0x120
    bool use_alt;      // 0x128
    bool field_0x129;  // 0x129
    u8 pad_0x12a[0x2];
    f32 field_0x12c;  // 0x12c
    u8 field_0x130;   // 0x130
    u8 field_0x131;   // 0x131
    u8 pad_0x132[0x2];
    s32 mode;         // 0x134
    s32 field_0x138;  // 0x138
    u8 pad_0x13c[0x4];
    CStrId* ids;   // 0x140
    u32 id_count;  // 0x148
    u8 pad_0x14c[0x1c];
    CStrId id;           // 0x168
    f32 charge_start;    // 0x170
    f32 charge_full;     // 0x174
    bool charging;       // 0x178
    u8 pad_0x179[0x3];
    f32 charge;          // 0x17c
    u8 field_0x180;      // 0x180
    u8 field_0x181;      // 0x181
    u8 pad_0x182[0x2];
    f32 field_0x184;     // 0x184
    u8 field_0x188;      // 0x188
    bool field_0x189;    // 0x189
    u8 field_0x18a;      // 0x18a
    u8 pad_0x18b[0x1];
    s32 field_0x18c;     // 0x18c
    u8 pad_0x190[0x4];
    u8 field_0x194;      // 0x194
    u8 field_0x195;      // 0x195
    u8 pad_0x196[0x82];
    u8 field_0x218;      // 0x218
    u8 pad_0x219[0xf];
    UnkGunListeners_ovb0091 listeners;  // 0x228
    u8 pad_0x2a8[0x11];
    u8 field_0x2b9;      // 0x2b9
    u8 pad_0x2ba[0xe];
    UnkGunGrab_ovb0091* grab;  // 0x2c8
    u8 pad_0x2d0[0x10];
    void* field_0x2e0;   // 0x2e0
};
static_assert(__builtin_offsetof(UnkGunComponent_ovb0091, listeners) == 0x228, "layout");
static_assert(__builtin_offsetof(UnkGunComponent_ovb0091, field_0x2e0) == 0x2e0, "layout");

using UnkGun_ovb0091 = UnkGunComponent_ovb0091;

template <typename T>
static inline UnkGun_ovb0091* AsGun(T* p) {
    return reinterpret_cast<UnkGun_ovb0091*>(p);
}

// A virtual call by vtable slot offset.
template <typename R, typename... A, typename T>
static inline R VCall(T* object, u32 offset, A... args) {
    using Fn = R (*)(T*, A...);
    return (*reinterpret_cast<Fn**>(object))[offset / 8](object, args...);
}

struct UnkChargeTimes_ovb0091 {
    u32 unk_0x0;
    f32 start;  // 0x4
    f32 full;   // 0x8
};
const UnkChargeTimes_ovb0091* fn_7100837220(s32 mode);
bool fn_710113db74(void* object);
void fn_710083ad6c(UnkSamusGunComponent* gun);
void fn_710083d27c(UnkSamusGunComponent* gun, bool flag);
void fn_710083a034(UnkGun_ovb0091* self, bool flag, bool now);
void* fn_71007bad34(void* object);
void fn_710034f374(void* object);
void fn_710083dd48(UnkGun_ovb0091* self, void* value);
void fn_710083c17c(UnkGun_ovb0091* self);
// base::UnkVector<void*>::GrowAt, out of line.
void** fn_7100841e10(base::UnkVector<void*>* self, u32 index);

struct UnkGunReleaseArgs_ovb0091 {
    s32 mode;
    bool full;
    bool field_0x5;
};
void fn_7100831dcc(void* object, UnkGunWeapon_ovb0091* weapon, bool flag, const UnkGunReleaseArgs_ovb0091* args);
void fn_710083b83c(char* self, const char* a, const char* b, const char* c, f32 value, const char* d,
                   const char* e, const char* f, const char* g, const char* h, s32 i, s32 j);

struct UnkGunTunable_ovb0091 {
    u8 pad_0x0[0x98];
    f32 field_0x98;  // 0x98
};

static inline UnkGunTunable_ovb0091* GunTunable() {
    return reinterpret_cast<UnkGunTunable_ovb0091*>(&g_unk_7172790a08);
}

struct UnkGunModeSource_ovb0091 {
    u8 unk_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkGunModeState_ovb0091 {
    u8 unk_0x0[0x20];
    UnkGunModeSource_ovb0091* source;  // 0x20
    u8 unk_0x28[0x8c];
    s32 kind;  // 0xb4
};

static inline UnkGunWeapon_ovb0091* ActiveWeapon(UnkGun_ovb0091* self) {
    return self->use_alt ? self->alt : self->main;
}

// Mode 2 when there is an active weapon and the game manager's gun state is of kind 6 (for kind 7,
// its object at +0x20 decides), mode 1 otherwise (as CGunComponent_Mode_b50.cpp).
static inline s32 GunMode(UnkGun_ovb0091* self) {
    if (!ActiveWeapon(self))
        return 1;
    auto* state = static_cast<UnkGunModeState_ovb0091*>(fn_7100df5460(g_unk_7101d47310));
    if (!state)
        return 1;
    s32 kind = state->kind;
    if (kind == 7) {
        if (!state->source)
            return 1;
        kind = state->source->state;
    }
    return kind == 6 ? 2 : 1;
}

static inline void StartCharge(UnkGun_ovb0091* self) {
    self->charging = true;
    self->charge = 0.0f;
    self->field_0x131 = 0;
    const UnkChargeTimes_ovb0091* times = fn_7100837220(self->mode);
    self->charge_start = times->start;
    self->charge_full = times->full;
}

// Ends the charge: fires the active weapon's charged shot first when `fire` is set.
static inline void Release(UnkGun_ovb0091* self, bool fire) {
    if (fire) {
        UnkGunWeapon_ovb0091* weapon = ActiveWeapon(self);
        if (weapon)
            VCall<void, s32, bool>(weapon, 0xd8, self->mode, false);
    }
    self->charging = false;
    self->charge = 0.0f;
    self->field_0x180 = 0;
    self->field_0x181 = 0;
    self->field_0x184 = -1.0f;
    self->field_0x189 = false;
}

static inline void CancelCharge(UnkGun_ovb0091* self) {
    if (self->field_0x181) {
        Release(self, true);
    } else if (self->charging) {
        self->field_0x131 = 0;
        if (!self->field_0x188)
            Release(self, self->charge >= self->charge_start);
    }
}

static inline void EndCharge(UnkGun_ovb0091* self) {
    if (!self->charging)
        return;
    CancelCharge(self);
}

static inline void UpdateMode(UnkGun_ovb0091* self) {
    s32 mode = GunMode(self);
    if (self->mode != mode) {
        self->mode = mode;
        CancelCharge(self);
    }
}

static inline void CallAll(UnkGunGroups_ovb0091& groups, u32 slot) {
    for (UnkGunWeaponList_ovb0091* g = groups.data; g != groups.data + groups.count; ++g) {
        for (UnkGunWeapon_ovb0091** it = g->data; it != g->data + g->count; ++it)
            VCall<void>(*it, slot);
    }
}

// The item's qualified id, built on first use.
static inline const UnkQualifiedId& ItemId_ovb0091(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_71008395f0(UnkGun_ovb0091* self) {
    CallAll(self->primary, 0x148);
    CallAll(self->secondary, 0x148);
}

void fn_7100839924(UnkSamusGun_ovb0070* gun) {
    UnkGun_ovb0091* self = AsGun(gun);
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    self->field_0x12c = 0.0f;
    self->field_0x130 = 0;
    GunTunable()->field_0x98 = 1.0f;
    self->mode = 1;
    self->field_0x131 = 0;
    self->charging = false;
    self->charge = 0.0f;
    self->field_0x180 = 0;
    self->field_0x181 = 0;
    self->field_0x184 = -1.0f;
    self->field_0x188 = 0;
    self->field_0x189 = false;
    self->field_0x194 = 0;
    self->field_0x18a = 0;
    self->field_0x18c = 0;
    if (self->field_0x218)
        self->field_0x218 = 0;
    UpdateMode(self);
    self->id = g_unk_7101d470e0;
}

// Always inlined: fn_7100839d70 has it inlined, and it is defined on its own too.
__attribute__((always_inline)) void fn_7100839f68(UnkGun_ovb0091* self, const CStrId& id) {
    UnkGunWeapon_ovb0091* weapon = self->alt;
    if (!weapon)
        return;
    if (self->id.data == id.data) {
        self->id = g_unk_7101d470e0;
        return;
    }
    if (self->id_count && weapon->id.data == id.data && VCall<bool>(weapon, 0x140))
        fn_710083ad6c(reinterpret_cast<UnkSamusGunComponent*>(self));
}

void fn_710083a3c4(UnkGun_ovb0091* self) {
    CancelCharge(self);
    if (self->alt) {
        VCall<void>(self->alt, 0x128);
        self->alt = nullptr;
    }
    self->alt = self->primary.data[0].data[0];
    if (self->main) {
        VCall<void>(self->main, 0x128);
        self->main = nullptr;
    }
    self->main = self->secondary.data[0].data[0];
    self->use_alt = true;
    self->field_0x129 = true;
    self->field_0x12c = 0.0f;
    self->field_0x130 = 0;
    GunTunable()->field_0x98 = 1.0f;
    self->field_0x131 = 0;
    self->charging = false;
    self->charge = 0.0f;
    self->field_0x180 = 0;
    self->field_0x181 = 0;
    self->field_0x188 = 0;
    self->field_0x189 = false;
    self->field_0x194 = 0;
    self->field_0x195 = 0;
    self->field_0x184 = -1.0f;
    VCall<void>(self->alt, 0x150);
    VCall<void>(self->main, 0x150);
    if (self->alt) {
        VCall<void>(self->alt, 0x128);
        self->alt = nullptr;
    }
    if (self->main) {
        VCall<void>(self->main, 0x128);
        self->main = nullptr;
    }
    VCall<void>(self, 0x170);
    VCall<void>(self, 0x178);
}

void fn_710083ab50(UnkGun_ovb0091* self, void* object) {
    if (object) {
        UnkGun_ovb0091* other = static_cast<UnkGun_ovb0091*>(fn_71007bad34(object));
        if (other) {
            self->field_0x2b9 = other->field_0x2b9;
            UnkGunWeapon_ovb0091* weapon = self->main;
            bool flag = weapon && VCall<bool>(weapon, 0x30) ? !other->field_0x129 : false;
            fn_710083a034(self, flag, false);
            UpdateMode(self);
            self->field_0x194 = other->field_0x194;
        }
    }
    if (self->field_0x218)
        self->field_0x218 = 0;
}

// Takes the last queued id off the list (handing it to fn_710083f32c first), then clears the id.
void fn_710083ad6c(UnkSamusGunComponent* gun) {
    UnkGun_ovb0091* self = AsGun(gun);
    if (self->id_count) {
        fn_710083f32c(gun, self->ids + self->id_count - 1, true, false, false);
        s32 last = self->id_count - 1;
        self->ids[last].Reset();
        self->id_count = last;
    }
    self->field_0x138 = 5;
    self->id = g_unk_7101d470e0;
}

void fn_710083cd54(UnkGun_ovb0091* self, const CStrId* id, s32 value, bool now) {
    UnkGunWeapon_ovb0091* weapon = self->alt;
    if (!weapon || weapon->id.data == id->data)
        return;
    // Stored through an untyped path: the original reloads the id after it.
    __builtin_memcpy(&self->field_0x138, &value, sizeof(value));
    UnkSamusGunComponent* gun = reinterpret_cast<UnkSamusGunComponent*>(self);
    if (now) {
        fn_710083f32c(gun, id, true, true, false);
        fn_710083d27c(gun, true);
        return;
    }
    if (self->charge < 0.01f && VCall<bool>(weapon, 0x140)) {
        fn_710083f32c(gun, id, true, true, false);
        return;
    }
    self->id = *id;
}

void fn_710083d8f4(UnkGun_ovb0091* self, bool on) {
    UnkGunWeapon_ovb0091* weapon = ActiveWeapon(self);
    void* object = self->actor ? self->actor->field_0x260 : nullptr;
    if (weapon && VCall<bool, s32>(weapon, 0xb8, self->mode) && object && !fn_710113db74(object)) {
        if (self->charging == on)
            return;
        if (on) {
            UnkGunActor_ovb0091* actor = self->actor;
            if (actor && actor->flagged && fn_71009e75e8(actor->flagged))
                return;
            if (!self->charging)
                StartCharge(self);
            return;
        }
    }
    fn_7100839b00(reinterpret_cast<char*>(self), true);
}

void fn_710083db50(UnkGun_ovb0091* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    VCall<void>(self, 0x180);
    fn_710083c17c(self);
    VCall<void, f32>(self, 0x188, dt);
    if (self->field_0x12c > 0.0f) {
        f32 t = self->field_0x12c - dt;
        self->field_0x12c = t;
        // Cleared through an untyped store, so the clamp isn't folded into one store.
        if (t < 0.0f)
            __builtin_memset(&self->field_0x12c, 0, sizeof(f32));
    }
    UnkGunWeapon_ovb0091* weapon = ActiveWeapon(self);
    if (weapon)
        VCall<void, f32>(weapon, 0x108, dt);
    CallAll(self->primary, 0x110);
    CallAll(self->secondary, 0x110);
    if (self->field_0x130) {
        self->field_0x130 = 0;
        GunTunable()->field_0x98 = 1.0f;
        fn_710083d27c(reinterpret_cast<UnkSamusGunComponent*>(self), self->use_alt);
    }
}

bool fn_710083e250(UnkGun_ovb0091* self, f32 value, bool flag) {
    bool charging = self->charging;
    self->field_0x188 = 0;
    if (charging && self->charge >= self->charge_full)
        return false;
    UnkGunWeapon_ovb0091* weapon = ActiveWeapon(self);
    if (!weapon || !weapon->ready)
        return false;
    if (!charging) {
        StartCharge(self);
        VCall<void, s32, bool>(weapon, 0xc8, self->mode, false);
    }
    self->field_0x180 = 1;
    self->field_0x181 = 1;
    self->charge = self->charge_full;
    self->field_0x184 = value;
    self->field_0x189 = flag;
    VCall<void>(self, 0x190);
    return true;
}

// Ends a walk over the listeners: once the lock is back to zero, the removed entries leave the list
// and the added ones join it (as fn_710113831c).
void fn_7100841c4c(UnkGunListenersLock_ovb0091* self) {
    --self->listeners->lock;
    UnkGunListeners_ovb0091* listeners = self->listeners;
    if (listeners->lock > 0)
        return;
    for (u32 i = 0; i < listeners->removed.count; ++i)
        listeners->items.Remove(*fn_7100841e10(&listeners->removed, i));
    // Cleared through an untyped store: the original reloads the holder's pointer after it.
    __builtin_memset(&listeners->removed.count, 0, sizeof(u32));
    listeners = self->listeners;
    for (u32 i = 0; i < listeners->added.count; ++i)
        listeners->items.PushBack(*fn_7100841e10(&listeners->added, i));
    listeners->added.count = 0;
}

void fn_710083e33c(UnkChargeComponent_b47a3* charge, bool notify) {
    UnkGun_ovb0091* self = AsGun(charge);
    if (!self->charging)
        return;
    if (notify && VCall<bool>(self->alt, 0x48) && self->charging && self->charge >= self->charge_full) {
        if (self->field_0x2e0)
            fn_710083dd48(self, self->field_0x2e0);
        else
            self->field_0x12c = 0.12f;
        if (self->target && self->target->field_0x10) {
            s32 mode = self->mode;
            bool full = self->charging && self->charge >= self->charge_full;
            bool flag = self->field_0x189;
            UnkGunReleaseArgs_ovb0091 args;
            args.mode = mode;
            args.full = full;
            args.field_0x5 = flag;
            fn_7100831dcc(self->target->field_0x10, self->alt, false, &args);
        }
        {
            UnkGunListenersLock_ovb0091 lock;
            lock.listeners = &self->listeners;
            ++self->listeners.lock;
            void** it = self->listeners.items.data;
            u32 count = self->listeners.items.count;
            for (void** end = it + count; it != end; ++it) {
                auto* listener = static_cast<UnkGunListener_ovb0091*>(*it);
                if (!listener || !listener->callback || !listener->callback(listener->object, true))
                    fn_7100841838(reinterpret_cast<UnkGunListeners_b53*>(&self->listeners), listener);
            }
            fn_7100841c4c(&lock);
        }
    }
    CancelCharge(self);
}

void fn_710083eb58(UnkGun_ovb0091* self) {
    UnkGunWeapon_ovb0091* weapon = ActiveWeapon(self);
    if (!weapon)
        return;
    UnkInventory* inventory = self->actor->inventory;
    if (inventory && fn_71011428bc(inventory, ItemId_ovb0091(g_unk_717275a558)))  // ITEM_WEAPON_DIFFUSION_BEAM
        VCall<void, s32>(weapon, 0xd0, 3);
    else
        VCall<void, s32>(weapon, 0xd0, self->mode);
}

void fn_710083ed38(UnkGun_ovb0091* self) {
    UnkGunActor_ovb0091* actor = self->actor;
    if (actor->field_0x2b0)
        fn_710034f374(actor->field_0x2b0);
    actor = self->actor;
    if (actor) {
        UnkReflObject* animation = actor->animation;
        if (animation && fn_710009d50c(animation->vf_0x0(), fn_7100713bb0()))  // CSamusAnimationComponent
            *reinterpret_cast<s32*>(reinterpret_cast<char*>(animation) + 0x530) = 3;
    }
    if (!self->charging)
        return;
    if (self->charge >= self->charge_full) {
        actor = self->actor;
        UnkReflObject* animation = actor ? actor->animation : nullptr;
        // CCharClassGunUnpossessCheckCantFireTrack
        if (!animation ||
            !fn_710117b348(reinterpret_cast<UnkAnimationComponent*>(animation), fn_7100d01a98())) {
            fn_710083e33c(reinterpret_cast<UnkChargeComponent_b47a3*>(self), true);
            return;
        }
        EndCharge(self);
        return;
    }
    EndCharge(self);
}

// Reflected call (its arguments read from the stream): seven strings, a float and two numbers.
// Returns true (the clone declares it void).
bool fn_7100840064(char* self, const void* stream) {
    CRntFile* file = reinterpret_cast<CRntFile*>(const_cast<void*>(stream));
    char a[0x80];
    char b[0x80];
    char c[0x80];
    f32 value = 0.0f;
    char d[0x80];
    char e[0x80];
    char h[0x80];
    char f[0x80];
    char g[0x80];
    s32 i = -1;
    s32 j = -1;
    fn_710007ca94(file, a);
    fn_710007ca94(file, b);
    fn_710007ca94(file, c);
    fn_710007c714(file, &value, 4, 1);
    fn_710007ca94(file, d);
    fn_710007ca94(file, e);
    fn_710007ca94(file, f);
    fn_710007ca94(file, g);
    fn_710007c714(file, &i, 4, 1);
    fn_710007c714(file, &j, 4, 1);
    fn_710007ca94(file, h);
    fn_710083b83c(self, a, b, c, value, d, e, h, f, g, i, j);
    return true;
}

// Reflected call: a name and a flag ("sb"); drops the charge, then hands the name's id to
// fn_710083f32c. Returns true (the clone declares it void).
bool fn_710084017c(char* object, const void* stream) {
    UnkGun_ovb0091* self = AsGun(object);
    char name[0x80];
    bool flag = true;
    fn_710007cf30(const_cast<void*>(stream), "sb", name, &flag);
    CancelCharge(self);
    CStrId id(name, true);
    fn_710083f32c(reinterpret_cast<UnkSamusGunComponent*>(self), &id, flag, false, false);
    return true;
}

void* fn_710083a2ac(UnkGunActor_ovb0091* actor);
void fn_710093aad0(void*);

void fn_7100839d70(UnkGun_ovb0091* self) {
    CancelCharge(self);
    if (self->alt)
        VCall<void>(self->alt, 0x38);
    if (self->main)
        VCall<void>(self->main, 0x38);
    fn_7100839f68(self, GameManagerIdTable().field_0x1240);
    fn_710083a034(self, false, false);
    if (void* object = fn_710083a2ac(self->actor))
        fn_710093aad0(object);
}

extern const u64 g_unk_710164ac60;
extern const u64 g_unk_710164ac68;

// The first weapon of the groups with the given id that accepts it (vf_0x30); null if none.
static inline UnkGunWeapon_ovb0091* FindWeapon(UnkGunGroups_ovb0091& groups, const CStrId& id) {
    for (UnkGunWeaponList_ovb0091* g = groups.data; g != groups.data + groups.count; ++g) {
        for (UnkGunWeapon_ovb0091** it = g->data; it != g->data + g->count; ++it) {
            if ((*it)->id.data == id.data && VCall<bool>(*it, 0x30))
                return *it;
        }
    }
    return nullptr;
}

void fn_710083ae88(UnkGun_ovb0091* self, bool on) {
    if (self->field_0x18a == on)
        return;
    UnkGunActor_ovb0091* actor = self->actor;
    self->field_0x18c = 0;
    self->field_0x18a = on;
    UnkTimelineComponent* timeline = actor ? actor->timeline : nullptr;
    if (on) {
        UnkGunWeapon_ovb0091* weapon = FindWeapon(self->secondary, GameManagerIdTable().field_0x4c8);
        if (!weapon || VCall<bool>(weapon, 0x60))
            self->field_0x18a = 0;
        return;
    }
    fn_710083a034(self, false, true);
    fn_710083f32c(reinterpret_cast<UnkSamusGunComponent*>(self), &GameManagerIdTable().field_0x1640, false, false,
                  true);
    if (timeline) {
        fn_7100a2b7b8(timeline, &g_unk_710164ac60);
        fn_7100a2b7b8(timeline, &g_unk_710164ac68);
    }
}

s64 fn_710007cee4(void*, const char**);
void* fn_710034df0c(char* self, const CStrId* id);
void fn_7100835bd8(void* object, const u64* a, const u64* b, const u64* c, const u64* d, const u64* e,
                   const u64* f, const u64* g);

static inline u64 HashOf(const char* str) {
    return fn_7100001570(str, __builtin_strlen(str));
}

// Reflected call: nine strings from the stream; the first names the object (fn_710034df0c), seven
// of the others are passed hashed to fn_7100835bd8. Returns true (the clone declares it void).
bool fn_71008402ac(char* self, const void* stream) {
    void* file = const_cast<void*>(stream);
    fn_710007c00c(reinterpret_cast<CRntFile*>(file), 0, 0);
    const char* name;
    const char* b;
    const char* c;
    const char* unused;
    const char* e;
    const char* f;
    const char* g;
    const char* h;
    const char* i;
    fn_710007cee4(file, &name);
    fn_710007cee4(file, &b);
    fn_710007cee4(file, &c);
    fn_710007cee4(file, &unused);
    fn_710007cee4(file, &e);
    fn_710007cee4(file, &f);
    fn_710007cee4(file, &g);
    fn_710007cee4(file, &h);
    fn_710007cee4(file, &i);
    CStrId id(name, true);
    if (void* object = fn_710034df0c(self, &id)) {
        u64 hb = HashOf(b);
        u64 hc = HashOf(c);
        u64 he = HashOf(e);
        u64 hf = HashOf(f);
        u64 hg = HashOf(g);
        u64 hh = HashOf(h);
        u64 hi = HashOf(i);
        fn_7100835bd8(object, &hb, &hc, &he, &hf, &hg, &hh, &hi);
    }
    return true;
}
