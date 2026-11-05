// CSPBTutoLogicAction unit (tools/tu_map.py, the name from reflection): the action's step. After the base
// step (fn_7100a55ac0) it resolves its first actor reference (+0x20); when that names an actor, the
// handle slot at +0x198 is moved to the actor's +0x68 list of its fn_71005012bc component; otherwise
// the second reference (+0x60) names a central unit whose linked entity (+0x6d8, its +0x10) gives the
// value at +0x1b0 and the component instead. Then both references are copied into the embedded
// CSPRTutoLogicAction-like object at +0xa0 (its references at +0x10 and +0x50), which runs its own step
// (fn_7100a53778). Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflStatics.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100a55ac0(void* self, void* arg);

struct UnkTutoHandle;
void fn_7100565ef0(UnkTutoHandle* handle, UnkTutoHandle** slot);
void fn_7100565d38(UnkTutoHandle* handle, UnkTutoHandle** slot);

struct UnkRefl_fn_71005012bc;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_71005012bc>(void*);

struct UnkCentralUnitProto;
UnkCentralUnitProto* fn_71007d74b0(void* actor);

void fn_7100a53778(void* self, void* arg);

// A lazily resolved actor reference: the source property, a state (1 until the handle takes its
// value, 2 after) and the handle.
struct UnkSPBTutoRef_b53 {
    void* vtable;
    UnkErasedVector source;  // 0x8
    u32 state;               // 0x30
    u8 pad_0x34[4];
    base::UnkRefHandle handle;  // 0x38

    __attribute__((always_inline)) base::UnkRefCounted* Resolve() {
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(&source);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        return handle.ptr;
    }
    __attribute__((always_inline)) void CopyFrom(UnkSPBTutoRef_b53& other) {
        const base::UnkRefHandle* from = &other.handle;
        fn_7100234f78(&handle, from);
        state = other.state;
        fn_710023b29c(&source, &other.source);
    }
};
static_assert(sizeof(UnkSPBTutoRef_b53) == 0x40, "UnkSPBTutoRef_b53 layout");

static inline void* Actor(base::UnkRefCounted* target) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10);
}

struct UnkSPBTutoInner_b53 {
    u8 pad_0x0[0x10];
    UnkSPBTutoRef_b53 first;   // 0x10
    UnkSPBTutoRef_b53 second;  // 0x50

    // Takes copies of both references, then runs its step.
    __attribute__((always_inline)) void Run(UnkSPBTutoRef_b53& a, UnkSPBTutoRef_b53& b, void* arg) {
        first.CopyFrom(a);
        second.CopyFrom(b);
        fn_7100a53778(this, arg);
    }
};

// Keeps the unit's test after the link's.
static inline __attribute__((always_inline)) char* Pin(char* p) {
    asm("" : "+r"(p));
    return p;
}

struct UnkSPBTutoAction_b53 {
    void* vtable;
    u8 pad_0x8[0x18];
    UnkSPBTutoRef_b53 first;   // 0x20
    UnkSPBTutoRef_b53 second;  // 0x60
    UnkSPBTutoInner_b53 inner;  // 0xa0, the embedded action
    u8 pad_0x130[0x198 - 0x130];
    UnkTutoHandle* slot;  // 0x198
    u8 pad_0x1a0[0x10];
    void* field_0x1b0;

    // Moves the handle slot to the component's +0x68 list.
    __attribute__((always_inline)) void Attach(char* comp) {
        if (slot) {
            fn_7100565ef0(slot, &slot);
            slot = nullptr;
        }
        slot = reinterpret_cast<UnkTutoHandle*>(comp + 0x68);
        fn_7100565d38(slot, &slot);
    }
};

void fn_7100a5264c(UnkSPBTutoAction_b53* self, void* arg) {
    fn_7100a55ac0(self, arg);
    base::UnkRefCounted* target = self->first.Resolve();
    if (target && Actor(target)) {
        asm("" : "+r"(target));
        if (char* comp = reinterpret_cast<char*>(base::reflection::refl_FindTyped<UnkRefl_fn_71005012bc>(Actor(target))))
            self->Attach(comp);
    } else {
        base::UnkRefCounted* target2 = self->second.Resolve();
        if (target2 && Actor(target2)) {
            asm("" : "+r"(target2));
            char* unit = reinterpret_cast<char*>(fn_71007d74b0(Actor(target2)));
            char* link = *reinterpret_cast<char**>(unit + 0x6d8);
            if (link && Pin(unit)) {  // the link is read before the unit is tested
                char* entity = *reinterpret_cast<char**>(link + 0x10);
                if (entity) {
                    self->field_0x1b0 = *reinterpret_cast<void**>(entity + 0x278);
                    if (char* comp = reinterpret_cast<char*>(base::reflection::refl_FindTyped<UnkRefl_fn_71005012bc>(entity)))
                        self->Attach(comp);
                }
            }
        }
    }
    self->inner.Run(self->first, self->second, arg);
}
