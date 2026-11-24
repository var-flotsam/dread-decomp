// CActorComponent's unit (tools/tu_map.py unit 342): the property handler. Three properties run a virtual
// of the component on the stream (two with the stream, one without), one reads a flag and switches the
// component's on/off virtuals when the flag's effect changes, and one looks a function up by name in the
// class's entry table. Anything else answers 2. Names and layouts are placeholders.
#include "../../../src/game_s7/UnkPropS7.h"
#include "base/UnkClassEntry.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/UnkTU56.h"
#include "types.h"

using namespace unk_prop;
using base::reflection::CClass;

struct UnkActorComp_l24fc10;
void fn_7100023d2c(UnkActorComp_l24fc10* self, UnkClassEntry* entry, void* stream);

// What the component's flag depends on.
struct UnkActorLink_l24fc10 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;
};

struct UnkActorComp_l24fc10 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkActorLink_l24fc10* link;  // 0x18
    bool flag;  // 0x20

    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkActorComp_l24fc10*)>(vtable[0x0 / 8])(this); }
    void vf_0x58() { reinterpret_cast<void (*)(UnkActorComp_l24fc10*)>(vtable[0x58 / 8])(this); }
    void vf_0xb8(void* stream) { reinterpret_cast<void (*)(UnkActorComp_l24fc10*, void*)>(vtable[0xb8 / 8])(this, stream); }
    void vf_0xc0(void* stream) { reinterpret_cast<void (*)(UnkActorComp_l24fc10*, void*)>(vtable[0xc0 / 8])(this, stream); }
    void vf_0x108() { reinterpret_cast<void (*)(UnkActorComp_l24fc10*)>(vtable[0x108 / 8])(this); }
    void vf_0x110() { reinterpret_cast<void (*)(UnkActorComp_l24fc10*)>(vtable[0x110 / 8])(this); }
};

u32 fn_710024fc10(UnkActorComp_l24fc10* self, UnkProp* p) {
    if (Is(p, Key(0x2008))) {
        self->vf_0xb8(p->stream);
        return 1;
    }
    if (Is(p, Key(0x2010))) {
        self->vf_0xc0(p->stream);
        return 1;
    }
    if (Is(p, Key(0x8c8))) {
        self->vf_0x58();
        return 1;
    }
    if (Is(p, Key(0x1fc8))) {
        bool value = false;
        CRntFile_Read(reinterpret_cast<base::global::CRntFile*>(p->stream), &value, 1, 1);
        bool flag = value;
        bool old = self->flag;
        if (old == flag)
            return 1;
        bool was_on = old;
        if (old && self->link) {
            if (!self->link->field_0x1b4)
                was_on = false;
            else if (!self->link->field_0x1b0)
                was_on = true;
            else
                was_on = self->link->field_0x1a8 != nullptr;
        }
        self->flag = flag;
        if (flag && self->link && self->link->field_0x1b4) {
            if (!self->link->field_0x1b0) {
                if (!was_on)
                    self->vf_0x108();
            } else {
                bool now_on = self->link->field_0x1a8 != nullptr;
                if (was_on != now_on) {
                    if (was_on)
                        self->vf_0x110();
                    else
                        self->vf_0x108();
                }
            }
        } else if (was_on) {
            self->vf_0x110();
        }
        return 1;
    }
    if (Is(p, Key(0x2058))) {
        const char* name;
        CRntFile_ReadCString(p->stream, &name);
        u64 crc = StringCrc64(name, __builtin_strlen(name));
        const base::reflection::UnkReflArray& entries = self->vf_0x0()->m_array_0xe8;
        const u64* keys = static_cast<const u64*>(entries.keys);
        u32 i = 0;
        for (; i < entries.count; ++i) {
            if (keys[i] == crc)
                break;
        }
        UnkClassEntry* first = static_cast<UnkClassEntry*>(entries.data);
        UnkClassEntry* found = first + i;
        fn_7100023d2c(self, found == first + entries.count ? nullptr : found, p->stream);
        return 1;
    }
    return 2;
}
