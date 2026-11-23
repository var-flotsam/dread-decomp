// base::global::timeline::CTrack's unit (tools/tu_map.py unit 44): reads a track from a file: its two
// times (tenths of a second), then a count of reflected fields, each given by its name's hash and set
// from a value read as the field's type says. Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::global::CRntFile;
using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

// As src/base/global/UnkTimelineTracks.cpp declares it.
struct UnkTrack;
// As src/game_s7/fn_71000e2800.cpp declares them: the colour types' descriptors.
struct UnkEnumType;
UnkEnumType* fn_7100067f5c();  // "base::color::CColor3F"
UnkEnumType* fn_7100068204();  // "base::color::CColor4F"
// As src/base/reflection/CFlagsetType/CFlagsetType_Read_ovb0068.cpp defines it.
struct UnkFlagsetType_b0068;
bool fn_710009a430(UnkFlagsetType_b0068* self, const UnkReflFieldObject* obj, CVariable* field, CRntFile* file);
// As src/base/reflection/UnkUnit85/UnkFieldPointee.cpp defines it: the member object a field of `owner` holds.
struct UnkReflFieldInfo;
struct UnkFieldOwner;
UnkReflTypedPtr fn_71000e755c(const UnkReflFieldInfo* field, UnkFieldOwner* owner);

// A reflected field (0xb0 bytes): its type at +0x10.
struct UnkTrackField_l06afd4 {
    u8 pad_0x0[0x10];
    const CType* type;  // 0x10
    u8 pad_0x18[0x98];
};
static_assert(sizeof(UnkTrackField_l06afd4) == 0xb0, "UnkTrackField_l06afd4 layout");

// A type descriptor as seen here: its id and, for a class, its fields by name hash.
struct UnkTrackType_l06afd4 {
    virtual const CType* GetClass();
    u8 pad_0x8[0x8];
    s64 id;  // 0x10: the hash of its name
    u8 pad_0x18[0xd0];
    UnkTrackField_l06afd4* fields;  // 0xe8
    u64* keys;                      // 0xf0
    u32 count;                      // 0xf8

    UnkTrackField_l06afd4* End() { return fields + count; }
    UnkTrackField_l06afd4* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
};

struct UnkTrackBody_l06afd4 {
    virtual char* GetClass();
    u8 pad_0x8[0x8];
    f32 field_0x10;  // seconds
    f32 field_0x14;  // seconds
};

// The field calls' argument: a value's type and where it is.
struct UnkTrackValue_l06afd4 {
    const CType* type;
    void* value;
};

static inline UnkTrackType_l06afd4* AsType(const CType* type) {
    return reinterpret_cast<UnkTrackType_l06afd4*>(const_cast<CType*>(type));
}

static inline UnkReflTypedPtr Typed_l06afd4(char* type, void* obj) {
    UnkReflTypedPtr p;
    p.type = fn_710009dcd8(type, obj);
    p.ptr = obj;
    return p;
}

static inline void Set_l06afd4(UnkTrackField_l06afd4* field, UnkReflTypedPtr* obj, UnkTrackValue_l06afd4* value) {
    fn_71000e7a80(reinterpret_cast<CVariable*>(field), reinterpret_cast<const UnkReflFieldObject*>(obj),
                  reinterpret_cast<UnkReflValueArg*>(value));
}

// A type id as the switch compares it.
static constexpr s64 TypeId(u64 hash) {
    return static_cast<s64>(hash);
}

