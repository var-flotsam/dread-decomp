// Looks up an entity's record in a hash map at 0xd64210 (keyed by the object at entity+0x30,
// hashed from two words at 0x30; buckets read through fn_7100eb3814) and passes a box around
// the record's position (100 units to either side, its height up) to fn_7100e796c4, first
// initialising the record's part at 0xc once.
#include "types.h"

struct UnkKeyObject {
    u8 unk_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

struct UnkEntityRef {
    u8 unk_0x0[0x30];
    UnkKeyObject* key;
};

struct UnkEntityArg {
    u8 unk_0x0[0x30];
    UnkEntityRef* entity;
};

struct UnkVec2 {
    f32 x;
    f32 y;
};

struct UnkRecord {
    UnkKeyObject* key;
    s32 field_0x8;
    u8 field_0xc[0x8];
    UnkVec2 field_0x14;
    u8 unk_0x1c[0x4];
    f32 field_0x20;
    u8 unk_0x24[0x4];
    f32 field_0x28;
    u8 unk_0x2c[0x15f8 - 0x2c];
    UnkRecord* next;
};

struct UnkRecordBuckets {
    UnkRecord** data;
    u32 count;
    u32 capacity;
};

struct UnkBox {
    UnkVec2 min;
    UnkVec2 max;
};

struct UnkRecordOwner {
    u8 unk_0x0[0x118];
    u8 field_0x118[0xd64210 - 0x118];
    UnkRecordBuckets records;
};

UnkRecord** fn_7100eb3814(UnkRecordBuckets* buckets, u32 index);
void fn_7100eb7680(void* part);
void fn_7100e796c4(void* target, const UnkBox* box, const UnkBox* bounds);

// NON_MATCHING(minor): the original loads the record's y as an integer (copied to the box as
// is, moved to a float register for the sum) and keeps x alone in a callee-saved register; ours
// loads both as floats.
bool fn_7100e9bf60(UnkRecordOwner* self, UnkEntityArg* arg) {
    if (!self->records.count)
        return false;
    UnkEntityRef* entity = arg->entity;
    u32 hash = entity->key->hash_a ^ entity->key->hash_b;
    u32 i = hash % self->records.capacity;
    if (i >= self->records.count)
        return false;
    UnkRecord* record = *fn_7100eb3814(&self->records, i);
    for (; record; record = record->next) {
        if (record->key == entity->key)
            break;
    }
    if (!record)
        return false;
    f32 x = record->field_0x14.x;
    f32 y = record->field_0x14.y;
    UnkBox box;
    box.min.y = record->field_0x14.y;
    if (!record->field_0x8) {
        fn_7100eb7680(record->field_0xc);
        record->field_0x8 = 1;
    }
    f32 height = record->field_0x28 - record->field_0x20;
    box.min.x = x + -100.0f;
    box.max.x = x + 100.0f;
    box.max.y = height + y;
    fn_7100e796c4(self->field_0x118, &box, &box);
    return false;
}
