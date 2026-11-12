// Components that give their owner's timeline (owner +0x30, timeline +0x2e8) a key with the all-flags
// mask and no delay, unless it already holds it (fn_7100a2cfe8): one key passed in
// (CDoorLifeComponent's unit) or one of two fixed hashes picked by a flag (CCharClassLavaPoolComponent's
// unit); CCharClassWaterPoolComponent's unit picks one of two keys reached through the GOT and sets
// it without the test. Names are placeholders.
#include "../game_s2/UnkSpitSend.h"
#include "UnkLeftoverReg.h"
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "game/CAutoolAIComponent.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkTimelineOwnerA5_b61 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* timeline;
};

struct UnkTimelineUserA5_b61 {
    u8 pad_0x0[0x30];
    UnkTimelineOwnerA5_b61* owner;
};

static inline void SetAll_A5_b61(UnkTimelineComponent* timeline, const u64* key) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
}

void fn_71008d2c28(UnkTimelineUserA5_b61* self, const u64* key) {
    UnkTimelineComponent* timeline = self->owner->timeline;
    if (timeline && !fn_7100a2cfe8(timeline, key))
        SetAll_A5_b61(timeline, key);
}

static inline void SetPicked_A5_b61(UnkTimelineUserA5_b61* self, bool on, u64 key_on, u64 key_off) {
    UnkTimelineOwnerA5_b61* owner = self->owner;
    if (!owner)
        return;
    UnkTimelineComponent* timeline = owner->timeline;
    if (!timeline)
        return;
    u64 key = on ? key_on : key_off;
    if (!fn_7100a2cfe8(timeline, &key))
        SetAll_A5_b61(timeline, &key);
}

void fn_71008fcc18(UnkTimelineUserA5_b61* self, bool on) {
    SetPicked_A5_b61(self, on, 0x92048091723f117f, 0x96df48769ef15b7e);
}

void fn_71008fcd3c(UnkTimelineUserA5_b61* self, bool on) {
    SetPicked_A5_b61(self, on, 0x4dae1ba9d9bcd8fb, 0x2e8fe5d43f230a50);
}

extern u64 g_unk_710164b1f8;
extern u64 g_unk_710164b200;

void fn_7100909e78(UnkTimelineUserA5_b61* self, bool on) {
    UnkTimelineOwnerA5_b61* owner = self->owner;
    if (!owner)
        return;
    UnkTimelineComponent* timeline = owner->timeline;
    if (!timeline)
        return;
    SetAll_A5_b61(timeline, on ? &g_unk_710164b1f8 : &g_unk_710164b200);
}

// CEvacuationCountDown's unit: every entry of the list fn_71007d5d8c returns for `id` whose object's
// (+0x10) part (+0x18) has a timeline (fn_71003c6c3c) gets `value` at +0x48 and the key
// g_unk_710164ab40 set.
struct UnkEvacPart_b61;
struct UnkEvacTimeline_b61 {
    u8 pad_0x0[0x48];
    u32 field_0x48;
};
struct UnkEvacObject_b61 {
    u8 pad_0x0[0x18];
    UnkEvacPart_b61* part;
};
struct UnkEvacEntry_b61 {
    u8 pad_0x0[0x10];
    UnkEvacObject_b61* object;
};
struct UnkEvacList_b61 {
    UnkEvacEntry_b61** data;
    u32 count;
};
UnkEvacList_b61* fn_71007d5d8c(u32 id);
extern u64 g_unk_710164ab40;

void fn_71007da43c(UnkLeftoverReg, u32 id, u32 value) {
    UnkEvacList_b61* list = fn_71007d5d8c(id);
    for (UnkEvacEntry_b61** it = list->data, **end = list->data + list->count; it != end; ++it) {
        UnkEvacEntry_b61* entry = *it;
        if (!entry || !entry->object || !entry->object->part)
            continue;
        UnkEvacTimeline_b61* timeline = fn_71003c6c3c(entry->object->part);
        if (!timeline)
            continue;
        timeline->field_0x48 = value;
        SetAll_A5_b61(reinterpret_cast<UnkTimelineComponent*>(timeline), &g_unk_710164ab40);
    }
}

// CLockOnMissileGun's unit: resets (fn_710084f894), releases the held references (0x18-byte entries
// from +0x1924, count at +0x1920), clears the state and sets the owner's (+0x18) timeline key
// g_unk_710164ad08.
struct UnkMissileGunA5_b61;
void fn_710084f894(UnkMissileGunA5_b61* self);
extern u64 g_unk_710164ad08;

struct UnkMissileGunA5_b61 {
    u8 pad_0x0[0x18];
    UnkTimelineOwnerA5_b61* owner;
    u8 pad_0x20[0x1920 - 0x20];
    u32 count;
    u8 entries[0x199c - 0x1924];
    bool field_0x199c;
    u8 pad_0x199d[0x19b8 - 0x199d];
    u64 field_0x19b8;

    base::UnkRefPtrMember& Entry(u64 i) {
        return *reinterpret_cast<base::UnkRefPtrMember*>(entries + i * 0x18);
    }
};

void fn_710084d4e0(UnkMissileGunA5_b61* self) {
    fn_710084f894(self);
    for (u64 i = 0; i < self->count; ++i) {
        self->Entry(i).~UnkRefPtrMember();
        asm("" ::: "memory");  // the original rereads the count after every entry
    }
    self->count = 0;
    self->field_0x199c = false;
    self->field_0x19b8 = 0;
    SetAll_A5_b61(self->owner->timeline, &g_unk_710164ad08);
}

// CElectrifyingAreaComponent's unit: refreshes (fn_71007d17d4), stores fn_71007d191c's result at
// +0x118, copies the owner's transform matrix (owner +0x38) to +0x40 and sets the timeline key
// g_unk_710164ab10.
struct UnkElectrifyingAreaA5_b61;
void fn_71007d17d4(UnkElectrifyingAreaA5_b61* self);
void* fn_71007d191c(UnkElectrifyingAreaA5_b61* self);
extern const u64 g_unk_710164ab10 __attribute__((visibility("hidden")));

struct UnkElectrifyingOwnerA5_b61 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;
    u8 pad_0x78[0x2e8 - 0x78];
    UnkTimelineComponent* timeline;
};

struct UnkElectrifyingAreaA5_b61 {
    u8 pad_0x0[0x30];
    UnkElectrifyingOwnerA5_b61* owner;
    u8 pad_0x38[0x40 - 0x38];
    UnkMatrix44 transform;
    u8 pad_0x80[0x118 - 0x80];
    void* field_0x118;
};

void fn_71007d1654(UnkElectrifyingAreaA5_b61* self) {
    fn_71007d17d4(self);
    self->field_0x118 = fn_71007d191c(self);
    UnkCopyMatrix(&self->transform, fn_710002614c(&self->owner->transform));
    if (UnkTimelineComponent* timeline = self->owner->timeline)
        SetAll_A5_b61(timeline, &g_unk_710164ab10);
}
