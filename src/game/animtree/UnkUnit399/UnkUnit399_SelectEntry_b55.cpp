// Unit 399 (animtree, near CAnimatedNode): looks an entry up by key in the node's flat map
// (values +0x78, keys +0x80, count +0x88), hands it to fn_71002dfa18, refreshes the node
// (fn_71002dfaf0), then passes the entry's +0x28 object to the owner's (+0x70) listener at +0x8.
// Names are placeholders.
#include "types.h"

struct UnkLayerNode;
void fn_71002dfaf0(UnkLayerNode* node, bool a, f32 dt, bool b);

struct UnkEntry399 {
    u8 pad_0x0[0x28];
    void* field_0x28;  // 0x28
};

struct UnkListener399 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(void* object);
};

struct UnkOwner399 {
    u8 pad_0x0[0x8];
    u8 listener[0x8];  // 0x8, a UnkListener399

    UnkListener399* Listener() { return reinterpret_cast<UnkListener399*>(listener); }
};

struct UnkAnimNode399 {
    u8 pad_0x0[0x70];
    UnkOwner399* owner;     // 0x70
    UnkEntry399** values;   // 0x78
    u64* keys;              // 0x80
    u32 count;              // 0x88

    UnkEntry399** Find(const u64& key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key) {
                return &values[i];
            }
        }
        return values + count;
    }

    UnkEntry399*& operator[](const u64& key) {
        u64 i = 0;
        for (; i != count; ++i) {
            if (keys[i] == key) {
                break;
            }
        }
        return values[i];
    }
};

void fn_71002dfa18(UnkAnimNode399* self, UnkEntry399* entry);

void fn_71002d51d8(UnkAnimNode399* self, const u64& key) {
    UnkEntry399** it = self->Find(key);
    fn_71002dfa18(self, it == self->values + self->count ? nullptr : *it);
    fn_71002dfaf0(reinterpret_cast<UnkLayerNode*>(self), false, 1.0f, false);
    UnkListener399* listener = self->owner->Listener();
    void* object = (*self)[key]->field_0x28;
    if (object != nullptr) {
        listener->vf_0x40(object);
    }
}
