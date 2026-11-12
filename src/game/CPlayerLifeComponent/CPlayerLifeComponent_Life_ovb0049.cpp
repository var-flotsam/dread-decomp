// CPlayerLifeComponent's unit (tools/tu_map.py unit 2716): the player's life component. Its constructor
// (fn_71011647b0) and destructor body (fn_7101164904), the reset (fn_7101164a78) and the takeover of
// another actor's state (fn_7101164b24), the update (fn_7101164e40), the low-life timeline ids
// (fn_7101167de4), the death message (fn_7101169a18), the life shards (fn_71011697a0) and whether the
// AI manager blocks damage (fn_7101165e74). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CCharacterLifeComponent.h"
#include "game/CInventoryComponent.h"
#include "game/CLifeComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU39.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkInventory;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
void fn_710002f4e4(void*);
f32 fn_7101142988(UnkInventory* inventory, const UnkQualifiedId& item, f32 fallback);
struct UnkStateSink;
struct UnkStateKey;
struct UnkEnumType;
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);

extern const u64 g_unk_710164d050;
extern const u64 g_unk_710164d058;
extern const u64 g_unk_710164d060;
extern char g_unk_7101cb3f98[];
extern UnkStateSink* g_unk_717275c380;
extern UnkEnumType g_unk_71729f02a8;
extern UnkPropertyEx<> g_unk_717275a0d8;

// The message sent when the player dies (built by fn_71008c0c74, destroyed by fn_71008c1558).
struct UnkLifeMessage_ovb0049 {
    u8 pad_0x0[0x48];
    bool field_0x48;
    u8 pad_0x49[0xb7];

    UnkLifeMessage_ovb0049() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkLifeMessage_ovb0049() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkLifeMessage_ovb0049) == 0x100, "message layout");

struct UnkLifeArg_ovb0049 {
    u64 field_0x0;
    u32 field_0x8;
};

struct UnkLifeTarget_ovb0049 {
    void** vtable;
    void vf_0x1e8(bool value) {
        reinterpret_cast<void (*)(UnkLifeTarget_ovb0049*, bool)>(vtable[0x1e8 / 8])(this, value);
    }
};

struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;

struct UnkPlayerLifeActor_ovb0049 {
    u8 pad_0x0[0x200];
    UnkLifeTarget_ovb0049* field_0x200;
    u8 pad_0x208[0x68];
    UnkInventory* inventory;  // 0x270
};

struct UnkPlayerLifeOwner_ovb0049 {
    u8 pad_0x0[0xa4];
    f32 field_0xa4;
};

struct UnkPlayerLife_ovb0049 {
    char* vtable;
    u8 pad_0x8[0x28];
    UnkPlayerLifeActor_ovb0049* actor;  // 0x30
    u8 pad_0x38[0x18];
    f32 max_life;  // 0x50
    f32 life;      // 0x54
    u8 pad_0x58[0xc];
    bool field_0x64;
    u8 pad_0x65[0x7];
    bool field_0x6c;
    u8 pad_0x6d[0x3];
    s32 state;  // 0x70
    u8 pad_0x74[0x1d4];
    u8 timer_0x248[0xb8];
    CStrId id_0x300;
    f32 field_0x308;
    f32 field_0x30c;
    f32 field_0x310;
    s32 field_0x314;
    f32 shards;  // 0x318
    bool field_0x31c;
    u8 pad_0x31d[0x3];
    f32 field_0x320;
    bool field_0x324;
    u8 pad_0x325[0x3];
    CFilePathStrId id_0x328;
    bool field_0x330;
    u8 pad_0x331[0x3];
    f32 field_0x334;
    s32 field_0x338;
    f32 field_0x33c;
    s32 field_0x340;
    s32 field_0x344;
    bool flag_0x348;
    bool field_0x349;
    u8 pad_0x34a[0x6];
    u8 message_0x350[0x100];
    bool field_0x450;
    u8 pad_0x451[0x7];
    UnkPlayerLifeOwner_ovb0049* owner;  // 0x458
    f32 field_0x460;
    u8 pad_0x464[0x4];
    u8 timer_0x468[0xb0];
    bool field_0x518;
    bool field_0x519;
    bool field_0x51a;

    UnkActor* Actor() { return reinterpret_cast<UnkActor*>(actor); }

    void vf_0x168(bool flag, f32 value) {
        reinterpret_cast<void (*)(UnkPlayerLife_ovb0049*, bool, f32)>(
            reinterpret_cast<void**>(vtable)[0x168 / 8])(this, flag, value);
    }
    void vf_0x170(f32 value) {
        reinterpret_cast<void (*)(UnkPlayerLife_ovb0049*, f32)>(reinterpret_cast<void**>(vtable)[0x170 / 8])(
            this, value);
    }
    void vf_0x1d0(s32 kind, UnkLifeMessage_ovb0049* message) {
        reinterpret_cast<void (*)(UnkPlayerLife_ovb0049*, s32, UnkLifeMessage_ovb0049*)>(
            reinterpret_cast<void**>(vtable)[0x1d0 / 8])(this, kind, message);
    }
};
static_assert(__builtin_offsetof(UnkPlayerLife_ovb0049, id_0x328) == 0x328, "layout");
static_assert(__builtin_offsetof(UnkPlayerLife_ovb0049, message_0x350) == 0x350, "layout");
static_assert(__builtin_offsetof(UnkPlayerLife_ovb0049, owner) == 0x458, "layout");
static_assert(__builtin_offsetof(UnkPlayerLife_ovb0049, field_0x51a) == 0x51a, "layout");

