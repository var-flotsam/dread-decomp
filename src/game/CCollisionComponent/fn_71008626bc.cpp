// A collider read from the collision file (unit 1319, CCollisionComponent): its names, created
// through the container's vf_0x168, then its shape and, when the file goes on, a flag. Names and
// layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// The next string, in place (as src/game/logic/collision/UnkShapeFactory.cpp declares it).
s64 fn_710007cee4(void* stream, const char** out);

// The shape kind a name stands for.
u32 fn_7100d4bc00(const CRntString* name);

// A collider's shape, which reads itself from the file (vf_0x20).
struct UnkColliderShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(base::global::CRntFile* file);
};

struct UnkReadCollider {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(bool flag);
    u8 pad_0x8[0x150];
    UnkColliderShape* shape;  // 0x158
};

// The collision container (fn_710086697c's owner): its slot 0x168 creates a collider from its ids,
// the shape kind and options (three unset references among them).
struct UnkColliderContainer {
    void** vtable;

    UnkReadCollider* Create(const CStrId* name, const CStrId* group, const CStrId* type_name, const CStrId* extra,
                            u32 kind) {
        using Fn = UnkReadCollider* (*)(UnkColliderContainer*, const CStrId*, const CStrId*, const CStrId*,
                                        const CStrId*, u32, u32, u32, u64, s32, u64, s32, u64, s32, bool, bool,
                                        bool);
        return reinterpret_cast<Fn>(vtable[0x168 / 8])(this, name, group, type_name, extra, kind, 1, 0, 0, -1, 0, -1,
                                                       0, -1, false, false, false);
    }
};

// Reads a collider's five strings (its name, its group, a type name, its shape kind's name and one
// more), creates it, reads its shape and, when the file has more, its flag. Always true.
bool fn_71008626bc(UnkColliderContainer* self, base::global::CRntFile* stream) {
    const char* name;
    const char* group;
    const char* type_name;
    const char* kind_name;
    const char* extra;
    fn_710007cee4(stream, &name);
    fn_710007cee4(stream, &group);
    fn_710007cee4(stream, &type_name);
    fn_710007cee4(stream, &kind_name);
    fn_710007cee4(stream, &extra);
    CStrId type_id(type_name, true);
    u32 kind = 0;
    if (*kind_name) {
        CRntString text(kind_name);
        kind = fn_7100d4bc00(&text);
    }
    UnkReadCollider* collider;
    {
        CStrId name_id(name, true);
        CStrId group_id(group, true);
        CStrId extra_id(extra, true);
        collider = self->Create(&name_id, &group_id, &type_id, &extra_id, kind);
    }
    collider->shape->vf_0x20(stream);
    if (stream->position < stream->size) {
        bool flag = false;
        fn_710007c714(stream, &flag, 1, 1);
        collider->vf_0x30(flag);
    }
    return true;
}
