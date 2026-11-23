// CCooldownXBossLavaCarpetAttack's unit (tools/tu_map.py unit 747): the attack's update. Runs the damage
// cooldown down, drops the records' render items, clears the hits, and while a record burns, outlines the
// grown ones, damages the player standing in the outlines and paints up to four burning spans onto the
// boss's floor model. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBossAIComponent.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::CClass;

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
struct UnkSlots;
struct UnkLavaAttackOutlines_ovb0059;
struct UnkLavaOutlines_l4c493c;

extern CStrId g_unk_71726d7b68 __attribute__((visibility("hidden")));

bool fn_7100d635fc(UnkLavaOutlines_l4c493c* outlines, const CVector2D* point, f32 margin);
void fn_71004c51e4(UnkLavaAttackOutlines_ovb0059* self, const CVector3D* from, const CVector3D* to);
void fn_710116a168(void* target, void* key);
void fn_710116a170(void* target, void* key);
bool fn_71001c2a2c(UnkSlots* self, u32 kind, const CColor4F* color, u32 mode);

// The damage request sent to the player's damage receiver.
struct UnkLavaRequest_l4c493c {
    u8 pad_0x0[0x21];
    bool field_0x21;
    u8 pad_0x22[0x7];
    bool field_0x29;
    u8 pad_0x2a[0x2];
    f32 cooldown;  // 0x2c
    u8 pad_0x30[0x84];
    u32 flags;  // 0xb4
    u8 id_raw[0x8];  // 0xb8: a CStrId
    u8 pad_0xc0[0x40];

    CStrId& Id() { return *reinterpret_cast<CStrId*>(id_raw); }

    UnkLavaRequest_l4c493c() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkLavaRequest_l4c493c() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkLavaRequest_l4c493c) == 0x100, "request layout");

struct UnkLavaReceiver_l4c493c {
    void** vtable;
    void vf_0x188(UnkLavaRequest_l4c493c* request) {
        reinterpret_cast<void (*)(UnkLavaReceiver_l4c493c*, UnkLavaRequest_l4c493c*)>(vtable[0x188 / 8])(this,
                                                                                                  request);
    }
};

struct UnkLavaMorph_l4c493c {
    u8 pad_0x0[0xf9];
    bool morphed;  // 0xf9
};

// The player as fn_7100d7c990 gives it.
struct UnkLavaPlayer_l4c493c {
    u8 pad_0x0[0x1c0];
    UnkLavaReceiver_l4c493c* receiver;  // 0x1c0
    u8 pad_0x1c8[0x28];
    UnkLavaMorph_l4c493c* morph;  // 0x1f0
};

static inline UnkLavaPlayer_l4c493c* Player(UnkActor* owner) {
    return reinterpret_cast<UnkLavaPlayer_l4c493c*>(fn_7100d7c990(owner));
}

struct UnkLavaOwner_l4c493c {
    u8 pad_0x0[0x30];
    u8 field_0x30[0x8];
};

struct UnkLavaItem_l4c493c {
    void* field_0x0;
    u8 render[0x8];  // 0x8: the item the render manager holds
};

// A stretch of the carpet: whether it shows, where it runs from and to, how long it has burnt and for
// how long it burns. The records sit 4 bytes after their count, so the item pointer is only 4-aligned.
struct __attribute__((packed, aligned(4))) UnkLavaRecord_l4c493c {
    bool visible;
    u8 pad_0x1[0x3];
    CVector3D from;  // 0x4
    CVector3D to;  // 0x10
    f32 time;  // 0x1c
    f32 duration;  // 0x20
    bool active;  // 0x24
    u8 pad_0x25[0x3];
    UnkLavaItem_l4c493c* item;  // 0x28

    bool Burning() const { return active && visible && time > 0.0f; }
};
static_assert(sizeof(UnkLavaRecord_l4c493c) == 0x30, "record layout");

// Up to 30 records after their count.
struct UnkLavaRecords_l4c493c {
    u32 count;
    u8 storage[30 * sizeof(UnkLavaRecord_l4c493c)];

