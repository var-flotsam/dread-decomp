// Unit 365 (navmesh::CNavMeshGenerator): connects two areas both ways: each records the other with
// the key (id and kind) it is reached through, adding the key to an existing link or a new link.
// Names and layouts are placeholders.
#include "base/UnkIdKindKey.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkNavArea_l29c4c0;

// Another area and the keys this one reaches it through.
struct UnkNavLink_l29c4c0 {
    UnkNavArea_l29c4c0* other;
    base::global::CRntVector<UnkIdKindKey> keys{15};

    explicit UnkNavLink_l29c4c0(UnkNavArea_l29c4c0* other) : other(other) {}
    UnkNavLink_l29c4c0(UnkNavArea_l29c4c0* other, u16 id, u8 kind) : other(other) {
        keys.EmplaceBack(UnkIdKindKey(id, kind));
    }
    ~UnkNavLink_l29c4c0() { keys.Free(); }

    // Adds the key unless the link already has it.
    void AddKey(u16 id, u8 kind);
};
static_assert(sizeof(UnkNavLink_l29c4c0) == 0x30, "layout");

struct UnkNavArea_l29c4c0 {
    u8 pad_0x0[0x30];
    base::global::CRntVector<UnkNavLink_l29c4c0> links;  // 0x30
};

// Appends a copy of `link` (its keys copied).
void fn_710029cba8(base::global::CRntVector<UnkNavLink_l29c4c0>* self, const UnkNavLink_l29c4c0* link);

// The link to the same area as `probe`, or the end of the links.
static inline UnkNavLink_l29c4c0* FindLink(const base::global::CRntVector<UnkNavLink_l29c4c0>& links,
                                           const UnkNavLink_l29c4c0& probe) {
    for (u32 i = 0; i < links.count; ++i)
        if (links.data[i].other == probe.other)
            return &links.data[i];
    return &links.data[links.count];
}

static inline UnkIdKindKey* FindKey(const base::global::CRntVector<UnkIdKindKey>& keys, const UnkIdKindKey& key) {
    for (u32 i = 0; i < keys.count; ++i)
        if (keys.data[i].id == key.id && keys.data[i].kind == key.kind)
            return &keys.data[i];
    return &keys.data[keys.count];
}

inline void UnkNavLink_l29c4c0::AddKey(u16 id, u8 kind) {
    if (FindKey(keys, UnkIdKindKey(id, kind)) == &keys.data[keys.count])
        keys.EmplaceBack(UnkIdKindKey(id, kind));
}
void fn_710029c4c0(void* self, UnkNavArea_l29c4c0* a, u16 a_id, u8 a_kind, UnkNavArea_l29c4c0* b, u16 b_id,
                   u8 b_kind) {
    if (a == b)
        return;

    // a reaches b through (a_id, a_kind).
    UnkNavLink_l29c4c0* to_b = FindLink(a->links, UnkNavLink_l29c4c0(b));
    if (to_b != &a->links.data[a->links.count]) {
        to_b->AddKey(a_id, a_kind);
    } else {
        UnkNavLink_l29c4c0 added(b, a_id, a_kind);
        fn_710029cba8(&a->links, &added);
    }

    // b reaches a through (b_id, b_kind).
    UnkNavLink_l29c4c0* to_a = FindLink(b->links, UnkNavLink_l29c4c0(a));
    if (to_a != &b->links.data[b->links.count]) {
        to_a->AddKey(b_id, b_kind);
    } else {
        UnkNavLink_l29c4c0 added(a, b_id, b_kind);
        fn_710029cba8(&b->links, &added);
    }
}