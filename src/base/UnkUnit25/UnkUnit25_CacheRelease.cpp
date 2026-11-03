// From unit 25 of tools/tu_map.py (0x7100018a44-0x710001f9ac): the release of a cached object
// (base/UnkCache.h frees one whose load failed): its vf_0x28 (an unload), then its string id at +0x8
// and file-path id at +0x10 are released and, with `destroy`, it is deleted through its vtable's
// slot 2. Names and layouts are placeholders.
#include "base/UnkCache.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkCachedObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // deleting destructor
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();  // unload
    base::UnkStrIdMember name;                      // +0x8
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;  // +0x10
};

void fn_7100019fc0(void* obj, bool destroy) {
    UnkCachedObject* self = static_cast<UnkCachedObject*>(obj);
    self->vf_0x28();
    self->name.~UnkStrIdMemberOf();
    self->path.~UnkStrIdMemberOf();
    if (destroy)
        self->vf_0x10();
}
