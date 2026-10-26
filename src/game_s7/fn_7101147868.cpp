// Appends a "SamusInventory" record whose value is a string of 26 digits: for each of 26 item
// definitions, 1 when the inventory's map holds an amount above zero under the item's qualified
// id, else 0.
#include <cstdio>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

using base::global::CStrId;

// An item's qualified id (game/UnkGameBlackboard.h), built on first use (as in fn_7100fc4b34).
static inline const CStrId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    // A pointer compare (an integer compare inverts the branch and the block layout).
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid.id;
}

struct UnkKeyData {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkAmountNode {
    UnkKeyData* key;
    f32* value;
    u8 unk_0x10[0x10];
    UnkAmountNode* next;
};

struct UnkInventory_7101147868 {
    u8 unk_0x0[0x50];
    UnkAmountNode** buckets;
    u32 count;
    u32 modulus;
};

static inline bool HasItem(UnkInventory_7101147868* self, UnkPropertyEx<>& item) {
    const CStrId& id = Qid(item);
    if (self->count == 0)
        return false;
    UnkKeyData* key = reinterpret_cast<UnkKeyData*>(id.data);
    u32 index = (key->field_0x30 ^ key->field_0x34) % self->modulus;
    UnkAmountNode** bucket = self->buckets + index;
    if (!bucket)
        return false;
    for (UnkAmountNode* node = *bucket; node; node = node->next) {
        if (node->key == key)
            return node->value && *node->value > 0.0f;
    }
    return false;
}

struct UnkRecord_7101147868 {
    CStrId field_0x0;
    base::UnkStrIdMember field_0x8;
    u64 field_0x10;
    u32 field_0x18;

    // Takes the ids by value: the caller destroys the argument copies after the append, and
    // keeps their final stores (as references to temporaries, those stores are dropped).
    UnkRecord_7101147868(CStrId name, CStrId value) asm("fn_7100ec4240");
};
void fn_71003af880(base::UnkVector<UnkRecord_7101147868>* self, UnkRecord_7101147868&& value);


void fn_7101147868(UnkInventory_7101147868* self, base::UnkVector<UnkRecord_7101147868>* records) {
    int has_0 = HasItem(self, g_unk_717275a518);
    int has_1 = HasItem(self, g_unk_717275a9d8);
    int has_2 = HasItem(self, g_unk_717275aa58);
    int has_3 = HasItem(self, g_unk_717275a5d8);
    int has_4 = HasItem(self, g_unk_717275a8d8);
    int has_5 = HasItem(self, g_unk_717275a498);
    int has_6 = HasItem(self, g_unk_717275a558);
    int has_7 = HasItem(self, g_unk_717275a4d8);
    int has_8 = HasItem(self, g_unk_717275aad8);
    int has_9 = HasItem(self, g_unk_717275aa98);
    int has_10 = HasItem(self, g_unk_717275a598);
    int has_11 = HasItem(self, g_unk_717275a758);
    int has_12 = HasItem(self, g_unk_717275a6d8);
    int has_13 = HasItem(self, g_unk_717275a898);
    int has_14 = HasItem(self, g_unk_717275a798);
    int has_15 = HasItem(self, g_unk_717275ab58);
    int has_16 = HasItem(self, g_unk_717275ab98);
    int has_17 = HasItem(self, g_unk_717275a958);
    int has_18 = HasItem(self, g_unk_717275a458);
    int has_19 = HasItem(self, g_unk_717275a918);
    int has_20 = HasItem(self, g_unk_717275a7d8);
    int has_21 = HasItem(self, g_unk_717275a658);
    int has_22 = HasItem(self, g_unk_717275ab18);
    int has_23 = HasItem(self, g_unk_717275a818);
    int has_24 = HasItem(self, g_unk_717275a418);
    int has_25 = HasItem(self, g_unk_717275a718);
    char str[32];
    sprintf(str, "%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i", has_0, has_1, has_2, has_3,
            has_4, has_5, has_6, has_7, has_8, has_9, has_10, has_11, has_12, has_13, has_14, has_15,
            has_16, has_17, has_18, has_19, has_20, has_21, has_22, has_23, has_24, has_25);
    fn_71003af880(records, UnkRecord_7101147868(CStrId("SamusInventory", true), CStrId(str, true)));
}
