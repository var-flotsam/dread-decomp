// Looks up an object in a chained hash map at +0xd0 (keyed by the object pointer, hashed by
// two words of the object) and returns its value, inserting a newly built one if missing.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_71003327a4_Key {
    u8 pad_0x0[0x10];
    u8 field_0x10[0x20];
    u32 field_0x30;
    u32 field_0x34;
    u8 pad_0x38[0xc];
    union {  // no type-based alias info: the key pointer is reloaded after the increment
        s32 field_0x44;
    };
};

struct UnkStruct_71003327a4;

// The map's value: an id and one more word, built from the owner and the key.
struct UnkStruct_71003327a4_Value {
    CStrId field_0x0;
    u64 field_0x8;
    UnkStruct_71003327a4_Value(UnkStruct_71003327a4* owner, void* src);  // fn_71003328ac
};

struct UnkStruct_71003327a4_Node {
    UnkStruct_71003327a4_Key* key;
    UnkStruct_71003327a4_Value value;
    UnkStruct_71003327a4_Node* next;
};

// The insert result (0x20 bytes; only the node is used here).
struct UnkStruct_71003327a4_Iter {
    UnkStruct_71003327a4_Node* node;
    void* field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    UnkStruct_71003327a4_Iter(const UnkStruct_71003327a4_Iter& o);
};

using UnkMap_71003327a4 = base::UnkVector<UnkStruct_71003327a4_Node*>;

UnkStruct_71003327a4_Node** fn_7100332978(UnkMap_71003327a4* map, u32 index);
UnkStruct_71003327a4_Iter fn_7100332b54(UnkMap_71003327a4* map, UnkStruct_71003327a4_Key** key,
                                        UnkStruct_71003327a4_Value&& value);

struct UnkStruct_71003327a4 {
    u8 pad_0x0[0xd0];
    UnkMap_71003327a4 field_0xd0;
};

UnkStruct_71003327a4_Value* fn_71003327a4(UnkStruct_71003327a4* self, UnkStruct_71003327a4_Key** key) {
    UnkMap_71003327a4* map = &self->field_0xd0;
    if (map->count) {
        u32 index = ((*key)->field_0x30 ^ (*key)->field_0x34) % map->capacity;
        if (index < map->count) {
            for (UnkStruct_71003327a4_Node* n = *fn_7100332978(map, index); n; n = n->next) {
                if (n->key == *key)
                    return &n->value;
            }
        }
    }
    ++(*key)->field_0x44;
    return &fn_7100332b54(map, key, UnkStruct_71003327a4_Value(self, (*key)->field_0x10)).node->value;
}