void fn_7101164f0c(UnkPlayerLife_ovb0049* self, f32 value);
void fn_7101164cc0(UnkPlayerLife_ovb0049* self);
void fn_7101165180(UnkPlayerLife_ovb0049* self);
void fn_7101158cb8(UnkPlayerLife_ovb0049* self, f32 dt);
struct UnkEnemyLifeDtor_ovb0043;
void fn_7101158c18(UnkPlayerLife_ovb0049* self);

void fn_71011647b0(UnkPlayerLife_ovb0049* self) {
    fn_7101158c18(self);
    self->vtable = g_unk_7101cb3f98 + 0x10;
    fn_710002f4e4(self->timer_0x248);
    new (&self->id_0x300) CStrId(g_unk_710162e2ed, true);
    self->field_0x320 = 0.0f;
    self->field_0x308 = 0.3f;
    self->field_0x30c = 100.0f;
    self->field_0x324 = false;
    // The empty file path (CFilePathStrId's default constructor), with the fields 0x310..0x31c
    // cleared in between.
    self->id_0x328.data = nullptr;
    __builtin_memset(&self->field_0x310, 0, 0xd);
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->id_0x328.data = &g_unk_7101d47200->empty;
    self->field_0x330 = false;
    self->field_0x334 = 0.0f;
    self->field_0x338 = 0;
    self->field_0x33c = 0.0f;
    self->field_0x340 = 1;
    self->field_0x344 = 0;
    self->flag_0x348 = false;
    self->field_0x349 = false;
    fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(self->message_0x350));
    self->field_0x450 = false;
    self->owner = nullptr;
    self->field_0x460 = -1.0f;
    fn_710002f4e4(self->timer_0x468);
    self->field_0x518 = false;
    self->field_0x519 = false;
    self->field_0x51a = true;
}

// Releases a string id in two steps (the count is reloaded after the decrement) and leaves the
// pool's empty id.
static inline void ReleaseId(UnkStringData** data, UnkStringPool** pools) {
    if (UnkStringPool* pool = *pools) {
        if (*data && *data != &pool->empty) {
            --(*data)->ref_count;
            if ((*data)->ref_count == 0) {
                fn_71000803a4(pool, *data);
                pool = *pools;
            }
        }
        *data = &pool->empty;
    }
}

void fn_7101164904(UnkPlayerLife_ovb0049* self) {
    self->vtable = g_unk_7101cb3f98 + 0x10;
    fn_710002f67c(self->timer_0x468);
    fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(self->message_0x350));
    ReleaseId(&self->id_0x328.data, &g_unk_7101d47200);
    ReleaseId(&self->id_0x300.data, &g_unk_7101d47108);
    fn_710002f67c(self->timer_0x248);
    fn_7101158c4c(reinterpret_cast<UnkEnemyLifeDtor_ovb0043*>(self));
}

// Restarts the hit timing when a grace period is left (+0x334), else clears it and the timeline
// ids it set.
static inline void Restart(UnkPlayerLife_ovb0049* self) {
    if (self->field_0x334 > 0.0f) {
        const f32* total = &self->owner->field_0xa4;
        self->field_0x33c = 0.0f;
        self->field_0x33c = *total - self->field_0x334;
        fn_7101164f0c(self, 0.0f);
        return;
    }
    self->field_0x334 = 0.0f;
    self->field_0x340 = 0;
    self->field_0x344 = 0;
    fn_7100d7c94c(self->Actor(), &g_unk_710164d058);
    fn_7100d7c94c(self->Actor(), &g_unk_710164d060);
    if (!self->flag_0x348)
        fn_7100d7c94c(self->Actor(), &g_unk_710164d050);
}

void fn_7101164a78(UnkPlayerLife_ovb0049* self) {
    fn_710115fc5c(reinterpret_cast<char*>(self));
    fn_710002fa90(self->timer_0x468);
    fn_710002fa90(self->timer_0x248);
    Restart(self);
}

struct UnkPlayerLifeSource_ovb0049 {
    u8 pad_0x0[0x1c0];
    UnkPlayerLife_ovb0049* life;  // 0x1c0
    u8 pad_0x1c8[0x120];
    UnkTimelineComponent* timeline;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkPlayerLifeSource_ovb0049, timeline) == 0x2e8, "layout");

