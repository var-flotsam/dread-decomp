// From unit 1734 of tools/tu_map.py (0x7100b91030-0x7100b95bb8, no reflected class; the boss rush
// screens of src/game_s7/fn_7100b921e0.cpp and fn_7100b94320.cpp are in it): the constructor of the
// "BOSSRUSHREWARDS" music trigger (its base, src/game/sound/UnkMusicTrigger.cpp's, names a music
// entry) with a list of three parts that it registers by id. Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

extern void* g_unk_7101a1b5a0[];
using UnkString20 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

// The music trigger base as this file sees it (src/game/sound/UnkMusicTrigger.cpp defines it and its
// constructor, fn_7101186780): its vtable, the music entry's name and flags.
struct UnkMusicTrigger;
void fn_7101186780(UnkMusicTrigger* self);
struct UnkMusicTriggerBase {
    char* vtable;
    CRntString name;  // 0x8
    u32 flags;        // 0x38
    bool field_0x3c;
    u64 field_0x40;
    u64 field_0x48;
};

struct UnkRewardsPart;
typedef UnkRewardsPart* UnkRewardsPartPtr __attribute__((aligned(4)));

// The list of parts: up to 16, each registered once per id.
struct UnkRewardsPartList {
    char* vtable;
    void* owner;  // the trigger
    u64 field_0x10;
    u64 field_0x18;
    u32 count;
    UnkRewardsPartPtr parts[16];  // 0x24
    u32 field_0xa4;
    u16 field_0xa8;

    void Add(UnkRewardsPart* part);
};
static_assert(sizeof(UnkRewardsPartList) == 0xb0, "UnkRewardsPartList layout");

// A part: its list and id, then (after each class's own words) a description with an empty name,
// three scales of 1 and a string with inline storage.
struct UnkRewardsPart {
    char* vtable;
    UnkRewardsPartList* owner;
    s32 id;  // 0x10
    u64 field_0x18;
    u64 field_0x20;
    s32 field_0x28;
    s32 field_0x2c;
    u64 field_0x30;
};

// A string (base::global::CRntString's layout) built from the empty literal by fn_710007e704.
struct UnkRewardsName {
    alignas(8) u8 unk_0x0[0x30];

    UnkRewardsName() { fn_710007e704(reinterpret_cast<base::UnkNamedEntry*>(this), ""); }
};

struct UnkRewardsPartDescBody {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
    u32 field_0x10 = 0;
    UnkRewardsName name;  // 0x18
    u32 field_0x48 = 0;
    alignas(8) f32 scale[3] = {1.0f, 1.0f, 1.0f};  // 0x50
    UnkString20 holder;                            // 0x60
    u32 field_0xc8 = 0;
    alignas(8) u32 field_0xd0 = 0;
    u16 field_0xd4 = 1;
};

struct UnkRewardsPartDesc {
    bool field_0x0;
    s32 field_0x4;
    u32 field_0x8;
    UnkRewardsPartDescBody body;  // 0x10
};
static_assert(sizeof(UnkRewardsPartDesc) == 0xe8, "UnkRewardsPartDesc layout");

template <u32 N>
struct UnkRewardsPartN : UnkRewardsPart {
    u64 field_0x38[N];
    UnkRewardsPartDesc desc;
};

struct UnkRewardsPart3 : UnkRewardsPartN<11> {
    u16 field_0x178;
    bool field_0x17a;
};

struct UnkBossRushRewards : UnkMusicTriggerBase {
    UnkRewardsPartList parts;  // 0x50
    UnkRewardsPartN<10> part1;  // 0x100
    UnkRewardsPartN<9> part2;   // 0x270
    UnkRewardsPart3 part3;      // 0x3d8
};
static_assert(__builtin_offsetof(UnkBossRushRewards, part2) == 0x270, "UnkBossRushRewards layout");
static_assert(__builtin_offsetof(UnkBossRushRewards, part3) == 0x3d8, "UnkBossRushRewards layout");
static_assert(sizeof(UnkBossRushRewards) == 0x558, "UnkBossRushRewards layout");

extern char g_unk_7101aa00d8[];  // the trigger's vtable
extern char g_unk_7101a9cdc0[];  // the part list's
extern char g_unk_7101aa0190[];  // the parts'
extern char g_unk_7101aa01f0[];
extern char g_unk_7101aa0250[];

inline void UnkRewardsPartList::Add(UnkRewardsPart* part) {
    UnkRewardsPartPtr* end = parts + count;
    for (UnkRewardsPartPtr* p = parts; p != end; ++p)
        if ((*p)->id == part->id)
            return;
    part->owner = this;
    *end = part;
    ++count;
}

static inline void InitPart(UnkRewardsPart* part, char* vtable, s32 id) {
    part->field_0x20 = 0;
    part->owner = nullptr;
    part->field_0x30 = 0;
    part->field_0x18 = 0;
    part->field_0x28 = 2;
    part->field_0x2c = 1;
    part->id = id;
    part->vtable = vtable + 0x10;
}

static inline void InitDesc(UnkRewardsPartDesc* d) {
    d->field_0x0 = false;
    d->field_0x4 = -1;
    d->field_0x8 = 0;
    new (&d->body) UnkRewardsPartDescBody;
}

template <u32 N>
static inline void InitPartN(UnkRewardsPartN<N>* part, char* vtable, s32 id) {
    InitPart(part, vtable, id);
    for (u32 i = 0; i < N; ++i)
        part->field_0x38[i] = 0;
    InitDesc(&part->desc);
}

// NON_MATCHING(minor): the constant stores are scheduled in another order (and the last word of
// each part's description is not addressed through the description), and the part registration
// loops keep other registers; the same stores between the same calls, in the same call order.
void fn_7100b91030(UnkBossRushRewards* self) {
    fn_7101186780(reinterpret_cast<UnkMusicTrigger*>(self));
    self->vtable = g_unk_7101aa00d8 + 0x10;
    self->parts.vtable = g_unk_7101a9cdc0 + 0x10;
    self->parts.owner = nullptr;
    self->parts.field_0x10 = 0;
    self->parts.field_0x18 = 0;
    self->parts.count = 0;
    self->parts.field_0xa4 = 4;
    self->parts.field_0xa8 = 0;
    InitPartN(&self->part1, g_unk_7101aa0190, 1);
    InitPartN(&self->part2, g_unk_7101aa01f0, 2);
    InitPartN(&self->part3, g_unk_7101aa0250, 3);
    self->part3.field_0x178 = 0;
    self->part3.field_0x17a = false;
    fn_710007e77c(&self->name, "BOSSRUSHREWARDS");
    self->parts.owner = self;
    self->parts.Add(&self->part1);
    self->parts.Add(&self->part2);
    self->parts.Add(&self->part3);
}
