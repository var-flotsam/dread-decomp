// GUI manager unit (tools/tu_map.py, unit 2744, the hint is the string "gui/scripts/guimanager.lua"):
// a reflected function of the object at 8 that looks the argument's id up in the list at +0x40 and,
// when the entry holds an object, detaches it (the virtual at 0x48 of its owner at +0x20 when it has
// a handle), frees it and clears the entry. Returns an empty value. Names are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

namespace {

struct UnkOwner_b41a3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(void* handle);
};

struct UnkHandleEntry_b41a3 {
    void* handle;
    u32 field_0x8;
    u8 pad_0xc[0x14];
    UnkOwner_b41a3* owner;
};

struct UnkNode_b41a3 {
    u64 key;
    UnkHandleEntry_b41a3* value;
    UnkNode_b41a3* next;
};

}  // namespace

UnkReflValue fn_71011bb728(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkReflValue* a = args[0];
    const u64* id;
    if (!a->type)
        id = nullptr;
    else if (a->field_0x14 == 1 || *reinterpret_cast<const u64*>(a->type + 0x18) > 8)
        id = static_cast<const u64*>(a->field_0x0);
    else
        id = reinterpret_cast<const u64*>(a);
    for (UnkNode_b41a3* n = *reinterpret_cast<UnkNode_b41a3**>(target + 0x40); n; n = n->next) {
        if (n->key == *id) {
            if (UnkHandleEntry_b41a3* e = n->value) {
                if (e->handle) {
                    e->field_0x8 = 0;
                    e->owner->vf_0x48(e->handle);
                }
                fn_7100083c00(e);
                n->value = nullptr;
            }
            break;
        }
    }
    return base::reflection::refl_EmptyValue();
}
