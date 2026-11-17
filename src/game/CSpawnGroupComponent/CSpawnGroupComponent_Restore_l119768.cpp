// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): fn_7101119768 restores the spawn group's saved state from
// the current scenario's section of a blackboard: a flag (property g_unk_7172a065e0) that refills the group's list
// (fn_710111b230) or resets it, and a count (property g_unk_7172a06608) that caps +0x194. Names and layouts are
// placeholders.
#include "base/UnkVtableList.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU604.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

struct UnkList_71011199ccNode {
    u64 value;
    UnkList_71011199ccNode* next;
};

struct UnkList_71011199cc : base::UnkVtableList<UnkList_71011199ccNode> {
    using Node = UnkList_71011199ccNode;

    UnkList_71011199cc() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkList_71011199cc() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        Node* n = head;
        if (!n)
            return;
        base::UnkAllocator* a = allocator;
        while (n) {
            Node* next = n->next;
            a->vf_0x48(n);
            n = next;
        }
    }
};

struct UnkComponent_71011199cc;
void fn_710111b230(UnkComponent_71011199cc* self, UnkList_71011199cc* out);

struct UnkSpawnGroupValue_l119768 {
    CStrId id;
    u64 field_0x8;
};

struct UnkSpawnGroup_l119768 {
    u8 pad_0x0[0x70];
    u32 field_0x70;
    u8 pad_0x74[0xc4];
    bool field_0x138;
    u8 pad_0x139[3];
    bool field_0x13c;
    u8 pad_0x13d;
    bool field_0x13e;
    bool field_0x13f;
    u8 pad_0x140[0x4c];
    s32 field_0x18c;
    u8 pad_0x190[4];
    s32 field_0x194;
    u8 pad_0x198[4];
    s32 field_0x19c;
    u8 pad_0x1a0[0x250];
    UnkSpawnGroupValue_l119768* values;  // 0x3f0
    u64* keys;                           // 0x3f8
    u32 count;                           // 0x400

    // The position of the property's name in the key list, or the count when it is not there.
    u64 IndexOf(const UnkProperty& property) const {
        u64 index = 0;
        u32 n = count;
        if (n) {
            const u64* k = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        return index;
    }
};
static_assert(__builtin_offsetof(UnkSpawnGroup_l119768, field_0x13c) == 0x13c, "");
static_assert(__builtin_offsetof(UnkSpawnGroup_l119768, field_0x19c) == 0x19c, "");
static_assert(__builtin_offsetof(UnkSpawnGroup_l119768, values) == 0x3f0, "");

void fn_710111b528(UnkSpawnGroup_l119768* self);

// Fills a list through fn_710111b230 and frees it again (fn_71011199cc, inlined here).
static inline void Refill(UnkSpawnGroup_l119768* self) {
    UnkList_71011199cc list;
    fn_710111b230(reinterpret_cast<UnkComponent_71011199cc*>(self), &list);
}

// Reads the integer under `name` in the blackboard section `section` names.
static inline bool GetSectionInt(CBlackboard* blackboard, const CStrId& section, const void* name, s32* out) {
    CBlackboard::CSection* s = fn_7101047270(blackboard, section);
    return s && fn_7100395a30(s, name, out);
}

void fn_7101119768(UnkSpawnGroup_l119768* self, CBlackboard* blackboard) {
    if (!self->field_0x13c)
        return;
    bool flag = self->field_0x13e;
    {
        CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        u64 index = self->IndexOf(g_unk_7172a065e0);
        UnkSpawnGroupValue_l119768* values = self->values;
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
            fn_710006d11c(section, &values[index].id, &flag);
    }
    if (flag) {
        Refill(self);
    } else if (self->field_0x70 == 0) {
        self->field_0x138 = false;
        self->field_0x13f = false;
        fn_710111b528(self);
    }
    self->field_0x19c = 0;
    bool found;
    {
        CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        u64 index = self->IndexOf(g_unk_7172a06608);
        UnkSpawnGroupValue_l119768* values = self->values;
        found = GetSectionInt(blackboard, section_id, &values[index], &self->field_0x19c);
    }
    if (found) {
        s32 value = self->field_0x19c;
        s32 limit = self->field_0x18c;
        self->field_0x194 = limit > value ? value : limit;
    }
}
