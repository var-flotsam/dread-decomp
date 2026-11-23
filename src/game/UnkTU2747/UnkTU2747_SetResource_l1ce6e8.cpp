// UnkTU2747 (GUI::CDisplayObject's unit): looks a resource up by its name, lower-cased, and unless the flag
// at +0x10 is set or the name is the current one, takes the name and the flag, points the object's
// instance at the resource (creating it on first use) and refreshes the object. Names and layouts are
// placeholders.
#include <string.h>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrId;

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkInlineString_l1ce6e8 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_l1ce6e8() {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_l1ce6e8() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};

struct UnkDisplayResource_l1ce6e8;
struct UnkDisplayResourceSet_l1ce6e8;

// What shows the resource (0x118 bytes).
struct UnkDisplayInstance_l1ce6e8 {
    UnkDisplayResource_l1ce6e8* resource;
    u8 pad_0x8[0x110];
};

// The resource under a name, or null.
UnkDisplayResource_l1ce6e8** fn_7100b068d4(UnkDisplayResourceSet_l1ce6e8* set, const CRntString* name);
void fn_7100b039a4(UnkDisplayInstance_l1ce6e8* self, UnkDisplayResource_l1ce6e8* resource);  // constructor
void fn_7100b03c78(UnkDisplayInstance_l1ce6e8* self);                                         // reset

struct UnkDisplayObject_l1ce6e8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58(f32 time);
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();

    u8 pad_0x8[0x8];
    bool field_0x10;
    u8 pad_0x11[0xf];
    UnkStrId name;                            // 0x20
    UnkDisplayResourceSet_l1ce6e8* resources;  // 0x28
    UnkDisplayInstance_l1ce6e8* instance;     // 0x30
};
static_assert(__builtin_offsetof(UnkDisplayObject_l1ce6e8, instance) == 0x30, "layout");

void fn_71011ce6e8(UnkDisplayObject_l1ce6e8* self, const char* name, bool flag) {
    UnkDisplayResourceSet_l1ce6e8* resources = self->resources;
    if (!resources || self->field_0x10)
        return;

    char lower[0x100];
    lower[0] = 0;
    u32 len = strlen(name);
    if (len != 0) {
        u64 i = 0;
        do {
            char c = name[i];
            if (c >= 'A' && c <= 'Z')
                c += 0x20;
            lower[i] = c;
        } while (++i < len);
        lower[len] = 0;
    }

    UnkDisplayResource_l1ce6e8** found;
    {
        UnkInlineString_l1ce6e8 key;
        CRntString_AssignCString(key.Str(), lower);
        found = fn_7100b068d4(resources, key.Str());
    }
    if (!found)
        return;
    {
        CStrId id(name, true);
        if (self->name.data == id.data)
            return;
    }

    self->name = UnkStrId(name, true);
    self->field_0x10 = flag;
    if (self->instance) {
        fn_7100b03c78(self->instance);
        UnkDisplayInstance_l1ce6e8* instance = self->instance;
        instance->resource = *found;
    } else {
        UnkDisplayInstance_l1ce6e8* instance =
            static_cast<UnkDisplayInstance_l1ce6e8*>(fn_71000839a0(sizeof(UnkDisplayInstance_l1ce6e8), 0xc));
        fn_7100b039a4(instance, *found);
        self->instance = instance;
    }
    self->vf_0x58(0.0f);
    self->vf_0xa0();
}
