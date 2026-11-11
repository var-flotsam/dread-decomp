// CLockRoomLogicAction's unit (tools/tu_map.py unit 2531): the action's step. After the base step
// (fn_7100a55ac8) it resolves its lazy reference (+0x10: vtable, source +0x18 through fn_7100353be8,
// state +0x40, handle +0x48) and looks up the target's fn_7100da3630 component. If found: when the
// flag at +0xd8 is set, it copies the component's array of lazy references (+0x898, 0x40-byte
// elements) and, for each one that resolves to an actor with a door lock (fn_71002af860), calls
// fn_71008d498c(lock, 1, 1, 1); then it copies its string id (+0x50) to the component's +0x580 and
// hands it the array at +0x58 (fn_7100a6e01c). Finally it passes the target's +0x30 to
// fn_710013ffbc with that array. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkTU112.h"
#include "game/UnkTU1376.h"
#include "types.h"

struct UnkTutoExitAction_b47;

using UnkRef = base::UnkRefPtrMember;
UnkRef fn_7100353be8(void* source);

struct UnkWeakTarget;
struct UnkDoorLock;
UnkDoorLock* fn_71002af860(UnkWeakTarget*);
struct UnkDoorLife_b44a2;
struct UnkEntry_710013c3c8;

struct UnkRefl_fn_7100da3630;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100da3630>(void*);

static inline void* Actor(base::UnkRefCounted* target) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10);
}

// A reference resolved from its source on first use.
struct UnkLockRoomLazyRef_b54 {
    virtual ~UnkLockRoomLazyRef_b54();
    u8 source[0x28];  // 0x8
    u32 state;        // 0x30
    u8 pad_0x34[4];
    base::UnkRefHandle handle;  // 0x38

    void* Get() {
        if (state == 1) {
            {
                UnkRef h = fn_7100353be8(source);
                fn_710031d208(&handle, reinterpret_cast<base::UnkRefHandle*>(&h));
            }
            state = 2;
        }
        base::UnkRefCounted* target = handle.ptr;
        return target ? Actor(target) : nullptr;
    }
};
static_assert(sizeof(UnkLockRoomLazyRef_b54) == 0x40, "layout");

using UnkLockRoomRefArray_b54 = base::UnkVector<UnkLockRoomLazyRef_b54>;
// The array's copy constructor (src/game/CEntity/CEntity_VectorCopy_b43.cpp).
struct UnkEntry_b43a3;
void fn_7100d88548(base::UnkVector<UnkEntry_b43a3>* self, const base::UnkVector<UnkEntry_b43a3>* other);

struct UnkLockRoomArgs_b54;
void fn_7100a6e01c(void* component, UnkLockRoomArgs_b54* args);

struct UnkLockRoomAction_b54 {
    void* vtable;
    u8 pad_0x8[0x8];
    UnkLockRoomLazyRef_b54 ref;     // 0x10
    base::UnkStrIdMember id;        // 0x50
    u8 args[0x80];                  // 0x58
    bool flag;  // 0xd8
};
static_assert(__builtin_offsetof(UnkLockRoomAction_b54, flag) == 0xd8, "layout");

void fn_7100da3374(UnkLockRoomAction_b54* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    if (char* comp = reinterpret_cast<char*>(
            base::reflection::refl_FindTyped<UnkRefl_fn_7100da3630>(self->ref.Get()))) {
        if (self->flag) {
            alignas(8) char buf[sizeof(UnkLockRoomRefArray_b54)];
            auto& refs = *reinterpret_cast<UnkLockRoomRefArray_b54*>(buf);
            fn_7100d88548(reinterpret_cast<base::UnkVector<UnkEntry_b43a3>*>(&refs),
                          reinterpret_cast<base::UnkVector<UnkEntry_b43a3>*>(comp + 0x898));
            for (UnkLockRoomLazyRef_b54 *it = refs.data, *end = refs.data + refs.count; it != end; ++it) {
                if (void* actor = it->Get())
                    if (UnkDoorLock* lock = fn_71002af860(reinterpret_cast<UnkWeakTarget*>(actor)))
                        fn_71008d498c(reinterpret_cast<UnkDoorLife_b44a2*>(lock), 1, 1, 1);
            }
            refs.Free();
        }
        *reinterpret_cast<base::UnkStrIdMember*>(comp + 0x580) = self->id;
        fn_7100a6e01c(comp, reinterpret_cast<UnkLockRoomArgs_b54*>(self->args));
    }
    struct UnkActor {
        u8 pad[0x30];
        u8 field_0x30;
    };
    UnkActor* actor = static_cast<UnkActor*>(self->ref.Get());
    u8* key = &actor->field_0x30;  // before the entry: the original sets x1 before x0
    UnkEntry_710013c3c8* entry = reinterpret_cast<UnkEntry_710013c3c8*>(self->args);
    fn_710013ffbc(entry, key);
}
