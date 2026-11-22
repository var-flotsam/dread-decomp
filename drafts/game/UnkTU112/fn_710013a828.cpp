// The tunables' registry unit (tools/tu_map.py unit 112): a tunable's reflected value as text, by its type
// code: "true"/"false", a float with its trailing zeros dropped, an integer, a string or "V3D(x,y,z)".
// Names and layouts are placeholders.
#include <stdio.h>
#include <string.h>

#include "base/UnkTunableLine.h"
#include "base/core/CBaseObject.h"
#include "base/global/CRntString.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTunableReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkEnumType;
UnkEnumType* fn_710009ffd4();  // base::global::TRntString256's type

// Whether the value is of the type `get_type` returns (or derives from it).
template <class TypeGetter>
static inline bool IsKindOf(const UnkReflValue* value, TypeGetter get_type) {
    const CType* own = reinterpret_cast<const CType*>(value->type);
    return own && fn_710009d50c(own, reinterpret_cast<const CType*>(get_type()));
}

// Where the value is kept: inline when it fits in 8 bytes, else through field_0x0.
template <class T>
static inline const T* DataOf(const UnkReflValue* value) {
    if (!value->type)
        return nullptr;
    if (value->field_0x14 == 1 || *reinterpret_cast<const u64*>(value->type + 0x18) > 8)
        return static_cast<const T*>(value->field_0x0);
    return reinterpret_cast<const T*>(value);
}

UnkTunableLine fn_710013a828(const UnkTunableReflValue* value) {
    UnkTunableLine line;
    char buffer[0x40];
    const char* text = buffer;
    switch (fn_7100024140(reinterpret_cast<const CType*>(value->type))) {
    case 'b':
        // a bool, tested as a byte
        if (IsKindOf(value, fn_710000d228) && *DataOf<u8>(value) != 0)
            text = "true";
        else
            text = "false";
        break;
    case 'f': {
        f32 v = 0.0f;
        if (IsKindOf(value, fn_710000abbc))
            v = *DataOf<f32>(value);
        snprintf(buffer, sizeof(buffer), "%.5f", (f64)v);
        // Prints again with only as many decimals as are not trailing zeros (at least one).
        s32 length = strlen(buffer);
        for (s64 last = (s64)length - 1; last >= 0; --last) {
            if (buffer[last] != '0') {
                s32 decimals = last - (strchr(buffer, '.') - buffer);
                snprintf(buffer, sizeof(buffer), "%.*f", decimals > 1 ? decimals : 1, (f64)v);
                break;
            }
        }
        break;
    }
    case 'i': {
        s32 v = 0;
        if (IsKindOf(value, fn_7100014768))
            v = *DataOf<s32>(value);
        snprintf(buffer, sizeof(buffer), "%i", v);
        break;
    }
    case 's': {
        UnkTunableLine string;
        if (IsKindOf(value, fn_710009ffd4))
            CRntString_Assign(string.Str(), DataOf<base::global::CRntString>(value));
        line = string;
        return line;
    }
    case 'v': {
        base::math::CVector3D v = {0.0f, 0.0f, 0.0f};
        if (IsKindOf(value, fn_71000246e4))
            v = *DataOf<base::math::CVector3D>(value);
        snprintf(buffer, sizeof(buffer), "V3D(%.3f,%.3f,%.3f)", (f64)v.x, (f64)v.y, (f64)v.z);
        break;
    }
    default:
        return line;
    }
    CRntString_AssignCString(line.Str(), text);
    return line;
}
