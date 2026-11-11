// Best draft of fn_71001f9e8c (97.9%: the original tests the masked result with cbz w8 where this tests bit 0 of w0).
#include "types.h"

struct UnkMapOwnerA_A1b71;

struct UnkCondition_b0235 {
    u64 key;       // 0x0
    u64 field_0x8;
    u32 kind;      // 0x10
    u32 field_0x14;
};

struct UnkConditionList_b0235 {
    u8 pad_0x0[0x10];
    UnkCondition_b0235* data;  // 0x10
    u32 count;                 // 0x18
};

struct UnkConditionOwner_b0235;
u32 fn_71001efbec(UnkConditionOwner_b0235* owner, const u64* key);
void fn_71001ef008(UnkConditionOwner_b0235* owner, u64 key, u8* result);

bool fn_71001f9e8c(void* value, UnkMapOwnerA_A1b71* owner_) {
    auto* list = static_cast<UnkConditionList_b0235*>(value);
    auto* owner = reinterpret_cast<UnkConditionOwner_b0235*>(owner_);
    if (list->count == 0)
        return true;
    u8 ok;
    for (UnkCondition_b0235* c = list->data;;) {
        ok = 0;
        if (c->kind == 1) {
            fn_71001ef008(owner, c->key, &ok);
            if (!ok)
                return false;
        } else {
            u64 key = c->key;
            ok = fn_71001efbec(owner, &key) & 1;
            if (!ok)
                return false;
        }
        ++c;
        if (c == list->data + list->count)
            return true;
    }
}
