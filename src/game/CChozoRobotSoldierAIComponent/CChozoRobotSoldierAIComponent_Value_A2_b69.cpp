// CChozoRobotSoldierAIComponent's unit (the name from reflection): reads the value `index` (fn_71008c2de8) of one
// of three tables of the part at +0x149a0, chosen by fn_71011057fc and the flag at +0x27790 (+0xb48 or +0xad8 when
// fn_71011057fc answers, else +0xa68 when the flag is set), into *out; false when there is none.
// Names and layouts are placeholders.
#include "game/CDamageSourceFactor.h"
#include "types.h"

struct UnkRobotSoldierTable_A2_b69;

struct UnkRobotSoldierPart_A2_b69 {
    u8 pad_0x0[0xa68];
    u8 table_a68[0xad8 - 0xa68];  // +0xa68
    u8 table_ad8[0xb48 - 0xad8];  // +0xad8
    u8 table_b48[0x10];           // +0xb48
};

struct UnkRobotSoldierAI_A2_b69 {
    u8 pad_0x0[0x149a0];
    UnkRobotSoldierPart_A2_b69* part;  // +0x149a0
    u8 pad_0x149a8[0x27790 - 0x149a8];
    bool flag;  // +0x27790
};

bool fn_71011057fc(UnkRobotSoldierAI_A2_b69* self);

bool fn_7100471cb4(UnkRobotSoldierAI_A2_b69* self, u32 index, f32* out) {
    UnkRobotSoldierTable_A2_b69* table;
    if (fn_71011057fc(self)) {
        table = reinterpret_cast<UnkRobotSoldierTable_A2_b69*>(!self->flag ? self->part->table_ad8
                                                                             : self->part->table_b48);
    } else {
        if (!self->flag)
            return false;
        table = reinterpret_cast<UnkRobotSoldierTable_A2_b69*>(self->part->table_a68);
    }
    *out = fn_71008c2de8(table, index);
    return true;
}
