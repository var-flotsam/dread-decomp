// Unit base::reflection::CPointerType: follows a typed pointer through its chain of pointer
// types until the target is not a pointer (or is null). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include <stdint.h>

#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;

__attribute__((visibility("hidden"))) extern const CClass g_unk_7101d5ae10;

struct UnkPointerType;
typedef UnkReflTypedPtr (*UnkPointerDeref)(void* pointer);

static inline UnkPointerDeref DerefOf(const UnkPointerType* type) {
    return *reinterpret_cast<const UnkPointerDeref*>(reinterpret_cast<uintptr_t>(type) + 0xf0);
}

static inline bool IsPointerType(const void* type) {
    const UnkReflObject* obj = reinterpret_cast<const UnkReflObject*>(type);
    const CType* cls = obj->vf_0x0();
    fn_710009bedc();
    return fn_710009d50c(cls, &g_unk_7101d5ae10);
}

static inline UnkReflTypedPtr NoPointee() {
    UnkReflTypedPtr none;
    none.type = nullptr;
    none.bits = 0;
    return none;
}

// A typed pointer that is assigned only from another object.
struct UnkWalkPtr : UnkReflTypedPtr {
    UnkWalkPtr(const UnkReflTypedPtr& o) : UnkReflTypedPtr(o) {}
    UnkWalkPtr& operator=(const UnkReflTypedPtr& o) {
        if (this != &o) {
            ptr = o.ptr;
            type = o.type;
        }
        return *this;
    }
};

static inline UnkReflTypedPtr Pointee(const UnkReflTypedPtr& p) {
    void* obj = p.ptr;
    char* t = fn_710009dcd8(p.type, obj);
    t = fn_710009dcd8(t, obj);
    const UnkPointerType* pt = t && IsPointerType(t) ? reinterpret_cast<const UnkPointerType*>(t) : nullptr;
    UnkPointerDeref deref = DerefOf(pt);
    return deref && reinterpret_cast<const UnkPointerType*>(t) == pt ? deref(obj) : NoPointee();
}

UnkReflTypedPtr fn_710009c460(const UnkReflTypedPtr& pointer) {
    UnkWalkPtr cur(pointer);
    while (cur.type && cur.ptr && IsPointerType(cur.type)) {
        void* obj = cur.ptr;
        char* t = fn_710009dcd8(cur.type, obj);
        t = fn_710009dcd8(t, obj);
        const UnkPointerType* pt = t && IsPointerType(t) ? reinterpret_cast<const UnkPointerType*>(t) : nullptr;
        UnkPointerDeref deref = DerefOf(pt);
        if (deref && reinterpret_cast<const UnkPointerType*>(t) == pt) {
            UnkReflTypedPtr next = deref(obj);
            cur = next;
        } else {
            UnkReflTypedPtr none = NoPointee();
            cur = none;
        }
    }
    return cur;
}
