// CProfessorDoorComponent (tools/tu_map.py unit 1249; the name from reflection): a reflected setter
// taking a bool. The owner's (+8) object at +0x30 is resolved (fn_71002b3b84); when there is one it
// gets fn_71009c0f1c with one of two hashed ids picked by the bool. Returns the empty value.
// Names and layout are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkWeakTarget;
void* fn_71002b3b84(UnkWeakTarget*);
void fn_71009c0f1c(void* object, u64 id);

struct UnkProfessorDoorOwner_b44a2 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* field_0x30;
};

UnkReflValue fn_71007dde48(void*, char* obj, UnkReflValue* const* args) {
    auto* owner = *reinterpret_cast<UnkProfessorDoorOwner_b44a2**>(obj + 8);
    bool open = *base::reflection::refl_ArgValue<bool>(args[0]);
    if (void* target = fn_71002b3b84(owner->field_0x30))
        fn_71009c0f1c(target, open ? 0xb0c1699f8b57b14bull : 0x13ae275a1bcd6558ull);
    return base::reflection::refl_EmptyValue();
}
