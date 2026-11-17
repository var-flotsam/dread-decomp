// CSabotoruSpawnPointComponent's unit: a reflected setter that copies the
// link (a callback set holding a lazily resolved handle) passed as its argument into the component's
// link at +0x3b8, unless either one is resolved to a dead target. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkLinkTarget_la1c634 : base::UnkRefCounted {
    void* field_0x10;
};

// The resolved handle, assigned out of line.
struct UnkLinkHandle_la1c634 {
    base::UnkRefHandle handle;
    UnkLinkHandle_la1c634& operator=(const UnkLinkHandle_la1c634& other) {
        const base::UnkRefHandle* from = &other.handle;
        fn_710031d208(&handle, from);
        return *this;
    }
};

struct UnkLinkBase_la1c634 {
    void** vtable;
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 state;                       // 0x30: 1 not resolved yet, 2 resolved

    UnkLinkBase_la1c634() {}
    ~UnkLinkBase_la1c634() { vtable = &g_unk_7101a20998[2]; }
};

struct UnkLink_la1c634 : UnkLinkBase_la1c634 {
    UnkLinkHandle_la1c634 owner;  // 0x38

    UnkLink_la1c634(const UnkLink_la1c634& other) {
        state = 0;
        vtable = &g_unk_7101a33fa0[2];
        owner.handle.ptr = nullptr;
        if (base::UnkRefCounted* p = other.owner.handle.ptr) {
            ++p->ref_count;
            if (base::UnkRefCounted* old = owner.handle.ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            owner.handle.ptr = p;
        }
        state = other.state;
        fn_710023b29c(&callbacks, &other.callbacks);
    }
    ~UnkLink_la1c634() {
        vtable = &g_unk_7101a33fa0[2];
        if (base::UnkRefCounted* p = owner.handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkLink_la1c634& operator=(const UnkLink_la1c634& other) {
        owner = other.owner;
        state = other.state;
        fn_710023b29c(&callbacks, &other.callbacks);
        return *this;
    }
    // False when the handle was resolved to a target that is gone.
    bool IsAlive() const {
        if (state == 2) {
            auto* target = static_cast<UnkLinkTarget_la1c634*>(owner.handle.ptr);
            if (!target || !target->field_0x10)
                return false;
        }
        return true;
    }
};
static_assert(sizeof(UnkLink_la1c634) == 0x40, "layout");

struct UnkSabotoruSpawnPoint_la1c634 {
    u8 pad_0x0[0x3b8];
    UnkLink_la1c634 link;  // 0x3b8
};

struct UnkReflSelf_la1c634 {
    void* vtable;
    UnkSabotoruSpawnPoint_la1c634* component;  // 0x8
};

// The argument's storage: out of line when flagged so or larger than 8 bytes.
static inline const void* ArgValue(const UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

void fn_7100a1c634(UnkReflSelf_la1c634* self, UnkReflValue* const* args) {
    const UnkReflValue* arg = args[0];
    UnkSabotoruSpawnPoint_la1c634* component = self->component;
    UnkLink_la1c634 link(*static_cast<const UnkLink_la1c634*>(ArgValue(arg)));
    if (link.IsAlive() && component->link.IsAlive())
        component->link = link;
}
