// msapi::material::SAlpha's unit (tools/tu_map.py unit 229): finds the entry with an index in the
// group of a kind, clears its resource and path, and sets its value. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using FilePathMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;

struct UnkSlotValue_l1a8d38 {
    u64 field_0x0;
    u32 pad_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
};

struct UnkSlotEntry_l1a8d38 {
    u8 pad_0x0[0x20];
    u32 index;  // 0x20
    FilePathMember path;  // 0x28
    u8 pad_0x30[0x30];
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u64 field_0x78;
    UnkCountedResource* resource;  // 0x80

    void Clear() {
        if (resource) {
            if (--resource->ref_count == 0)
                fn_710001737c(resource);
            resource = nullptr;
        }
        path = reinterpret_cast<const FilePathMember&>(g_unk_7101d470e8);
    }
};
static_assert(sizeof(UnkSlotEntry_l1a8d38) == 0x88, "layout");

struct UnkSlotGroup_l1a8d38 {
    u8 pad_0x0[0x8];
    u32 kind;  // 0x8
    u8 pad_0xc[0x2c];
    UnkSlotEntry_l1a8d38* entries;  // 0x38
    u32 count;  // 0x40
    u8 pad_0x44[0x1c];

};
static_assert(sizeof(UnkSlotGroup_l1a8d38) == 0x60, "layout");

// The group's entry with this index, if the group is of this kind.
static UnkSlotEntry_l1a8d38* FindEntry(UnkSlotGroup_l1a8d38* group, u32 kind, u32 index) {
    if (group->kind == kind) {
        for (UnkSlotEntry_l1a8d38* it = group->entries; it != group->entries + group->count; ++it) {
            if (it->index == index)
                return it;
        }
    }
    return nullptr;
}

struct UnkSlotOwner_l1a8d38 {
    u8 pad_0x0[0x358];
    UnkSlotGroup_l1a8d38* groups;  // 0x358
    u32 group_count;  // 0x360
};

void fn_71001a8d38(void* target, u32 index, void* value, u32 flag) {
    UnkSlotOwner_l1a8d38* self = static_cast<UnkSlotOwner_l1a8d38*>(target);
    const UnkSlotValue_l1a8d38* v = static_cast<const UnkSlotValue_l1a8d38*>(value);
    UnkSlotEntry_l1a8d38* entry = nullptr;
    for (UnkSlotGroup_l1a8d38* g = self->groups; g != self->groups + self->group_count && !entry; ++g) {
        entry = FindEntry(g, flag, index);
        if (entry)
            entry->Clear();
    }
    if (!entry)
        return;
    entry->field_0x78 = v->field_0x0;
    entry->path = reinterpret_cast<const FilePathMember&>(g_unk_7101d470e8);
    entry->field_0x60 = v->field_0xc;
    entry->field_0x64 = v->field_0x10;
    entry->field_0x68 = v->field_0x14;
    entry->field_0x6c = v->field_0x18;
    entry->field_0x70 = v->field_0x1c;
}