    UnkLavaRecord_l4c493c* begin() { return reinterpret_cast<UnkLavaRecord_l4c493c*>(storage); }
    UnkLavaRecord_l4c493c* end() { return begin() + count; }
};

struct UnkLavaHitObject_l4c493c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void Reset(void* target);  // 0x48
};

struct UnkLavaHit_l4c493c {
    void* target;
    u32 field_0x8;
    u32 field_0xc;
    u8 pad_0x10[0x10];
    UnkLavaHitObject_l4c493c* object;  // 0x20
    u8 pad_0x28[0x20];
};
static_assert(sizeof(UnkLavaHit_l4c493c) == 0x48, "hit layout");

struct UnkLavaParams_l4c493c {
    u8 pad_0x0[0x12c];
    f32 damage_cooldown;  // 0x12c
};

struct UnkLavaCarpet_l4c493c {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x58];
    UnkLavaParams_l4c493c* params;  // 0x68
    u8 pad_0x70[0x30];
    u8 outlines[0xa8];  // 0xa0: the outline set; the hits below are part of it
    UnkLavaHit_l4c493c* hits;  // 0x148
    u32 hit_count;  // 0x150
    u8 pad_0x154[0x2c];
    bool field_0x180;
    u8 pad_0x181[0x57];
    UnkLavaRecords_l4c493c records;  // 0x1d8
    u8 pad_0x77c[0x4];
    f32 cooldown;  // 0x780
    bool field_0x784;
    bool damaging;  // 0x785

};
static_assert(__builtin_offsetof(UnkLavaCarpet_l4c493c, hits) == 0x148, "layout");
static_assert(__builtin_offsetof(UnkLavaCarpet_l4c493c, records) == 0x1d8, "layout");
static_assert(__builtin_offsetof(UnkLavaCarpet_l4c493c, cooldown) == 0x780, "layout");

struct UnkLavaBossAI_l4c493c {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x156c8];
    u8 doors[0x8];  // 0x156d0
};

struct UnkLavaBossEntity_l4c493c {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkLavaFloorPart_l4c493c {
    u8 pad_0x0[0xb];
    u8 index;  // 0xb
    u8 pad_0xc[0x154];
    UnkSlots* slots;  // 0x160
};

static inline f32 AtMost3(f32 v) {
    return !(v >= 3.0f) ? v : 3.0f;
}

static inline f32 NotNegative(f32 v) {
    return v > 0.0f ? v : 0.0f;
}

// Sets slot `kind` of the floor model's material to `value`, no component below zero.
static inline void SetSlot(UnkSlots* slots, u32 kind, const CColor4F& value) {
    CColor4F color(NotNegative(value.r), NotNegative(value.g), NotNegative(value.b), NotNegative(value.a));
    fn_71001c2a2c(slots, kind, &color, 0);
}

// Whether the player stands inside the outlines of the grown stretches.
static inline bool PlayerOnCarpet(UnkLavaCarpet_l4c493c* self) {
    UnkActor* player = fn_7100d7c990(self->owner);
    CVector3D position;
    position.x = player->field_0x44;
    position.y = player->field_0x54;
    position.z = player->field_0x64;
    CVector2D point;
    fn_710007b430(&point, &position);
    return fn_7100d635fc(reinterpret_cast<UnkLavaOutlines_l4c493c*>(self->outlines), &point, 0.0f);
}

void fn_71004c493c(UnkLavaCarpet_l4c493c* self, f32 dt) {
    if (self->cooldown > 0.0f) {
        self->cooldown -= dt;
        if (self->cooldown < 0.0f)
            self->cooldown = 0.0f;
    }

    for (UnkLavaRecord_l4c493c& record : self->records) {
        if (record.item) {
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(record.item->render), 0);
            record.item = nullptr;
        }
    }

    bool burning = false;
    for (UnkLavaRecord_l4c493c& record : self->records) {
        if (record.Burning()) {
            burning = true;
            break;
        }
    }

