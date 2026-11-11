// UnkTU42: walks a range of object pointers and calls vf_0x18 on those passing the iterator's filters,
// stopping once the iterator's tracked value changes. Names and layouts are placeholders.
#include "types.h"

struct UnkDispatchObject;
struct UnkDispatchArgs {
    u64 value;
    u64 arg;
};
struct UnkDispatchObjectVtbl {
    void* slots[3];
    void (*vf_0x18)(UnkDispatchObject*, UnkDispatchArgs*);
};
struct UnkDispatchObject {
    UnkDispatchObjectVtbl* vtbl;
    u32 field_0x8;
};

struct UnkDispatchIter {
    u8 pad_0x0[8];
    u64 state;
    u64 arg;
    UnkDispatchObject** cursor;
    u32 mask;
    u8 ignoreBits;
    u8 pad_0x25[3];
    u8 enabled;
};

bool fn_71000687d8(UnkDispatchIter* self, UnkDispatchObject** begin, UnkDispatchObject** end, bool exact) {
    u64 original = self->state;
    self->cursor = begin;
    if (!(begin < end))
        return false;
    if (!exact) {
        for (;;) {
            if (self->enabled) {
                UnkDispatchObject* obj = *self->cursor;
                u32 flags = obj->field_0x8;
                if ((self->mask & (1u << ((flags >> 8) & 1))) && !(flags & (u8)~self->ignoreBits)) {
                    UnkDispatchArgs args = {original, self->arg};
                    obj->vtbl->vf_0x18(obj, &args);
                    if (original != self->state)
                        break;
                }
            }
            self->cursor++;
            if (!(self->cursor < end))
                return false;
        }
    } else {
        for (;;) {
            UnkDispatchObject* obj = *self->cursor;
            u32 flags = obj->field_0x8;
            if (!(flags & 0x100) && self->enabled && (self->mask & 1) && !(flags & (u8)~self->ignoreBits)) {
                UnkDispatchArgs args = {original, self->arg};
                obj->vtbl->vf_0x18(obj, &args);
                if (original != self->state)
                    break;
            }
            self->cursor++;
            if (!(self->cursor < end))
                return false;
        }
    }
    self->cursor++;
    return true;
}
