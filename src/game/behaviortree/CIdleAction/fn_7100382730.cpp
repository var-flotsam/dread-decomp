// Starts a query on the owner's object at +0x240: fills a query record (a vtable, a direction
// vector and a flag, mostly zero) and hands it to fn_71006f4b0c. With the flag at +0x2c set the
// direction is the sign of the x offset to the position object; otherwise it is copied from a
// nested record of the object's component when that has one. Always returns 2.
#include "game/UnkAIComponent.h"
#include "game/UnkActor.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkRecord {
    u8 pad_0x0[0x78];
    u64 field_0x78;
    u32 field_0x80;
    u8 field_0x84;
};

struct UnkRecordList {
    u8 pad_0x0[0x10];
    UnkRecord* field_0x10;
};

struct UnkRecordPtr {
    UnkRecordList* field_0x0;
};

struct UnkSelf {
    u8 pad_0x0[0x18];
    UnkActor* field_0x18;
    u8 pad_0x20[0xc];
    u8 field_0x2c;
};

struct UnkQuery {
    void** vtable;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 pad_0x3e[2];

    UnkQuery() {
        field_0x1c = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        vtable = g_unk_7101a25b10 + 2;
        __builtin_memset(&field_0x8, 0, 13);
        field_0x30 = 0;
        field_0x39 = 0;
        field_0x3b = 0;
        field_0x3d = 0;
        field_0x34 = 5;
    }
};

// NON_MATCHING(minor): the two null tests after the query object lookup (the object, the position
// object) are made in the other order (scheduling only)
u32 fn_7100382730(UnkSelf* self) {
    if (self->field_0x18) {
        UnkActor* target = fn_7100d7c990(self->field_0x18);
        UnkActor* owner = self->field_0x18;
        if (owner && owner->field_0x240) {
            UnkNavigation* object = fn_71010fccf8(static_cast<UnkAIComponent*>(owner->field_0x240));
            if (target && object) {
                owner = self->field_0x18;
                if (owner) {
                    UnkQuery query;
                    if (self->field_0x2c) {
                        f32 dx = target->field_0x44 - owner->field_0x44;
                        query.field_0x8 = (f32)((dx > 0.0f) - (dx < 0.0f));
                        query.field_0xc = 0.0f;
                        query.field_0x10 = 0.0f;
                        query.field_0x14 = 1;
                    } else if (UnkRecordList* list = static_cast<UnkRecordPtr*>(object->field_0xb0)->field_0x0) {
                        if (UnkRecord* record = list->field_0x10) {
                            if (record->field_0x84) {
                                u64 xy = record->field_0x78;
                                u32 z = record->field_0x80;
                                *reinterpret_cast<u64*>(&query.field_0x8) = xy;
                                *reinterpret_cast<u32*>(&query.field_0x10) = z;
                                query.field_0x14 = 1;
                            }
                        }
                    }
                    fn_71006f4b0c(object, &query, 1);
                }
            }
        }
    }
    return 2;
}
