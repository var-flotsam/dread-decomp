// Constructor of an object derived from fn_710041a198's: sets its vtable, constructs the sub-objects
// at +0x528, +0x658, +0x7e8, +0x8e8, +0x9f0 and +0xb38, a growable array at +0xc40 (flags 0xf) and
// another sub-object at +0xc78, sets a few flags and finishes in vtable slot 0x18. The members are
// addressed by offset; their types are unknown.
#include <new>

#include "base/UnkVector.h"
#include "game/UnkBossAIComponent.h"
#include "types.h"

typedef void** __attribute__((may_alias)) UnkVtable;

struct UnkNode : UnkBossAIComponent {};

void fn_71004a4ffc(void*);
void fn_71004ae6f4(void*);
void fn_71004b42b8(void*);
void fn_71004b4ee8(void*);
void fn_71004b9cc4(void*);
void fn_71004a1790(void*);
void fn_71008c2da8(void*);
extern void* g_unk_7101a3ebe8[];

template <class T>
static inline void St(UnkNode* self, u32 off, T value) {
    *reinterpret_cast<T*>(reinterpret_cast<char*>(self) + off) = value;
}

static inline char* At(UnkNode* self, u32 off) { return reinterpret_cast<char*>(self) + off; }

// NON_MATCHING(minor): the zero stores at +0xc40 and +0xc48 are made in the other order
void fn_71004ce858(UnkNode* self) {
    fn_710041a198(self);
    self->vtable = g_unk_7101a3ebe8 + 2;
    fn_71004a4ffc(At(self, 0x528));
    fn_71004ae6f4(At(self, 0x658));
    fn_71004b42b8(At(self, 0x7e8));
    fn_71004b4ee8(At(self, 0x8e8));
    fn_71004b9cc4(At(self, 0x9f0));
    fn_71004a1790(At(self, 0xb38));
    St<u32>(self, 0xc50, 10);
    St<u64>(self, 0xc40, 0);
    St<u64>(self, 0xc48, 0);
    St<u8>(self, 0xc54, 1);
    St<u32>(self, 0xc58, 0xf);
    St<void*>(self, 0xc60, fn_7100083990());
    fn_71008c2da8(At(self, 0xc78));
    self->field_0x4fc = 0x101;
    *reinterpret_cast<u8*>(At(self, 0x409)) = 0;
    *reinterpret_cast<u8*>(At(self, 0x4dc)) = 1;
    reinterpret_cast<void (*)(UnkNode*)>(self->vtable[3])(self);
}
