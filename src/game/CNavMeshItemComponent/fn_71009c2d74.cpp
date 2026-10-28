// Reflected setters that select an object's source by id: the current one if it already has the
// id, else the first listed source with it (none for id -1 or when no source has it).
#include "base/reflection/UnkReflValue.h"

using base::reflection::refl_ArgValue;
using base::reflection::refl_EmptyValue;
using base::reflection::UnkReflValue;

struct UnkSourceDesc {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkSource {
    u8 pad_0x0[0x40];
    UnkSourceDesc* desc;
};

struct UnkSourceList;
// The element at `index`, growing the list when it is growable (base::UnkVector::GrowAt).
UnkSource*& fn_71009c0bf0(UnkSourceList* self, u32 index);

struct UnkSourceList {
    u8 pad_0x0[0x8];
    u32 count;
};

struct UnkOwner {
    u8 pad_0x0[0x148];
    UnkSourceList sources;
    u8 pad_0x154[0x2c];
    UnkSource* current;
};

void fn_71009bfda8(UnkOwner* self, UnkSource* src);

UnkReflValue fn_71009c2d74(void*, char* obj, UnkReflValue* const* args) {
    UnkOwner* self = *reinterpret_cast<UnkOwner**>(obj + 8);
    u64 id = *refl_ArgValue<u64>(args[0]);
    if (!self->current || self->current->desc->id != id) {
        UnkSource* found = nullptr;
        if (id != u64(-1)) {
            for (u32 i = 0; i < self->sources.count; ++i) {
                UnkSource* src = fn_71009c0bf0(&self->sources, i);
                if (src && src->desc->id == id) {
                    found = src;
                    break;
                }
            }
        }
        fn_71009bfda8(self, found);
    }
    return refl_EmptyValue();
}

// The descriptor whose id fn_71009c28b8 looks for (a global pointer in .bss).
extern __attribute__((visibility("hidden"))) UnkSourceDesc* g_unk_717273ee10;

struct UnkSourceArg;
void fn_71009c5ab8(UnkSource* src, UnkSourceArg* arg);

// Passes the argument (null when it has no type) to the first listed source with that id.
UnkReflValue fn_71009c28b8(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkOwner* self = *reinterpret_cast<UnkOwner**>(obj + 8);
    UnkSourceArg* value = arg->type ? const_cast<UnkSourceArg*>(refl_ArgValue<UnkSourceArg>(arg)) : nullptr;
    u64 id = g_unk_717273ee10->id;
    if (id != u64(-1)) {
        for (u32 i = 0; i < self->sources.count; ++i) {
            UnkSource* src = fn_71009c0bf0(&self->sources, i);
            if (src && src->desc->id == id) {
                fn_71009c5ab8(src, value);
                break;
            }
        }
    }
    return refl_EmptyValue();
}