    for (u32 i = 0; i < self->hit_count; ++i) {
        UnkLavaHit_l4c493c& hit = self->hits[i];
        if (hit.target) {
            hit.field_0x8 = 0;
            hit.object->Reset(hit.target);
            hit.field_0xc = 0;
        }
    }
    self->hit_count = 0;
    self->field_0x180 = true;

    if (burning) {
        for (UnkLavaRecord_l4c493c& record : self->records) {
            if (!record.Burning() || record.time < 0.75f)
                continue;
            fn_71004c51e4(reinterpret_cast<UnkLavaAttackOutlines_ovb0059*>(self), &record.from, &record.to);
        }

        if (UnkLavaMorph_l4c493c* morph = Player(self->owner)->morph) {
            bool damaging;
            if (!morph->morphed && PlayerOnCarpet(self)) {
                if (self->cooldown == 0.0f) {
                    self->cooldown = self->params->damage_cooldown;
                    UnkLavaRequest_l4c493c request;
                    request.Id() = g_unk_71726d7b68;
                    fn_71008c1538(reinterpret_cast<UnkRequest*>(&request),
                                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) +
                                                                0x1a0));
                    request.flags = 0x10000000;
                    request.field_0x21 = true;
                    request.field_0x29 = true;
                    request.cooldown = self->cooldown;
                    Player(self->owner)->receiver->vf_0x188(&request);
                    fn_710116a168(Player(self->owner)->receiver,
                                  reinterpret_cast<UnkLavaOwner_l4c493c*>(self->owner)->field_0x30);
                }
                damaging = true;
            } else {
                if (self->damaging) {
                    fn_710116a170(Player(self->owner)->receiver,
                                  reinterpret_cast<UnkLavaOwner_l4c493c*>(self->owner)->field_0x30);
                    self->cooldown = 0.0f;
                }
                damaging = false;
            }
            self->damaging = damaging;
        }
    }

    // Up to four burning spans: where each starts and ends along x, and how strongly it glows.
    f32 starts[4] = {5000.0f, 5000.0f, 5000.0f, 5000.0f};
    f32 ends[4] = {5000.0f, 5000.0f, 5000.0f, 5000.0f};
    f32 glows[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    u32 spans = 0;
    for (UnkLavaRecord_l4c493c& record : self->records) {
        if (!record.Burning())
            continue;
        f32 left = record.time - record.duration;
        if (!(left + 0.001f < 0.0f || left + -0.001f > 0.0f))
            continue;
        if (spans < 4) {
            starts[spans] = record.from.x < record.to.x ? record.from.x : record.to.x;
            ends[spans] = record.from.x < record.to.x ? record.to.x : record.from.x;
            f32 fade = 1.0f - AtMost3(record.time) / AtMost3(record.duration);
            glows[spans] = 1.0f - fade * fade;
        }
        ++spans;
    }
    CColor4F start_color(starts[0], starts[1], starts[2], starts[3]);
    CColor4F end_color(ends[0], ends[1], ends[2], ends[3]);
    CColor4F glow_color(glows[0], glows[1], glows[2], glows[3]);

    UnkLavaBossAI_l4c493c* ai = static_cast<UnkLavaBossAI_l4c493c*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71004cf318()))
        return;
    UnkLavaBossEntity_l4c493c* boss =
        reinterpret_cast<UnkLavaBossEntity_l4c493c*>(*fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(ai->doors)));
    if (!boss || !boss->actor)
        return;
    void* world = boss->actor->field_0x280;
    if (!world)
        return;
    UnkLavaFloorPart_l4c493c* floor =
        reinterpret_cast<UnkLavaFloorPart_l4c493c*>(fn_7101152b90(static_cast<UnkWorld*>(world),
                                                                   StringCrc64("floormodel", 10)));
    if (floor && !fn_710114e680(static_cast<UnkObject*>(world), floor->index))
        floor = nullptr;

    SetSlot(floor->slots, 0, start_color);
    SetSlot(floor->slots, 1, end_color);
    SetSlot(floor->slots, 2, glow_color);
}
