// State handler of a component: when the owner (+0x18) has an object at +0x240 whose target (from
// fn_71010fccf8) has a record whose entry at +0x10 of the first pointer has kind 2, copies the
// value at +0xac of the second pointer and the 12 bytes at +0x40 of the owner's record at +0x1f0 into the
// object at +0x14678. Then hands a default parameter block to fn_71006f4b0c. Always returns state 7.
#include "game/UnkAIComponent.h"
#include "game_s2/UnkParams.h"

struct UnkKind {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};

struct UnkEntry {
    u8 pad_0x0[0x10];
    UnkKind* field_0x10;
};

struct UnkValue {
    u8 pad_0x0[0xac];
    u32 field_0xac;
};

struct UnkRecord {
    UnkEntry* field_0x0;
    u8 pad_0x8[8];
    UnkValue* field_0x10;
};

struct UnkSource {
    u8 pad_0x0[0x40];
    u64 field_0x40;
    u32 field_0x48;
};

struct UnkOwner {
    u8 pad_0x0[0x1f0];
    UnkSource* field_0x1f0;
    u8 pad_0x1f8[0x48];
    UnkAIComponent* field_0x240;
};

struct UnkComponent {
    u8 pad_0x0[0x18];
    UnkOwner* field_0x18;
};

struct __attribute__((packed)) UnkCopy {
    u32 field_0x0;
    u64 field_0x4;
    u32 field_0xc;
};

u32 fn_71003dd084(UnkComponent* self) {
    UnkAIComponent* object = self->field_0x18->field_0x240;
    UnkNavigation* target = fn_71010fccf8(object);
    UnkSource* source = self->field_0x18->field_0x1f0;
    if (object && target && source) {
        UnkRecord* record = static_cast<UnkRecord*>(target->field_0xb0);
        if (record->field_0x0 && record->field_0x0->field_0x10 && record->field_0x0->field_0x10->field_0x10 == 2 &&
            record->field_0x10) {
            UnkCopy* dst = reinterpret_cast<UnkCopy*>(reinterpret_cast<char*>(object) + 0x14678);
            dst->field_0x0 = record->field_0x10->field_0xac;
            u64 low = source->field_0x40;
            u32 high = source->field_0x48;
            dst->field_0x4 = low;
            dst->field_0xc = high;
        }
    }
    if (target) {
        UnkParams params;
        params.field_0x30 = 0;
        params.field_0x1c = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        fn_71006f4b0c(target, &params, 1);
    }
    return 7;
}
