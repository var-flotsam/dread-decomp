// CFreezeRoomComponent (name from reflection): fn_7100a23a20 (and fn_7100a23220 below) runs the base step (fn_7100a46e78),
// keeps the game manager's flag at +0x2441 at +0xc1c and makes the parameter set it picks (+0xc00
// when set, +0xbe4 otherwise) the current one (+0xbc0), then refreshes (fn_7100a232d4,
// fn_7100a238f4). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct CFreezeRoomComponent;
void fn_7100a238f4(CFreezeRoomComponent* self);
f32 fn_7100a232d4(CFreezeRoomComponent* self);
void fn_7100a46e78(CFreezeRoomComponent* self);

struct UnkFreezePair_b50a5 {
    f32 x;
    f32 y;
};

struct UnkFreezeCurrent_b50a5 {
    f32 field_0x0;
    f32 field_0x4;
    UnkFreezePair_b50a5 field_0x8;
    f32 field_0x10;
    f32 field_0x14;
    void Set(const struct UnkFreezeParams_b50a5& p);
};

struct UnkFreezeParams_b50a5 {
    f32 field_0x0;
    f32 field_0x4;
    UnkFreezePair_b50a5 field_0x8;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
};

inline void UnkFreezeCurrent_b50a5::Set(const UnkFreezeParams_b50a5& p) {
    field_0x0 = p.field_0x0;
    field_0x4 = p.field_0x4;
    field_0x8 = p.field_0x8;
    field_0x10 = p.field_0x10;
    field_0x14 = p.field_0x18;
}

struct UnkFreezeRoom_b50a5 {
    u8 unk_0x0[0xbc0];
    UnkFreezeCurrent_b50a5 current;  // 0xbc0
    u8 unk_0xbd8[0xc];
    UnkFreezeParams_b50a5 normal;    // 0xbe4
    UnkFreezeParams_b50a5 alt;       // 0xc00
    u32 use_alt;                     // 0xc1c
};

static inline __attribute__((always_inline)) void PickParams_b50a5(CFreezeRoomComponent* self) {
    UnkFreezeRoom_b50a5* room = reinterpret_cast<UnkFreezeRoom_b50a5*>(self);
    u8 flag = reinterpret_cast<const u8*>(g_unk_7101d47310)[0x2441];
    room->use_alt = flag;
    if (!flag)
        room->current.Set(room->normal);
    else
        room->current.Set(room->alt);
}

void fn_7100a23a20(CFreezeRoomComponent* self) {
    fn_7100a46e78(self);
    PickParams_b50a5(self);
    fn_7100a232d4(self);
    fn_7100a238f4(self);
}

// fn_7100a23220: the same with another base step (fn_7100a45cac), and the sound path at +0xc20
// handed to the sound manager (fn_710010808c) before the refresh.
struct UnkLiquidPool_b44a2;
void fn_7100a45cac(UnkLiquidPool_b44a2* self);
struct UnkSoundMgr;
struct UnkPathNode;
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_710010808c(UnkSoundMgr*, UnkPathNode*);

void fn_7100a23220(CFreezeRoomComponent* self) {
    fn_7100a45cac(reinterpret_cast<UnkLiquidPool_b44a2*>(self));
    PickParams_b50a5(self);
    fn_710010808c(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(reinterpret_cast<char*>(self) + 0xc20));
    fn_7100a232d4(self);
    fn_7100a238f4(self);
}
