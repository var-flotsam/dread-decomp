// Two notifiers of engine::scene::CScene's unit: an item whose state flag changed (the one at
// 0xf8 for fn_71001eef58, at 0xf6 for fn_71001eef74) reports it through its owner, which
// forwards the report to the delegate at its +8 as a tail call of one virtual for "on" and the
// next for "off"; the virtual receives the delegate and the item, not the flag. Also the pass of a
// flag to a scene group's items (fn_71001ef280).
#include "engine/scene/UnkToggle.h"

void fn_71001eef58(UnkToggleListener* owner, UnkToggle* item, bool on) {
    UnkToggleDelegate* d = owner->delegate_0x8;
    if (on)
        d->vtable_0x0->vf_0x40(d, item);
    else
        d->vtable_0x0->vf_0x48(d, item);
}

void fn_71001eef74(UnkToggleListener* owner, UnkToggle* item, bool on) {
    UnkToggleDelegate* d = owner->delegate_0x8;
    if (on)
        d->vtable_0x0->vf_0x50(d, item);
    else
        d->vtable_0x0->vf_0x58(d, item);
}

// ---- fn_71001ef280: passing a flag to a scene group's items ----

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkScene;

// An item of a group (src/game/UnkSceneGroupItem.cpp passes the flag on).
struct UnkSceneGroupItem {
    u64 field_0x0;
    u8 pad_0x8[0x8];
    u32 field_0x10;
};
void fn_71001f9e34(const UnkSceneGroupItem* self, UnkScene* scene, bool flag);

struct UnkSceneGroup {
    u8 pad_0x0[0x10];
    UnkSceneGroupItem* items;
    u32 item_count;
};

// The scene's groups by id: a chained hash map, nodes {id, group, ..., next at +0x20}.
struct UnkSceneGroupNode {
    u64 id;
    UnkSceneGroup* group;
    u8 pad_0x10[0x10];
    UnkSceneGroupNode* next;
};
struct UnkSceneGroups {
    u8 pad_0x0[0x118];
    UnkSceneGroupNode** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
};

// Passes `flag` to every item of the group `*id` names; false when there is none. The bucket is the
// id's halves xor-ed modulo the capacity (checked against the count, as RntVector's operator[]).
bool fn_71001ef280(UnkScene* scene, bool flag, const u64* id) {
    UnkSceneGroups* self = reinterpret_cast<UnkSceneGroups*>(scene);
    if (self->bucket_count == 0)
        return false;
    u64 key = *id;
    u32 i = ((u32)(key >> 32) ^ (u32)key) % self->bucket_capacity;
    if (self->bucket_count <= i)
        __assert_fail("i < m_iSize", "..\Base/global/RntVector.h", 0x15b, "operator[]");
    UnkSceneGroupNode** bucket = &self->buckets[i];
    if (!bucket)
        return false;
    for (UnkSceneGroupNode* node = *bucket; node; node = node->next) {
        if (node->id == key) {
            UnkSceneGroup* group = node->group;
            for (UnkSceneGroupItem *it = group->items, *end = it + group->item_count; it != end; ++it)
                fn_71001f9e34(it, scene, flag);
            return true;
        }
    }
    return false;
}
