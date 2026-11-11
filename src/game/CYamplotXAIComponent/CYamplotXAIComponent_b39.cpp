// CYamplotXAIComponent unit (tools/tu_map.py): the constructor. After the base constructor
// (fn_710036932c) and the vtable it builds the members at +0x146c8 (fn_71006ec9f0) and +0x146f8
// (fn_71006ed608), clears two 16-byte records, sets +0x14780 to -1, clears the 48 bytes after it,
// builds the object at +0x147c8 (fn_71002b57bc), clears +0x148c8 and finishes with vtable slot 0x18.
// The fields are addressed by offset, as in src/game/CCentralUnitAIComponent/fn_71004076e4.cpp.
// Names and layouts are placeholders.
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CYamplotXAIComponent.h"
#include "game/UnkObject_0x100.h"
#include "types.h"

// Declared as src/game/CCentralUnitAIComponent/fn_71004076e4.cpp defines it.
struct UnkComponent;
void fn_71002b57bc(UnkObject_0x100* self);
void fn_71006ec9f0(void* member);
void fn_71006ed608(void* member);

struct UnkZero16_yxa {
    u64 words[2];
};

struct UnkZero48_yxa {
    u64 words[6];
};

template <class T>
static inline T& At_yxa(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

struct UnkYamplotXAI_yxa {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};

void fn_71006f3190(UnkYamplotXAI_yxa* self) {
    fn_710036932c(reinterpret_cast<UnkComponent*>(self));
    At_yxa<void**>(self, 0) = g_unk_7101a6ae18 + 2;
    fn_71006ec9f0(reinterpret_cast<char*>(self) + 0x146c8);
    fn_71006ed608(reinterpret_cast<char*>(self) + 0x146f8);
    At_yxa<UnkZero16_yxa>(self, 0x14730) = {};
    At_yxa<UnkZero16_yxa>(self, 0x14770) = {};
    At_yxa<u32>(self, 0x14780) = 0xffffffff;
    At_yxa<UnkZero48_yxa>(self, 0x14784) = {};
    fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(reinterpret_cast<char*>(self) + 0x147c8));
    At_yxa<u32>(self, 0x148c8) = 0;
    self->vf_0x18();
}
