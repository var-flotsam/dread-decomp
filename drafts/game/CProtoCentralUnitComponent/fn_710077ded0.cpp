#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

using UnkRef = base::UnkRefPtrMember;
UnkRef fn_7100353be8(void* source);
struct UnkProtoCentralUnitBase_ovb0265;
void fn_71007744d0(UnkProtoCentralUnitBase_ovb0265* self);
void* fn_71002b3b84(void* object);
void fn_71009c0f1c(void* object, u64 id);
bool fn_7101196fd8(void* manager);

struct UnkProtoUiManager {
    u8 pad_0x0[0xb14];
    u8 field_0xb14;
};
extern UnkProtoUiManager* g_unk_7101d47330;

struct UnkProtoTarget {
    u8 pad_0x0[0x10];
    char* object;  // 0x10
};

struct UnkProtoCentralUnit_ovb0265 {
    u8 pad_0x0[0xa08];
    u8 field_0xa08[0x28];
    u32 state;  // 0xa30
    u8 pad_0xa34[4];
    base::UnkRefHandle target;  // 0xa38
};

static inline void TakeTarget(UnkProtoCentralUnit_ovb0265* self) {
    if (self->state == 1) {
        {
            base::UnkRefPtrMember handle = fn_7100353be8(self->field_0xa08);
            fn_710031d208(&self->target, reinterpret_cast<base::UnkRefHandle*>(&handle));
        }
        self->state = 2;
    }
}

void fn_710077ded0(UnkProtoCentralUnit_ovb0265* self, s32 message) {
    fn_71007744d0(reinterpret_cast<UnkProtoCentralUnitBase_ovb0265*>(self));
    if (message == 7)
        return;
    TakeTarget(self);
    UnkProtoTarget* target = reinterpret_cast<UnkProtoTarget*>(self->target.ptr);
    if (!target)
        return;
    if (!target->object)
        return;
    // the original reads the object again
    asm("" : "+r"(target));
    if (void* o = fn_71002b3b84(target->object))
        fn_71009c0f1c(o, 0xe43bbd07b9e307b9);
    if (fn_7101196fd8(g_unk_7101d47330))
        return;
    if (g_unk_7101d47330->field_0xb14)
        return;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    TakeTarget(self);
    void* animation = *reinterpret_cast<void**>(reinterpret_cast<UnkProtoTarget*>(self->target.ptr)->object + 0x220);
    if (!animation)
        return;
    mode.id = 0;
    mask.id = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
    fn_7101176690(animation, 0xe43bbd07b9e307b9, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                  &mask);
}