void fn_710006afd4(UnkTrack* track, CRntFile* file) {
    UnkTrackBody_l06afd4* self = reinterpret_cast<UnkTrackBody_l06afd4*>(track);
    u32 start;
    u32 length;
    CRntFile_Read(file, &start, 4, 1);
    CRntFile_Read(file, &length, 4, 1);
    self->field_0x10 = start * 0.1f;
    self->field_0x14 = length * 0.1f;
    UnkReflTypedPtr obj = Typed_l06afd4(self->GetClass(), self);

    s32 count = 0;
    CRntFile_Read(file, &count, 4, 1);
    for (s32 i = 0; i < count; ++i) {
        u64 hash;
        CRntFile_Read(file, &hash, 8, 1);
        UnkTrackType_l06afd4* type = AsType(reinterpret_cast<const CType*>(obj.type));
        UnkTrackType_l06afd4* cls =
            type && fn_710009d50c(type->GetClass(), fn_7100096100()) ? type : nullptr;
        UnkTrackField_l06afd4* field = cls->Find(hash);
        if (field == cls->End() || !field)
            continue;

        const CType* field_type = field->type;
        switch (AsType(field_type)->id) {
        case TypeId(0x3b98086a3ed2623d):
        case TypeId(0xf6d656c06a983519):
        case TypeId(0xb97cd894d76cacd6): {
            u32 value;
            CRntFile_Read(file, &value, 4, 1);
            UnkTrackValue_l06afd4 arg = {fn_7100005178(), &value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        case TypeId(0x28230a7fdbf66651): {
            u64 value = -1;
            CRntFile_Read(file, &value, 8, 1);
            UnkTrackValue_l06afd4 arg = {fn_710000fd7c(), &value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        case TypeId(0xbf34f79bba0deaf6):
        case TypeId(0x8c2deec131321516): {
            u16 size;
            char text[0x140];
            CRntFile_Read(file, &size, 2, 1);
            if (size)
                CRntFile_Read(file, text, size, 1);
            text[size] = '\0';
            const char* value = text;
            UnkTrackValue_l06afd4 arg = {fn_71000081e8(), &value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        case TypeId(0xc7058cebee667049): {
            UnkTrackType_l06afd4* flags = AsType(field_type);
            if (flags && !fn_710009d50c(flags->GetClass(), fn_710009a11c()))
                flags = nullptr;
            fn_710009a430(reinterpret_cast<UnkFlagsetType_b0068*>(flags), reinterpret_cast<const UnkReflFieldObject*>(&obj),
                          reinterpret_cast<CVariable*>(field), file);
            break;
        }
        case TypeId(0x4048cbeec15d3c0e): {
            f32 value[4] = {1.0f, 1.0f, 1.0f, 1.0f};
            CRntFile_Read(file, value, 0x10, 1);
            UnkTrackValue_l06afd4 arg = {reinterpret_cast<const CType*>(fn_7100068204()), value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        case TypeId(0x937459ba5ed68a51): {
            f32 value;
            CRntFile_Read(file, &value, 4, 1);
            UnkTrackValue_l06afd4 arg = {fn_710000abbc(), &value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        case TypeId(0x291e3f130f064f1d): {
            s32 value;
            CRntFile_Read(file, &value, 4, 1);
            UnkTrackValue_l06afd4 arg = {fn_7100014768(), &value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        case TypeId(0x313df0831835525f): {
            f32 value[4] = {};
            CRntFile_Read(file, value, 0x10, 1);
            UnkTrackValue_l06afd4 arg = {fn_71000248c4(), value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        case TypeId(0xfc925429c459c462): {
            // An enum: its value is stored straight into the member.
            u32 value;
            CRntFile_Read(file, &value, 4, 1);
            UnkReflTypedPtr member = fn_71000e755c(reinterpret_cast<const UnkReflFieldInfo*>(field),
                                                   reinterpret_cast<UnkFieldOwner*>(&obj));
            *static_cast<u32*>(Typed_l06afd4(member.type, member.ptr).ptr) = value;
            break;
        }
        case TypeId(0x7c0e083af861f5a8): {
            f32 value[3] = {1.0f, 1.0f, 1.0f};
            CRntFile_Read(file, value, 0xc, 1);
            UnkTrackValue_l06afd4 arg = {reinterpret_cast<const CType*>(fn_7100067f5c()), value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        case TypeId(0x6a0c7bde338b1a2b): {
            bool value;
            CRntFile_Read(file, &value, 1, 1);
            UnkTrackValue_l06afd4 arg = {fn_710000d228(), &value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        case TypeId(0x0d7b335721099bf9): {
            f32 value[3] = {};
            CRntFile_Read(file, value, 0xc, 1);
            UnkTrackValue_l06afd4 arg = {fn_71000246e4(), value};
            Set_l06afd4(field, &obj, &arg);
            break;
        }
        }
    }
}
