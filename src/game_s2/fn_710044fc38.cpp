// Sets up a chozo commander AI component's sub-components: casts the owner's object to the
// component's actor class, then registers sixteen state blocks (each added to a list in the
// component and initialised from the actor).
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkCmdActor {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkCmdActor*)>(vtable[0])(this); }
};

struct __attribute__((packed)) UnkCmdComponent {
    u8 pad_0x0[0x10];
    UnkCmdActor* field_0x10;
    u8 pad_0x18[0x1600];
    u32 field_0x1618;
    void* field_0x161c[0x1000];
    u8 pad_after[0x1];
};

bool fn_71003ac8f4(UnkCmdComponent* self);

void fn_710043b630(void* state, char* actor);
void fn_7100435160(void* state, char* actor);
void fn_710043a36c(void* state, char* actor);
void fn_71004389e4(void* state, char* actor);
void fn_7100427ac4(void* state, char* actor);
void fn_710042e670(void* state, char* actor);
void fn_71004374d0(void* state, char* actor);
void fn_71004250ac(void* state, char* actor);
void fn_7100443b24(void* state, char* actor);
void fn_710042879c(void* state, char* actor);
void fn_710042c980(void* state, char* actor);
void fn_710042ab7c(void* state, char* actor);
void fn_7100446744(void* state, char* actor);
void fn_710042c190(void* state, char* actor);
void fn_710043e1e4(void* state, char* actor);
void fn_71004303c8(void* state, char* actor);

// The actor pointer is kept at +0x149a0 of the component.
struct __attribute__((packed)) UnkCmdActorRef {
    u8 pad[0x149a0];
    char* actor;
};

__attribute__((always_inline)) static inline void RegisterState(UnkCmdComponent* self, u32 member_off,
                                                                void (*init)(void*, char*), u32 actor_off) {
    char* base = reinterpret_cast<char*>(self);
    char* member = base + member_off;
    // Addressed by hand: an array member would be materialised ahead of the first store.
    char* list = base + 0x161c;
    u32 n = self->field_0x1618;
    *reinterpret_cast<void**>(list + ((u64)n << 3)) = member;
    self->field_0x1618 = n + 1;
    init(member, reinterpret_cast<UnkCmdActorRef*>(self)->actor + actor_off);
}

bool fn_710044fc38(UnkCmdComponent* self) {
    bool result = fn_71003ac8f4(self);
    UnkCmdActor* obj = self->field_0x10;
    char* actor = nullptr;
    if (obj && fn_710009d50c(obj->vf_0x0(), fn_710044d0c0()))
        actor = reinterpret_cast<char*>(obj);
    reinterpret_cast<UnkCmdActorRef*>(self)->actor = actor;
    RegisterState(self, 0x149a8, fn_710043b630, 0x528);
    RegisterState(self, 0x14a28, fn_7100435160, 0x628);
    RegisterState(self, 0x14ab0, fn_710043a36c, 0x770);
    RegisterState(self, 0x14b18, fn_71004389e4, 0x870);
    RegisterState(self, 0x14b90, fn_7100427ac4, 0x988);
    RegisterState(self, 0x14bf0, fn_710042e670, 0xa88);
    RegisterState(self, 0x14c68, fn_71004374d0, 0xb98);
    RegisterState(self, 0x14ce8, fn_71004250ac, 0xca0);
    RegisterState(self, 0x14d70, fn_7100443b24, 0xd90);
    RegisterState(self, 0x14e10, fn_710042879c, 0xea8);
    RegisterState(self, 0x14ec0, fn_710042c980, 0xf98);
    RegisterState(self, 0x14f58, fn_710042ab7c, 0x10a0);
    RegisterState(self, 0x14fb8, fn_7100446744, 0x11a8);
    RegisterState(self, 0x15078, fn_710042c190, 0x12b8);
    RegisterState(self, 0x150d0, fn_710043e1e4, 0x13c0);
    RegisterState(self, 0x157f0, fn_71004303c8, 0x14b0);
    return result;
}
