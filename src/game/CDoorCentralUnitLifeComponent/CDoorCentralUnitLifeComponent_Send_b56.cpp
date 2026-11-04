// CDoorCentralUnitLifeComponent's unit (tools/tu_map.py): fn_71008ca0b8 passes the flag on to
// fn_71008ca700 in state 1 (+0xc08); otherwise, when the owner (+0x30) has its sub-object at +0x220,
// sends it the hash of "shieldclosed" (flag set) or "shieldclose" with mode 0 (as UnkSpitSend,
// src/game_s2/UnkSpitSend.h), then, when the owner has the object fn_71005012bc finds and
// fn_710024fbc8 says no for it, calls fn_710086c768 and fn_710024fa0c(1) on it. Names are
// placeholders. fn_71008ca554 (same unit) calls fn_71008d8190, passes the flag at +0xc49 to
// fn_71008ca700 and clears it; when the owner has its sub-object at +0x220, sends it the id at +0x30
// of the id table's entry at +0x1b40 (fn_7100031894; as CAbilityShinespark_Send_b55.cpp) with mode
// 0; then calls fn_71007e9ea0(1) on the owner's object fn_710030a820 finds, if any.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "types.h"

struct UnkCentralUnitDoor_b46a2;
void fn_71008ca700(UnkCentralUnitDoor_b46a2* door, bool open);

struct UnkStruct_710024fa0c;
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);
bool fn_710024fbc8(void*);

struct UnkDoorLifeOwner_b56 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkDoorLifePart_b56;
UnkDoorLifePart_b56* fn_71005012bc(UnkDoorLifeOwner_b56* owner);
void fn_710086c768(UnkDoorLifePart_b56* part);

struct UnkDoorLife_b56 {
    u8 pad_0x0[0x30];
    UnkDoorLifeOwner_b56* owner;
    u8 pad_0x38[0xc08 - 0x38];
    s32 field_0xc08;
    u8 pad_0xc0c[0xc49 - 0xc0c];
    bool field_0xc49;
};

struct UnkIdHolder_b56dl {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkIdTable_b56dl {
    UnkIdTable_b56dl();  // fn_7100031894
    ~UnkIdTable_b56dl();
    u8 pad_0x0[0x1b40];
    UnkIdHolder_b56dl* field_0x1b40;
};

inline UnkIdTable_b56dl& IdTable_b56dl() {
    static UnkIdTable_b56dl s_table;
    return s_table;
}

void fn_71008d8190(UnkDoorLife_b56* self);
struct UnkDoorLifeShield_b56;
UnkDoorLifeShield_b56* fn_710030a820(UnkDoorLifeOwner_b56* owner);
void fn_71007e9ea0(UnkDoorLifeShield_b56* shield, bool flag);

void fn_71008ca0b8(UnkDoorLife_b56* self, bool closed) {
    if (self->field_0xc08 == 1) {
        fn_71008ca700(reinterpret_cast<UnkCentralUnitDoor_b46a2*>(self), closed);
        return;
    }
    if (void* target = self->owner->field_0x220) {
        u64 hash;
        if (closed)
            hash = base::global::CName("shieldclosed").hash;
        else
            hash = base::global::CName("shieldclose").hash;
        UnkSpitSend(target, hash, 0xf4061ff25a2163d6);
    }
    if (UnkDoorLifePart_b56* part = fn_71005012bc(self->owner)) {
        if (!fn_710024fbc8(part)) {
            fn_710086c768(part);
            fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(part), true);
        }
    }
}

void fn_71008ca554(UnkDoorLife_b56* self) {
    fn_71008d8190(self);
    fn_71008ca700(reinterpret_cast<UnkCentralUnitDoor_b46a2*>(self), self->field_0xc49);
    self->field_0xc49 = false;
    if (void* target = self->owner->field_0x220)
        UnkSpitSend(target, IdTable_b56dl().field_0x1b40->id, 0xf4061ff25a2163d6);
    if (UnkDoorLifeShield_b56* shield = fn_710030a820(self->owner))
        fn_71007e9ea0(shield, true);
}
