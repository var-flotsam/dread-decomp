// CChozoWarriorAIComponent's unit (tools/tu_map.py): an event handler. On an event of kind 1 it runs
// fn_710049d094, then, for the event id (+0x30 of the object at +0x38 of the object at +0x10 of its +0x8)
// with a fixed hash while the record at +0x149cc is armed (+0x4c), marks it (+0x4d); for the unit's static
// id object instead, sets the record's state to 3 and runs fn_710049d190. Returns true. Names and layouts
// are placeholders.
#include "types.h"

struct UnkWarriorTurn_b58;
void fn_710049d094(UnkWarriorTurn_b58* self);

struct UnkWarriorEventId_A2_b70 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkWarriorEventInner_A2_b70 {
    u8 pad_0x0[0x38];
    UnkWarriorEventId_A2_b70* id;
};
struct UnkWarriorEventData_A2_b70 {
    u8 pad_0x0[0x10];
    UnkWarriorEventInner_A2_b70* inner;
};
struct UnkWarriorEvent_A2_b70 {
    u32 kind;
    u8 pad_0x4[4];
    UnkWarriorEventData_A2_b70* data;
};

struct UnkWarriorRecord_A2_b70 {
    u32 state;
    u8 pad_0x4[0x48];
    bool armed;  // 0x4c
    bool hit;    // 0x4d
};

struct UnkWarriorAI_A2_b70 {
    u8 pad_0x0[0x149cc];
    UnkWarriorRecord_A2_b70 record;  // 0x149cc
};

extern UnkWarriorEventId_A2_b70* g_unk_71726d1288 __attribute__((visibility("hidden")));
void fn_710049d190(UnkWarriorAI_A2_b70* self);

bool fn_710049cff0(UnkWarriorAI_A2_b70* self, UnkWarriorEvent_A2_b70* event) {
    if (event->kind == 1) {
        UnkWarriorRecord_A2_b70* record = &self->record;
        fn_710049d094(reinterpret_cast<UnkWarriorTurn_b58*>(self));
        UnkWarriorEventId_A2_b70* id = event->data->inner->id;
        if (id->hash == 0xd8aa1ffac09fb2fcull && record->armed) {
            record->hit = true;
        } else if (id == g_unk_71726d1288) {
            record->state = 3;
            fn_710049d190(self);
        }
    }
    return true;
}
