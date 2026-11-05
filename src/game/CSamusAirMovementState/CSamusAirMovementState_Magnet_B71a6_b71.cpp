// CSamusAirMovementState unit (tools/tu_map.py, unit 1455): unless the state is of kind 13, when the
// owner's inventory holds the magnet glove (ITEM_MAGNET_GLOVE) and the player's ability from
// fn_7100df554c is not in state 3 to 6, it builds a request (fn_71009a609c) carrying *value, asks the
// movement's state of the kind fn_71009a60fc names with the owner's three words at +0x44/+0x54/+0x64
// (fn_71009a64cc) and, when that holds, passes the request on (fn_710098ae04). Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkInventory;
f32 fn_7101142928(UnkInventory*, const UnkQualifiedId&);
struct UnkAbility;
u32 fn_7100309568(UnkAbility*);
u32 fn_71009a60fc();
UnkAbility* fn_7100df554c(UnkGameManager* manager);

extern void* g_unk_7101a874f0[];

struct UnkAirRequest_B71a6 {
    void* vtable;
    u8 pad_0x8[0x80 - 0x8];
    base::UnkRefPtrMember ref_0x80;
    base::UnkRefPtrMember ref_0x88;
    u8 pad_0x90[0xa0 - 0x90];
    u64 value;

    UnkAirRequest_B71a6();
    ~UnkAirRequest_B71a6() {
        vtable = reinterpret_cast<char*>(g_unk_7101a874f0) + 0x10;
    }
};

struct UnkAirMoveState_B71a6 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;
};

#pragma pack(push, 4)
struct UnkAirMove_B71a6 {
    u8 pad_0x0[0x2d18];
    u32 state_count;
    UnkAirMoveState_B71a6* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkAirOwner_B71a6 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0x54 - 0x48];
    u32 field_0x54;
    u8 pad_0x58[0x64 - 0x58];
    u32 field_0x64;
    u8 pad_0x68[0x270 - 0x68];
    UnkInventory* inventory;
};

struct UnkAirWords_B71a6 {
    u32 a, b, c;
};

struct UnkAirTarget_B71a6;

struct UnkAirState_B71a6 {
    void* vtable;
    UnkAirTarget_B71a6* target;
    s32 kind;
    u8 pad_0x14[0x30 - 0x14];
    UnkAirOwner_B71a6* owner;
    UnkAirMove_B71a6* movement;
};

void fn_71009a609c(UnkAirRequest_B71a6* self);
inline UnkAirRequest_B71a6::UnkAirRequest_B71a6() { fn_71009a609c(this); }
bool fn_71009a64cc(UnkAirMoveState_B71a6* state, const UnkAirWords_B71a6* words, s32 a, UnkAirRequest_B71a6* req,
                   s32 b);
void fn_710098ae04(UnkAirTarget_B71a6* target, UnkAirRequest_B71a6* req);

static inline UnkAirMoveState_B71a6* FindState(UnkAirMove_B71a6* self, s32 kind) {
    u64 size = (u64)self->state_count * sizeof(UnkAirMoveState_B71a6*);
    asm("" : "+r"(size));
    if (size == 0)
        return nullptr;
    UnkAirMoveState_B71a6** it = self->states;
    asm("" : "+r"(it));
    for (; size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

static inline const UnkQualifiedId& ItemId_B71a6(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

void fn_710098b590(UnkAirState_B71a6* self, void*, const u64* value) {
    if (self->kind == 13)
        return;
    UnkInventory* inventory = self->owner->inventory;
    if (fn_7101142928(inventory, ItemId_B71a6(g_unk_717275a9d8)) == 0.0f)
        return;
    if (UnkAbility* ability = fn_7100df554c(g_unk_7101d47310)) {
        if (fn_7100309568(ability) - 3 < 4)
            return;
    }
    UnkAirRequest_B71a6 req;
    req.value = *value;
    UnkAirMove_B71a6* m = self->movement;
    u32 k = fn_71009a60fc();
    UnkAirMoveState_B71a6* state = k <= 14 ? FindState(m, k) : nullptr;
    UnkAirOwner_B71a6* owner = self->owner;
    u32 a = owner->field_0x44, b = owner->field_0x54, c = owner->field_0x64;
    UnkAirWords_B71a6 words;
    words.a = a;
    words.b = b;
    words.c = c;
    if (fn_71009a64cc(state, &words, 1, &req, 1))
        fn_710098ae04(self->target, &req);
}

