// Unit 2661 (CCutscene::CTunableCutscene): the name of the first node with a payload under an object's
// "DC_Root" node: among the root's children (a sorted copy), the first that has children of its own and a
// payload, or a payload node under it; the empty id when there is none. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU212.h"
#include "types.h"

using base::global::CStrId;

// A node of an object's named tree (0x98 bytes): its payload, name and child nodes.
struct UnkNamedNode {
    u8 pad_0x0[0x60];
    void* payload;
    base::global::CStrId name;
    base::global::CRntVector<UnkNamedNode*> children;
};
static_assert(sizeof(UnkNamedNode) == 0x98, "UnkNamedNode placeholder layout");

// The nodes of a tree by the key (CRC-64) of their names.
struct UnkNamedNodeMap_l065a10 {
    u8 pad_0x0[8];
    UnkNamedNode* values;
    u64* keys;
    u32 count;

    UnkNamedNode* End() { return &values[count]; }
    UnkNamedNode* Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    UnkNamedNode* Get(const CStrId& name) {
        UnkNamedNode* node = Find(name.data->Key());
        if (node == End())
            return nullptr;
        return node;
    }
};

struct UnkNamedTreeOwner3_l065a10 {
    u8 pad_0x0[0x80];
    UnkNamedNodeMap_l065a10* nodes;
};
struct UnkNamedTreeOwner2_l065a10 {
    u8 pad_0x0[0x78];
    UnkNamedTreeOwner3_l065a10* field_0x78;
};
struct UnkNamedTreeOwner1_l065a10 {
    u8 pad_0x0[0x138];
    UnkNamedTreeOwner2_l065a10* field_0x138;
};
struct UnkNamedTreeOwner_l065a10 {
    u8 pad_0x0[0x50];
    UnkNamedTreeOwner1_l065a10* field_0x50;
};

// A copy of a node array: the same capacity from the same allocator.
struct UnkNodeArrayCopy {
    UnkNamedNode** data;
    u32 count;
    base::UnkAllocator* allocator;

    explicit UnkNodeArrayCopy(const base::global::CRntVector<UnkNamedNode*>& other) {
        allocator = other.allocator;
        data = other.capacity ? static_cast<UnkNamedNode**>(
                                    allocator->vf_0x40((u64)other.capacity * sizeof(UnkNamedNode*), 8, other.flags))
                              : nullptr;
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(UnkNamedNode*));
        count = other.count;
    }
    UnkNamedNode** begin() { return data; }
    UnkNamedNode** end() { return data + count; }
    ~UnkNodeArrayCopy() {
        if (data)
            allocator->vf_0x48(data);
    }
};

// Sorts nodes by fn_710019536c's value, largest first (libc++'s std::sort, src/game_s7/fn_7101072a80.cpp).
void fn_7101072a80(UnkNamedNode** first, UnkNamedNode** last);
// The first node at or under `node` with a payload (or null).

// The "DC_Root" id of the reflection id table.
static inline const CStrId& DcRoot_l065a10() {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x1a8];
}

CStrId fn_7101065a10(UnkNamedTreeOwner_l065a10* self) {
    if (UnkNamedTreeOwner1_l065a10* owner = self->field_0x50) {
        if (UnkNamedNodeMap_l065a10* nodes = owner->field_0x138->field_0x78->nodes) {
            if (UnkNamedNode* root = nodes->Get(DcRoot_l065a10())) {
                UnkNodeArrayCopy children(root->children);
                fn_7101072a80(children.begin(), children.end());
                if (children.count) {
                    UnkNamedNode** p = children.begin();
                    do {
                        UnkNamedNode* child = *p;
                        if (child->children.count) {
                            UnkNamedNode* node = child->payload ? child : fn_7100195310(child);
                            if (node)
                                return node->name;
                        }
                    } while (++p != children.end());
                }
            }
        }
    }
    return base::global::CStrId::Empty();
}
