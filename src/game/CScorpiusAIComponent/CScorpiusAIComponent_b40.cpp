// CScorpiusAIComponent unit: initialisation (fn_7100691dc8). Runs the base step, keeps the owner's object
// at 0x149a0 when it is a CCharClassScorpiusAIComponent (null otherwise), and registers eight embedded
// entries in the list at 0x1618 (the count, then the entries), each initialised from its part of that
// object. Returns bit 0 of the base step's result. The same shape as fn_71005fbdb0 in
// src/game/CKraidAIComponent/CKraidAIComponent_b40.cpp. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

typedef u64 u64_a4_sc __attribute__((aligned(4)));

struct UnkSelf;
u32 fn_71003ac8f4(UnkSelf*);
void fn_710068c850(void* entry, u64 source);
void fn_710068b828(void* entry, u64 source);
void fn_710068ada0(void* entry, u64 source);
void fn_7100686f04(void* entry, u64 source);
void fn_710068c220(void* entry, u64 source);
void fn_7100689ac8(void* entry, u64 source);
void fn_71006883c0(void* entry, u64 source);
void fn_7100687d60(void* entry, u64 source);

static inline __attribute__((always_inline)) void Register_sc(char* self, u32 offset, void (*init)(void*, u64),
                                                              u32 from) {
    char* entry = self + offset;
    char* list = self + 0x161c;
    u32 index = *reinterpret_cast<u32*>(self + 0x1618);
    *reinterpret_cast<u64_a4_sc*>(list + ((u64)index << 3)) = reinterpret_cast<u64>(entry);
    *reinterpret_cast<u32*>(self + 0x1618) = index + 1;
    init(entry, *reinterpret_cast<u64*>(self + 0x149a0) + from);
}

u32 fn_7100691dc8(char* self) {
    u32 result = fn_71003ac8f4(reinterpret_cast<UnkSelf*>(self));
    base::reflection::UnkReflObject* object = *reinterpret_cast<base::reflection::UnkReflObject**>(self + 0x10);
    base::reflection::UnkReflObject* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_710068f63c()))
        component = object;
    *reinterpret_cast<base::reflection::UnkReflObject**>(self + 0x149a0) = component;
    Register_sc(self, 0x149a8, fn_710068c850, 0x628);
    Register_sc(self, 0x149e0, fn_710068b828, 0x720);
    Register_sc(self, 0x14a28, fn_710068ada0, 0x820);
    Register_sc(self, 0x14a60, fn_7100686f04, 0x918);
    Register_sc(self, 0x14ab8, fn_710068c220, 0xa10);
    Register_sc(self, 0x14af0, fn_7100689ac8, 0xb08);
    Register_sc(self, 0x14b58, fn_71006883c0, 0xc10);
    Register_sc(self, 0x14bf0, fn_7100687d60, 0xd18);
    return result & 1;
}
