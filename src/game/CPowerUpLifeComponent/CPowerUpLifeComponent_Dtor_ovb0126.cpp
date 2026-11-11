// CPowerUpLifeComponent's unit (tools/tu_map.py): the destructor (fn_71008e4c54). The vtable; the object the
// handle at +0x3c8 refers to gets its slot-0xe0 call; then the members are destroyed: the string ids at
// +0x3d8 and +0x3d0, the handle at +0x3c8, the callback list at +0x388 (its own vtable, its counted owner
// at +0x3c0, the base vtable and the array of type-erased callables), and finally the life component's
// destructor (fn_71008e05e8). Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/global/CStrId.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CItemLifeComponent.h"
#include "game/CPowerUpLifeComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkPowerUpLifeTarget_ovb0126 {
    void Notify() {
        using Fn = void (*)(UnkPowerUpLifeTarget_ovb0126*);
        (*reinterpret_cast<Fn**>(this))[0xe0 / 8](this);
    }
};

struct UnkPowerUpLifeRefObj_ovb0126 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();  // 0x10
    union {
        s32 ref_count;
    };
    UnkPowerUpLifeTarget_ovb0126* target;  // 0x10
};

// A counted pointer released (not cleared) by its destructor.
struct UnkPowerUpLifeRef_ovb0126 {
    UnkPowerUpLifeRefObj_ovb0126* ptr;
    __attribute__((always_inline)) ~UnkPowerUpLifeRef_ovb0126() {
        if (UnkPowerUpLifeRefObj_ovb0126* p = ptr) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

// A string id whose release decrements and tests the count in two steps.
struct UnkPowerUpLifeId_ovb0126 {
    UnkStringData* data;
    __attribute__((always_inline)) ~UnkPowerUpLifeId_ovb0126() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkPowerUpLifeListBase_ovb0126 {
    char* vtable;
    UnkOwnedErasedVector callables;
    __attribute__((always_inline)) ~UnkPowerUpLifeListBase_ovb0126() {
        vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
    }
};

struct UnkPowerUpLifeList_ovb0126 : UnkPowerUpLifeListBase_ovb0126 {
    u32 field_0x30;
    UnkPowerUpLifeRef_ovb0126 owner;
    __attribute__((always_inline)) ~UnkPowerUpLifeList_ovb0126() {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
    }
};

struct UnkPowerUpLife_ovb0126 {
    char* vtable;
    u8 pad_0x8[0x380];
    UnkPowerUpLifeList_ovb0126 list;  // 0x388
    UnkPowerUpLifeRef_ovb0126 handle;  // 0x3c8
    UnkPowerUpLifeId_ovb0126 name;     // 0x3d0
    UnkPowerUpLifeId_ovb0126 id;       // 0x3d8

    __attribute__((always_inline)) void Destroy() {
        vtable = g_unk_7101a7f4c0 + 0x10;
        if (UnkPowerUpLifeRefObj_ovb0126* h = handle.ptr) {
            if (UnkPowerUpLifeTarget_ovb0126* target = h->target)
                target->Notify();
        }
        id.~UnkPowerUpLifeId_ovb0126();
        name.~UnkPowerUpLifeId_ovb0126();
        handle.~UnkPowerUpLifeRef_ovb0126();
        list.~UnkPowerUpLifeList_ovb0126();
        fn_71008e05e8(this);
    }
};
static_assert(__builtin_offsetof(UnkPowerUpLife_ovb0126, id) == 0x3d8, "layout");

void fn_71008e4c54(UnkPowerUpLife_ovb0126* self) {
    self->Destroy();
}
