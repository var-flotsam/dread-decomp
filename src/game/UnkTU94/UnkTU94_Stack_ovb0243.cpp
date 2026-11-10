// Unit 94 (base::serialization::CTypedRefWriter's unit, see src/base/serialization/CTypedRefWriter.cpp): two
// queries on the writer's context stack (a count and entries of 0xd8 bytes, each a context in use or
// storage for one held in place, with the type of what the storage holds). Names and layouts are
// placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflAttrKey;
using base::reflection::UnkReflValue;

struct UnkTypedRefWriter;
void fn_71000f07fc(UnkTypedRefWriter* self);

// The functions of a held type (only the destroy slot is named).
struct UnkHeldType_f29a0 {
    u8 pad_0x0[0x40];
    void (*destroy)(void* storage);
};

struct UnkTypedRefEntry_f29a0 {
    void* context;
    u8 storage[0xc8];
    UnkHeldType_f29a0* storage_type;  // 0xd0
};
static_assert(sizeof(UnkTypedRefEntry_f29a0) == 0xd8, "entry layout");

struct UnkTypedRefWriter {
    u32 field_0x0;
    u32 count;
    alignas(8) u8 storage[sizeof(UnkTypedRefEntry_f29a0)];

    UnkTypedRefEntry_f29a0* entries() { return reinterpret_cast<UnkTypedRefEntry_f29a0*>(storage); }
};

bool fn_71000f29a0(UnkTypedRefWriter* self) {
    u32 count = self->count - 1;
    if (count < self->count) {
        for (u64 i = count; i < self->count; ++i) {
            UnkTypedRefEntry_f29a0& entry = self->entries()[i];
            if (entry.storage_type) {
                entry.storage_type->destroy(entry.storage);
                entry.storage_type = nullptr;
            }
        }
    } else {
        for (u64 i = self->count; i < count; ++i) {
            self->entries()[i].context = nullptr;
            self->entries()[i].storage_type = nullptr;
        }
    }
    self->count = count;
    fn_71000f07fc(self);
    return true;
}

// The class descriptor of SBaseContext (src/game_s1/fn_71000f3150.cpp builds it).
struct UnkReflType;
extern UnkReflType g_unk_7101d67d80;

// What a context held in place keeps at +0x20 of its storage: an object whose +0x80 is a class
// descriptor with attributes.
struct UnkBaseContext_f2af4 {
    u8 pad_0x0[0x20];
    struct Source {
        u8 pad_0x0[0x80];
        void* field_0x80;
    }* source;  // 0x20
};

const base::reflection::UnkReflAttrKey& fn_7100090800();  // the attribute "bBinaryDumpable"
UnkReflValue fn_71000e8044(void* cls, const UnkReflAttrKey& key);

// Whether the class of the innermost context's source has the attribute "bBinaryDumpable" set.
bool fn_71000f2af4(UnkTypedRefWriter* self) {
    u8 result = 0;
    if (u32 count = self->count) {
        u32 top = count - 1;
        UnkTypedRefEntry_f29a0& entry = self->entries()[top];
        if (fn_710009d50c(reinterpret_cast<const CType*>(entry.storage_type),
                          reinterpret_cast<const CType*>(&g_unk_7101d67d80))) {
            auto* context = reinterpret_cast<UnkBaseContext_f2af4*>(entry.storage);
            if (context->source) {
                UnkReflValue value = fn_71000e8044(context->source->field_0x80, fn_7100090800());
                if (value.type) {
                    if (fn_710009d50c(reinterpret_cast<const CType*>(value.type), fn_710000d228())) {
                        void* address = value.field_0x14 == 1 || *reinterpret_cast<const u64*>(value.type + 0x18) > 8
                                            ? value.field_0x0
                                            : &value;
                        result = *static_cast<u8*>(address);
                    }
                }
                fn_71000a1c60(&value);
            }
        }
    }
    return result != 0;
}
