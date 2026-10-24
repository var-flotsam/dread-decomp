// Sets up a component's sub-components: casts the owner's object to the component's actor class,
// then registers six state blocks (each added to a list in the component and initialised from
// the actor's data).
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkStateActor {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkStateActor*)>(vtable[0])(this); }
};

struct __attribute__((packed)) UnkStateComponent {
    u8 pad_0x0[0x10];
    UnkStateActor* field_0x10;
    u8 pad_0x18[0x1600];
    u32 field_0x1618;
    void* field_0x161c[0x1000];
    u8 pad_after[0x1];
};

bool fn_7100498a38(UnkStateComponent* self);
const CClass* fn_710048d394();  // the actor class's refl_GetClass

void fn_7100481794(void* state, char* data);
void fn_710048cb88(void* state, char* data);
void fn_71004825c4(void* state, char* data);
void fn_710048069c(void* state, char* data);
void fn_7100483b88(void* state, char* data);
void fn_7100486d84(void* state, char* data);

// The actor pointer is kept at +0x14b70 of the component.
struct __attribute__((packed)) UnkStateActorRef {
    u8 pad[0x14b70];
    char* actor;
};

__attribute__((always_inline)) static inline void RegisterState(UnkStateComponent* self, u32 member_off,
                                                                void (*init)(void*, char*), u32 data_off) {
    char* member = reinterpret_cast<char*>(self) + member_off;
    // Addressed by hand: an array member would be materialised ahead of the first store.
    char* list = reinterpret_cast<char*>(self) + 0x161c;
    u32 n = self->field_0x1618;
    *reinterpret_cast<void**>(list + ((u64)n << 3)) = member;
    self->field_0x1618 = n + 1;
    init(member, reinterpret_cast<UnkStateActorRef*>(self)->actor + data_off);
}

bool fn_710048e0e4(UnkStateComponent* self) {
    bool result = fn_7100498a38(self);
    UnkStateActor* obj = self->field_0x10;
    char* actor = nullptr;
    if (obj && fn_710009d50c(obj->vf_0x0(), fn_710048d394()))
        actor = reinterpret_cast<char*>(obj);
    reinterpret_cast<UnkStateActorRef*>(self)->actor = actor;
    RegisterState(self, 0x14b78, fn_7100481794, 2056);
    RegisterState(self, 0x14be8, fn_710048cb88, 2296);
    RegisterState(self, 0x14c38, fn_71004825c4, 2536);
    RegisterState(self, 0x14ca0, fn_710048069c, 2776);
    RegisterState(self, 0x14d08, fn_7100483b88, 3016);
    RegisterState(self, 0x14dd0, fn_7100486d84, 3256);
    return result;
}
