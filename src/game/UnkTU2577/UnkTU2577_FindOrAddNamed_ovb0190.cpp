// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eeebb0: finds the object of the array at +0x830 (owned, 0x38 bytes, id at +8) with the given id, else builds one (fn_7100fb4b4c), names it and appends it. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkNamedObject_b190 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    CStrId name;  // 0x8
    u8 pad_0x10[0x28];
};

struct UnkNamedObjectPtr_b190 {
    union {
        UnkNamedObject_b190* ptr;
    };
    explicit UnkNamedObjectPtr_b190(UnkNamedObject_b190* p) : ptr(p) {}
    UnkNamedObjectPtr_b190(UnkNamedObjectPtr_b190&& o) {
        UnkNamedObject_b190* moved = o.ptr;
        o.ptr = nullptr;
        ptr = moved;
    }
    ~UnkNamedObjectPtr_b190() {
        UnkNamedObject_b190* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x8();
    }
};

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x830];
    base::UnkVector<UnkNamedObjectPtr_b190> objects;  // 0x830
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, objects) == 0x830, "layout");

UnkNamedObjectPtr_b190& fn_7100eee97c(base::UnkVector<UnkNamedObjectPtr_b190>* v, u32 index);
void fn_7100fb4b4c(UnkNamedObject_b190* self);

UnkNamedObject_b190* fn_7100eeebb0(UnkSubAreaMgr_b190* self, const CStrId* id) {
    for (u32 i = 0; i < self->objects.count; ++i) {
        UnkNamedObject_b190* o = fn_7100eee97c(&self->objects, i).ptr;
        if (o->name.data == id->data)
            return o;
    }
    UnkNamedObject_b190* o = static_cast<UnkNamedObject_b190*>(fn_71000839a0(0x38, 0x53));
    fn_7100fb4b4c(o);
    if (!o)
        return o;
    o->name = *id;
    self->objects.EmplaceBack(UnkNamedObjectPtr_b190(o));
    return o;
}
