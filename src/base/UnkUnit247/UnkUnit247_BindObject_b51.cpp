// Unit 247: binds an object (the argument's +0x78) to the list at +0x80 (unit 207): with
// `by_name`, once for each of the names at +0xd8 that the object has (fn_7100199558 on the
// name's hash, fn_710016ac04), else as a whole (fn_710016aa9c). Names and layouts are
// placeholders inferred from the code.

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

base::UnkStrIdMember* fn_71000ce740(base::UnkVector<base::UnkStrIdMember>* self, u32 index);
bool fn_7100199558(void* object, const u64* hash);
void fn_710016ac04(void* list, void* object, base::UnkStrIdMember* name);
void fn_710016aa9c(void* list, void* object);

struct UnkBindSource_71001bbee4 {
    u8 pad_0x0[0x78];
    void* object;
};

struct UnkBinder_71001bbee4 {
    u8 pad_0x0[0x80];
    u8 list[0x58];
    base::UnkVector<base::UnkStrIdMember> names;
};
static_assert(__builtin_offsetof(UnkBinder_71001bbee4, names) == 0xd8, "");

void fn_71001bbee4(UnkBinder_71001bbee4* self, const UnkBindSource_71001bbee4* source, bool by_name) {
    void* object = source->object;
    if (by_name) {
        u32 count = self->names.count;
        if (count) {
            for (u32 i = 0; i != count; ++i) {
                u64 hash = *reinterpret_cast<const u64*>(
                    reinterpret_cast<const char*>(fn_71000ce740(&self->names, i)->data) + 0x30);
                if (fn_7100199558(object, &hash))
                    fn_710016ac04(self->list, object, fn_71000ce740(&self->names, i));
            }
            return;
        }
    }
    fn_710016aa9c(self->list, object);
}
