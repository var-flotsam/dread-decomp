// CActorComponent's unit: looks up a reflected type by the id of the owner's name, and when it is
// a class derived from CActorComponent with an in-place constructor, creates one (allocation flags 5),
// constructs it and registers it with the owner. Names and layouts are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern char g_unk_717269fef8[] __attribute__((visibility("hidden")));

struct UnkNameData {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkNamed {
    u8 pad_0x0[0x10];
    UnkNameData* name;
};

struct UnkNamedOwner {
    UnkNamed* named;
};

struct UnkCompTypeView {
    u8 pad_0x0[0x18];
    u64 size;
    u8 pad_0x20[0x8];
    void (*construct)(void* object);
};

struct UnkNewComponent {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(UnkNamedOwner* owner);
};

UnkNewComponent* fn_710024f71c(UnkNamedOwner* self) {
    UnkStringData* name = reinterpret_cast<UnkStringData*>(self->named->name);
    if (name != &g_unk_7101d47108->empty)
        ++name->ref_count;
    UnkNewComponent* result = nullptr;
    UnkVersionRegistry* registry = g_unk_7101d4c8d0;
    u64 id = reinterpret_cast<UnkNameData*>(name)->hash;
    auto* entry = reinterpret_cast<base::reflection::UnkReflObject*>(fn_71000a5118(registry, &id));
    if (entry && fn_710009d50c(entry->vf_0x0(), fn_7100096100())) {
        auto* type = reinterpret_cast<const base::reflection::CType*>(entry);
        fn_710024e3c8();
        auto* view = reinterpret_cast<const UnkCompTypeView*>(entry);
        if (fn_710009d50c(type, reinterpret_cast<const base::reflection::CType*>(g_unk_717269fef8)) &&
            view->construct) {
            void* p = fn_7100083990()->vf_0x40(view->size, 8, 5);
            if (p) {
                view->construct(p);
                result = static_cast<UnkNewComponent*>(p);
                result->vf_0x38(self);
            }
        }
    }
    UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        if (name != &pool->empty) {
            if (--name->ref_count == 0)
                fn_71000803a4(pool, name);
        }
    }
    return result;
}
