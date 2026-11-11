// behaviortree::CFloatCondition's unit (tools/tu_map.py): two float operands (+0x38, +0x58), each a constant
// that a tunable (group and name ids) or a blackboard property can override, compared with the operator at
// +0x78 (1 <, 2 <=, 3 ==, 4 >=, 5 >, equality within 0.001): fn_710039c074 evaluates it, fn_710039c2b0 describes
// it as text. Names and layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkTunableRegistry.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::reflection::UnkTunableRegistry;

struct UnkTunableName_ovb0121 {
    u8 unk_0x0[0x10];
    const char* text;
    u8 unk_0x18[0x2c];
    // A union member: the original reloads the ids after the increment.
    union {
        u32 uses;
    };
};

// An operand: a constant, a blackboard property when its name is set, or a tunable when both its
// group and name are set.
struct UnkFloatOperand_ovb0121c {
    f32 value;
    u8 unk_0x4[0x4];
    base::global::CStrId property;     // 0x8
    UnkTunableName_ovb0121* group;     // 0x10
    UnkTunableName_ovb0121* name;      // 0x18
};

struct UnkFloatCondition_ovb0121c {
    u8 unk_0x0[0x38];
    UnkFloatOperand_ovb0121c a;  // 0x38
    UnkFloatOperand_ovb0121c b;  // 0x58
    s32 op;                      // 0x78
};

struct UnkSelf;
bool fn_7100394e2c(UnkSelf*, void*, f32*);
void fn_710013c7e0(UnkTunableRegistry* registry, const char* group, const char* name, f32* value);

static inline void Resolve(UnkFloatCondition_ovb0121c* self, UnkFloatOperand_ovb0121c& operand, f32* out) {
    base::global::CStrId* property = &operand.property;
    if (operand.group != reinterpret_cast<UnkTunableName_ovb0121*>(&g_unk_7101d47108->empty) &&
        operand.name != reinterpret_cast<UnkTunableName_ovb0121*>(&g_unk_7101d47108->empty)) {
        UnkTunableRegistry* registry = g_unk_7101d47350;
        if (!registry) {
            registry = static_cast<UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
            fn_710013ac14(registry);
            g_unk_7101d47350 = registry;
        }
        ++operand.group->uses;
        const char* group = operand.group->text;
        ++operand.name->uses;
        const char* name = operand.name->text;
        fn_710013c7e0(registry, group, name, out);
    } else if (property->data != &g_unk_7101d47108->empty) {
        fn_7100394e2c(reinterpret_cast<UnkSelf*>(self), property, out);
    }
}

static inline bool Near(f32 a, f32 b) {
    return a + -0.001f <= b && a + 0.001f >= b;
}

bool fn_710039c074(UnkFloatCondition_ovb0121c* self) {
    f32 a = self->a.value;
    f32 b = 0.0f;
    Resolve(self, self->a, &a);
    b = self->b.value;
    Resolve(self, self->b, &b);
    switch (self->op) {
    case 1:
        return a < b;
    case 2:
        return a < b || Near(a, b);
    case 3:
        return Near(a, b);
    case 4:
        return a > b || Near(a, b);
    case 5:
        return a > b;
    }
    return false;
}

using base::global::CRntString;

// A blackboard property name: its text at +0x10 and a use count at +0x44.
struct UnkNamedString_ovb0121 {
    u8 unk_0x0[0x10];
    CRntString text;
    u8 unk_0x40[0x4];
    // A union member: the original reloads the ids after the increment.
    union {
        u32 uses;
    };
};

// An operand: a constant, overridden by a blackboard property when its name is set.
struct UnkFloatOperand_ovb0121 {
    f32 value;
    u8 unk_0x4[0x4];
    UnkNamedString_ovb0121* property;
    u8 unk_0x10[0x10];
};

struct UnkFloatCondition_ovb0121 {
    u8 unk_0x0[0x38];
    UnkFloatOperand_ovb0121 a;  // 0x38
    UnkFloatOperand_ovb0121 b;  // 0x58
    s32 op;                     // 0x78
};

// The operator names, for op 1 to 5.
extern const char* const g_unk_7101a27b98[] __attribute__((visibility("hidden")));

void fn_710007f564(void*, const char*);

struct UnkText_ovb0121 {
    u8 unk_0x0[0x30];
    UnkText_ovb0121() {}
};

static inline void AppendOperand(UnkText_ovb0121* out, const UnkFloatOperand_ovb0121& operand) {
    if (operand.property != reinterpret_cast<UnkNamedString_ovb0121*>(&g_unk_7101d47108->empty)) {
        ++operand.property->uses;
        fn_710007f3b4(reinterpret_cast<CRntString*>(out), &operand.property->text);
    } else {
        fn_710007f5e0(out, operand.value);
    }
}

static inline const char* OpName(s32 op) {
    if (op >= 1 && op <= 5)
        return g_unk_7101a27b98[op - 1];
    return g_unk_710162e2ed;
}

UnkText_ovb0121 fn_710039c2b0(UnkFloatCondition_ovb0121* self) {
    UnkText_ovb0121 text;
    fn_710007e704(reinterpret_cast<base::UnkNamedEntry*>(&text), g_unk_710162e2ed);
    AppendOperand(&text, self->a);
    fn_710007f564(&text, " ");
    fn_710007f564(&text, OpName(self->op));
    fn_710007f564(&text, " ");
    AppendOperand(&text, self->b);
    fn_710007f564(&text, "?");
    return text;
}
