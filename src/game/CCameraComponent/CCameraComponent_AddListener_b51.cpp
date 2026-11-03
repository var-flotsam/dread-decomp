// From unit 2691 of tools/tu_map.py (CCameraComponent): adds an object to the list at +0xa8 unless it
// is there already, then notifies it (vf_0x40; vf_0x48 first when it was already listed). Names and
// layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkListener_71010e4804 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
};

struct UnkTag_71010e4804;

struct UnkOwner_71010e4804 {
    u8 pad_0x0[0xa8];
    base::UnkSList<UnkTag_71010e4804> listeners;  // +0xa8
};

bool fn_71010e4804(UnkOwner_71010e4804* self, UnkListener_71010e4804* obj) {
    if (!obj)
        return false;
    using List = base::UnkSList<UnkTag_71010e4804>;
    for (List::Node* n = self->listeners.head; n; n = n->next) {
        if (n->value == reinterpret_cast<u64>(obj)) {
            obj->vf_0x48();
            obj->vf_0x40();
            return true;
        }
    }
    self->listeners.PushBack(reinterpret_cast<u64>(obj));
    obj->vf_0x40();
    return true;
}
