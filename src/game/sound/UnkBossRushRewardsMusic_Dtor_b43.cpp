// The destructor of the "BOSSRUSHREWARDS" music trigger (src/game/sound/UnkBossRushRewardsMusic.cpp
// has its constructor and layout; this one sits in unit 1691 of tools/tu_map.py): the parts are
// destroyed last to first (each: its own vtable, its description's string holder freed and destroyed,
// its name destroyed, then the part base's vtable and its reflected value), then the part list (its
// vtable, cleared), then the music trigger base (fn_71011867d4). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2725.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::UnkReflValue;

extern char g_unk_7101aa00d8[];  // the trigger's vtable
extern char g_unk_7101a9cdc0[];  // the part list's
extern char g_unk_7101aa0190[];  // the parts'
extern char g_unk_7101aa01f0[];
extern char g_unk_7101aa0250[];
extern void* g_unk_7101a9cd68[];  // the part base's

struct UnkRewardsPartRaw_b43a3 {
    char* vtable;
    void* owner;
    s32 id;
    UnkReflValue value;  // 0x18
};

// One part: its description's name at `Name` and string holder 0x48 bytes after it (the parts' own
// words differ in number, so the description moves).
template <u32 Name>
struct UnkRewardsPartN_b43a3 : UnkRewardsPartRaw_b43a3 {
    u8 pad_0x30[Name - 0x30];
    CRntString name;
    u8 pad_name[0x48 - sizeof(CRntString)];
    CRntString holder;
};

template <u32 Name>
static inline __attribute__((always_inline)) void DestroyPart_b43a3(char* raw, char* vtable) {
    auto* p = reinterpret_cast<UnkRewardsPartN_b43a3<Name>*>(raw);
    p->vtable = vtable + 0x10;
    fn_710007ed40(&p->holder);
    fn_710007ecd4(&p->holder);
    fn_710007ecd4(&p->name);
    p->vtable = reinterpret_cast<char*>(g_unk_7101a9cd68) + 0x10;
    fn_71000a1c60(&p->value);
}

struct UnkRewardsPartListRaw_b43a3 {
    char* vtable;
    void* owner;
    u64 field_0x10;
    u64 field_0x18;
    u32 count;
};

struct UnkBossRushRewardsRaw_b43a3 {
    char* vtable;
    u8 pad_0x8[0x48];
    UnkRewardsPartListRaw_b43a3 parts;  // 0x50
};

void fn_7100b498fc(UnkBossRushRewardsRaw_b43a3* self) {
    char* raw = reinterpret_cast<char*>(self);
    self->vtable = g_unk_7101aa00d8 + 0x10;
    DestroyPart_b43a3<0xb8>(raw + 0x3d8, g_unk_7101aa0250);
    DestroyPart_b43a3<0xa8>(raw + 0x270, g_unk_7101aa01f0);
    DestroyPart_b43a3<0xb0>(raw + 0x100, g_unk_7101aa0190);
    self->parts.vtable = g_unk_7101a9cdc0 + 0x10;
    self->parts.field_0x10 = 0;
    self->parts.field_0x18 = 0;
    self->parts.count = 0;
    fn_71011867d4(self);
}
