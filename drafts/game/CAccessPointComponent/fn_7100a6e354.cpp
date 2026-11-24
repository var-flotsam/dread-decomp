// CAccessPointComponent unit: refreshes the access point after a load. Reads the unlocked flag and the
// current id from the scenario's blackboard section, falls back to the default entry when the id is not in
// the component's table, then freezes each link's door lock state from the blackboard. Names and layouts
// are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU367.h"
#include "game/UnkTU437.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern UnkPropertyEx<> g_unk_71727592f8;
extern u8 g_unk_717274ec10[];  // A UnkProperty.

struct UnkRef_la6e354 {
    UnkWeakHandle* ptr;
    ~UnkRef_la6e354() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkAccessLink_la6e354 {
    u8 pad_0x0[8];
    u8 source[0x28];  // 0x8
    u32 state;        // 0x30
    u8 pad_0x34[4];
    UnkRef_la6e354 handle;  // 0x38
};

struct UnkAccessValue_la6e354 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkAccessNode_la6e354 {
    CStrId key;
    u8 pad_0x8[0x38];
    UnkAccessNode_la6e354* next;  // 0x40
};

struct UnkAccessPoint_la6e354 {
    u8 pad_0x0[0x4d0];
    UnkAccessValue_la6e354* values;  // 0x4d0
    u64* keys;                       // 0x4d8
    u32 key_count;                   // 0x4e0
    u8 pad_0x4e4[0x590 - 0x4e4];
    bool unlocked;  // 0x590
    u8 pad_0x591[7];
    CStrId id;  // 0x598
    u8 pad_0x5a0[0x10];
    UnkAccessNode_la6e354** buckets;  // 0x5b0
    u32 table_count;                  // 0x5b8
    u32 bucket_count;                 // 0x5bc
    u8 pad_0x5c0[0x20];
    UnkAccessNode_la6e354* fallback;  // 0x5e0
    u8 pad_0x5e8[0x898 - 0x5e8];
    u8 links[8];  // 0x898
    u32 link_count;  // 0x8a0
    u8 pad_0x8a4[0x938 - 0x8a4];
    UnkAccessValue_la6e354* values2;  // 0x938
    u64* keys2;                       // 0x940
    u32 key_count2;                   // 0x948
};

UnkAccessLink_la6e354* fn_71007b0e04(void* array, u32 index);
bool fn_71008d1e10(UnkDoorLock* door);
void fn_71008d8890(UnkDoorLock* door, bool flag);

inline const CStrId& SectionId_la6e354() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(s_table.pad_0x0 + 0x1150);
}

static inline u64 IndexOf(const u64* keys, u32 count, const void* property) {
    u64 index = 0;
    if (count) {
        u64 key = (u64)reinterpret_cast<const UnkProperty*>(property)->name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    return index;
}

void fn_7100a6e354(UnkAccessPoint_la6e354* self) {
    fn_7100a9f300(reinterpret_cast<UnkTeleporterSelf_b0233*>(self));
    UnkGameManager* manager = g_unk_7101d47310;
    auto* sub = manager->field_0x608;
    CBlackboard* blackboard = &manager->field_0x968;
    if (sub) {
        {
            CStrId section_id = fn_71011a2fbc(sub);
            u64 index = IndexOf(self->keys, self->key_count, g_unk_7172753148);
            UnkAccessValue_la6e354* values = self->values;
            if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                fn_710006d11c(s, reinterpret_cast<const CStrId*>(&values[index].name), &self->unlocked);
        }
        const CStrId& section = SectionId_la6e354();
        UnkQualifiedId& qid = ItemQualifiedId(g_unk_71727592f8);
        if (qid.id.data == &g_unk_7101d47108->empty)
            fn_7100ae9a10(&qid, ItemSource(g_unk_71727592f8), CStrId::Empty());
        if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
            fn_710051d464(s, &qid, &self->id);
    }

    bool known = false;
    if (self->id.data != &g_unk_7101d47108->empty && self->table_count) {
        u32 h = (self->id.data->hash[0] ^ self->id.data->hash[1]) % self->bucket_count;
        UnkAccessNode_la6e354** slot = &self->buckets[h];
        if (slot) {
            for (UnkAccessNode_la6e354* n = *slot; n; n = n->next) {
                if (n->key.data == self->id.data) {
                    known = true;
                    break;
                }
            }
        }
    }
    if (!known && self->fallback)
        self->id = self->fallback->key;

    for (u32 i = 0; i < self->link_count; ++i) {
        UnkAccessLink_la6e354* link = fn_71007b0e04(self->links, i);
        if (link->state == 1) {
            {
                base::UnkRefPtrMember fresh = fn_7100353be8(link->source);
                fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(&link->handle),
                              reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            link->state = 2;
        }
        UnkWeakHandle* h = link->handle.ptr;
        UnkDoorLock* door = fn_71002af860(reinterpret_cast<UnkWeakTarget*>(h ? h->target : nullptr));
        if (!door || !fn_71008d1e10(door))
            continue;
        bool value = false;
        if (sub) {
            CStrId section_id = fn_71011a2fbc(sub);
            u64 index = IndexOf(self->keys2, self->key_count2, g_unk_717274ec10);
            UnkAccessValue_la6e354* values = self->values2;
            if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                fn_710006d11c(s, reinterpret_cast<const CStrId*>(&values[index].name), &value);
        }
        fn_71008d8890(door, !value);
    }
}
