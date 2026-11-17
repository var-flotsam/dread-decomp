// From unit 41 of tools/tu_map.py (base::global::timeline::CEvent): reads an event's fields from a
// file, each as its member's hash then a value of the member's type. Names and layouts are
// placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CName.h"
#include "base/global/CRntFile.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntFile;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

struct UnkTimelineEvent;
struct UnkReflField;
struct UnkReflFieldObject;
struct UnkReflValueArg;
struct UnkEnumType;
struct UnkFlagsetType_b0068;

UnkEnumType* fn_7100067f5c();
UnkEnumType* fn_7100068204();
bool fn_710009a430(UnkFlagsetType_b0068* self, const UnkReflFieldObject* obj, UnkReflField* field, CRntFile* file);
UnkReflTypedPtr fn_71000e755c(UnkReflField* field, const UnkReflTypedPtr* obj);

// The event as read here: its class (vtable slot 0), then a word the file starts with.
struct UnkEventView_06790c {
    virtual char* GetClass();
    u32 field_0x8;
};

// A reflected field (0xb0 bytes): its type at +0x10.
struct UnkEventField_06790c {
    u8 pad_0x0[0x10];
    const CType* type;
    u8 pad_0x18[0x98];
};
static_assert(sizeof(UnkEventField_06790c) == 0xb0, "");

// A class's descriptor as the lookup sees it: its class (vtable slot 0) and its fields by name hash.
struct UnkEventClass_06790c {
    virtual const CType* GetClass();
    u8 pad_0x8[0xe0];
    UnkEventField_06790c* fields;  // +0xe8
    u64* keys;                     // +0xf0
    u32 count;                     // +0xf8

    UnkEventField_06790c* End() { return fields + count; }
    UnkEventField_06790c* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
};

// A field's type as the cast sees it (vtable slot 0 gives its class).
struct UnkEventFieldType_06790c {
    virtual const CType* GetClass();
    u8 pad_0x8[0x8];
    u64 kind;  // +0x10: the hash of the type's name
};

// The colour type with three components; read with all of them at 1 by default.
struct UnkColor3F_06790c {
    f32 r;
    f32 g;
    f32 b;
};

// A value's type and where it is.
struct UnkEventValue_06790c {
    const void* type;
    void* value;
};

static inline void SetField(UnkEventField_06790c* field, const UnkReflTypedPtr* obj, UnkEventValue_06790c* value) {
    fn_71000e7a80(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(obj),
                  reinterpret_cast<UnkReflValueArg*>(value));
}

void fn_710006790c(UnkTimelineEvent* event, CRntFile* file) {
    UnkEventView_06790c* self = reinterpret_cast<UnkEventView_06790c*>(event);
    UnkReflTypedPtr obj;
    obj.type = fn_710009dcd8(self->GetClass(), self);
    obj.ptr = self;
    fn_710007c714(file, &self->field_0x8, 4, 1);
    s32 count = 0;
    fn_710007c714(file, &count, 4, 1);
    for (s32 i = 0; i < count; i++) {
        u64 hash;
        fn_710007c714(file, &hash, 8, 1);
        const u64 key = hash;  // the member's name hash
        UnkEventClass_06790c* owner = reinterpret_cast<UnkEventClass_06790c*>(obj.type);
        UnkEventClass_06790c* cls = nullptr;
        if (owner && fn_710009d50c(owner->GetClass(), fn_7100096100()))
            cls = owner;
        UnkEventField_06790c* field = cls->Find(key);
        if (field == cls->End() || !field)
            continue;
        UnkEventFieldType_06790c* type = (UnkEventFieldType_06790c*)field->type;
        switch (type->kind) {
        case 0xC7058CEBEE667049: {  // a flag set
            UnkFlagsetType_b0068* flagset = nullptr;
            if (type && fn_710009d50c(type->GetClass(), fn_710009a11c()))
                flagset = reinterpret_cast<UnkFlagsetType_b0068*>(type);
            fn_710009a430(flagset, reinterpret_cast<const UnkReflFieldObject*>(&obj),
                          reinterpret_cast<UnkReflField*>(field), file);
            break;
        }
        case 0xBF34F79BBA0DEAF6:
        case 0x8C2DEEC131321516: {  // a string
            u16 length;
            fn_710007c714(file, &length, 2, 1);
            char text[0x80];
            if (length != 0)
                fn_710007c714(file, text, length, 1);
            text[length] = '\0';
            const char* str = text;
            UnkEventValue_06790c arg = {fn_71000081e8(), &str};
            SetField(field, &obj, &arg);
            break;
        }
        case 0x3B98086A3ED2623D:
        case 0xB97CD894D76CACD6: {
            u32 value;
            fn_710007c714(file, &value, 4, 1);
            UnkEventValue_06790c arg = {fn_7100005178(), &value};
            SetField(field, &obj, &arg);
            break;
        }
        case 0x937459BA5ED68A51: {
            f32 value;
            fn_710007c714(file, &value, 4, 1);
            UnkEventValue_06790c arg = {fn_710000abbc(), &value};
            SetField(field, &obj, &arg);
            break;
        }
        case 0x291E3F130F064F1D: {
            s32 value;
            fn_710007c714(file, &value, 4, 1);
            UnkEventValue_06790c arg = {fn_7100014768(), &value};
            SetField(field, &obj, &arg);
            break;
        }
        case 0x6A0C7BDE338B1A2B: {
            bool value;
            fn_710007c714(file, &value, 1, 1);
            UnkEventValue_06790c arg = {fn_710000d228(), &value};
            SetField(field, &obj, &arg);
            break;
        }
        case 0x28230A7FDBF66651: {
            u64 name = ~0ull;
            fn_710007c714(file, &name, 8, 1);
            UnkEventValue_06790c arg = {fn_710000fd7c(), &name};
            SetField(field, &obj, &arg);
            break;
        }
        case 0x0D7B335721099BF9: {
            base::math::CVector3D value = {0.0f, 0.0f, 0.0f};
            fn_710007c714(file, &value, 12, 1);
            UnkEventValue_06790c arg = {fn_71000246e4(), &value};
            SetField(field, &obj, &arg);
            break;
        }
        case 0x7C0E083AF861F5A8: {
            UnkColor3F_06790c value = {1.0f, 1.0f, 1.0f};
            fn_710007c714(file, &value, 12, 1);
            UnkEventValue_06790c arg = {fn_7100067f5c(), &value};
            SetField(field, &obj, &arg);
            break;
        }
        case 0x313DF0831835525F: {
            base::math::CVector4D value = {0.0f, 0.0f, 0.0f, 0.0f};
            fn_710007c714(file, &value, 16, 1);
            UnkEventValue_06790c arg = {fn_71000248c4(), &value};
            SetField(field, &obj, &arg);
            break;
        }
        case 0x4048CBEEC15D3C0E: {
            base::color::CColor4F value(1.0f, 1.0f, 1.0f, 1.0f);
            fn_710007c714(file, &value, 16, 1);
            UnkEventValue_06790c arg = {fn_7100068204(), &value};
            SetField(field, &obj, &arg);
            break;
        }
        case 0xFC925429C459C462: {  // an enum: the member's word is written directly
            u32 value;
            fn_710007c714(file, &value, 4, 1);
            UnkReflTypedPtr member = fn_71000e755c(reinterpret_cast<UnkReflField*>(field), &obj);
            fn_710009dcd8(member.type, member.ptr);
            *static_cast<u32*>(member.ptr) = value;
            break;
        }
        }
    }
}