// Moves a timeline id that `timeline` has over to this component's actor.
static inline void TakeId(UnkPlayerLife_ovb0049* self, UnkTimelineComponent* timeline, const u64* key) {
    if (fn_7100a2be94(timeline, key)) {
        fn_7100a2b7b8(timeline, key);
        fn_7100d7c828(self->Actor(), key, false);
    }
}

void fn_7101164b24(UnkPlayerLife_ovb0049* self, UnkPlayerLifeSource_ovb0049* source) {
    Restart(self);
    if (!source) {
        fn_7101164cc0(self);
        return;
    }
    if (UnkTimelineComponent* timeline = source->timeline) {
        TakeId(self, timeline, &g_unk_710164d050);
        TakeId(self, timeline, &g_unk_710164d058);
        TakeId(self, timeline, &g_unk_710164d060);
    }
    fn_7101164cc0(self);
    if (UnkPlayerLife_ovb0049* life = source->life) {
        const u8* from = life->timer_0x468;
        fn_710002f90c(self->timer_0x468, from);
        fn_710002fa90(source->life->timer_0x468);
    }
}

void fn_7101164e40(UnkPlayerLife_ovb0049* self, f32 dt) {
    fn_710002fb80(self->timer_0x468, dt);
    if (self->field_0x320 > 0.0f) {
        self->field_0x320 -= dt;
        if (self->field_0x320 <= 0.0f) {
            self->field_0x320 = 0.0f;
            self->field_0x64 = false;
        }
    }
    fn_7101164f0c(self, dt);
    if (self->field_0x460 > -0.001f)
        self->field_0x460 += dt;
    if (self->field_0x324) {
        self->field_0x324 = false;
        fn_7101165180(self);
    }
    if (self->field_0x518 && !fn_7101053a0c(g_unk_7101d47310->field_0x1040)) {
        fn_7101164cc0(self);
        self->field_0x518 = false;
    }
    fn_7101158cb8(self, dt);
}

struct UnkManagerHandle_ovb0049 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

static inline UnkManager_71726af468* Manager() {
    if (!g_unk_71726af468) {
        auto* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

static inline UnkManagerHandle_ovb0049* Handle(UnkManager_71726af468* m) {
    return *reinterpret_cast<UnkManagerHandle_ovb0049**>(reinterpret_cast<char*>(m) + 0x2410);
}

bool fn_7101165e74(UnkPlayerLife_ovb0049* self) {
    UnkManagerHandle_ovb0049* handle = Handle(Manager());
    if (handle && handle->field_0x10)
        return true;
    if (fn_7100da9254(Manager()))
        return true;
    return self->field_0x6c;
}

void fn_7101167de4(UnkPlayerLife_ovb0049* self, bool on) {
    if (self->flag_0x348 == on)
        return;
    if (on) {
        if (self->state == 2)
            return;
        self->flag_0x348 = on;
        fn_7100d7c828(self->Actor(), &g_unk_710164d050, false);
    } else {
        self->flag_0x348 = on;
        fn_7100d7c94c(self->Actor(), &g_unk_710164d050);
        self->field_0x338 = 0;
    }
}

static inline const UnkQualifiedId& ItemId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_71011697a0(UnkPlayerLife_ovb0049* self, f32 shards) {
    UnkInventory* inventory = self->actor->inventory;
    self->shards = shards;
    if (shards >= 4.0f) {
        if (UnkIngameMenu* menu = fn_7100deac84(g_unk_7101d47310))
            fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f02a8), menu->field_0x2d0);
        self->field_0x51a = false;
        self->shards -= 4.0f;
        self->vf_0x170(self->max_life + 100.0f);
        self->vf_0x168(true, self->max_life);
        self->field_0x51a = true;
        s32 tanks = (s32)(self->life / self->field_0x30c);
        self->field_0x314 = tanks < 0 ? 0 : tanks;
        if (inventory && self->shards <= 0.0f) {
            fn_7101142a28(inventory, ItemId(g_unk_717275a098), true, true, true, 1.0f, 0.0f);
            fn_7101142a28(inventory, ItemId(g_unk_717275a098), true, true, true, 0.0f, 0.0f);
        }
    } else {
        fn_7101142a28(inventory, ItemId(g_unk_717275a098), true, true, true, self->shards, 0.0f);
    }
    self->field_0x31c = fn_7101142988(inventory, ItemId(g_unk_717275a0d8), 0.0f) == 16.0f;
}

void fn_7101169a18(UnkPlayerLife_ovb0049* self, bool flag) {
    if (self->state == 2)
        return;
    self->life = 0.0f;
    UnkLifeMessage_ovb0049 message;
    message.field_0x48 = !flag;
    self->vf_0x1d0(2, &message);
    if (UnkLifeTarget_ovb0049* target = self->actor->field_0x200) {
        UnkLifeArg_ovb0049 arg;
        arg.field_0x0 = 0;
        arg.field_0x8 = 0;
        fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(target),
                      reinterpret_cast<const UnkMovementVec_71010d08d4*>(&arg), true);
        target->vf_0x1e8(true);
    }
    self->field_0x64 = true;
}
