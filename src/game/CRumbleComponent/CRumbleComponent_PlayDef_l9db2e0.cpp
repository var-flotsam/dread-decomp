// CRumbleComponent's unit (tools/tu_map.py): plays a rumble from a rumble definition, building the
// rumble's parameters (its file, kind and tunables) and starting it with fn_71009dacbc.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CRumbleComponent.h"
#include "game/UnkHandle.h"
#include "game/UnkHandleTarget.h"
#include "game/UnkTU49.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::math::CVector3D;

// Three floats assigned one by one (a defaulted copy constructor).
struct UnkRumbleOffset_l9db2e0 {
    f32 x, y, z;
    UnkRumbleOffset_l9db2e0(const CVector3D& v) : x(v.x), y(v.y), z(v.z) {}
    UnkRumbleOffset_l9db2e0& operator=(const UnkRumbleOffset_l9db2e0& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

struct CRumbleComponent;
struct UnkRumbleParams;

// A file-path id (pool g_unk_7101d47200) whose count is a plain int.
struct UnkRumblePath_l9db2e0 {
    UnkStringData* data;

    explicit UnkRumblePath_l9db2e0(const base::global::UnkFilePathStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkRumblePath_l9db2e0& operator=(const UnkRumblePath_l9db2e0& other) {
        if (other.data != data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkRumblePath_l9db2e0() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// What a rumble is played from (UnkRumbleParams of src/game/CRumbleComponent.cpp).
struct UnkRumbleDesc_l9db2e0 {
    UnkRumblePath_l9db2e0 path;
    u64 field_0x8;
    u32 kind;  // 0x10
    UnkRumbleOffset_l9db2e0 offset;  // 0x14
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkRumbleDesc_l9db2e0()
        : path(g_unk_7101d470e8), field_0x8(0), kind(0x7fffffff), offset(g_unk_710164779c),
          field_0x20{0.0f, 0.0f, 1.0f, 1.0f}, field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};

// A rumble definition.
struct UnkRumbleDef_l9db2e0 {
    u32 kind;
    u8 pad_0x4[0x1c];
    UnkRumblePath_l9db2e0 path;  // 0x20
    UnkRumbleOffset_l9db2e0 offset;  // 0x28
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    u8 pad_0x54[8];
    f32 field_0x5c;
    u8 pad_0x60[0xc];
    bool field_0x6c;
    bool field_0x6d;
};

// Plays the definition's rumble; the two flags of fn_71009dacbc both take `flag`.
UnkHandle fn_71009db2e0(CRumbleComponent* self, void* a, bool b, bool c, bool d) {
    const UnkRumbleDef_l9db2e0* def = static_cast<const UnkRumbleDef_l9db2e0*>(a);
    UnkRumbleDesc_l9db2e0 desc;
    desc.path = def->path;
    desc.field_0x8 = 0;
    desc.kind = fn_7100071680(def->kind);
    desc.offset = def->offset;
    desc.field_0x20[0] = def->field_0x40;
    desc.field_0x20[1] = def->field_0x3c;
    desc.field_0x20[2] = def->field_0x5c > 0.0f ? def->field_0x5c : def->field_0x38;
    desc.field_0x20[3] = def->field_0x44;
    desc.field_0x30 = def->field_0x48 * 0.5f + 0.5f;
    desc.field_0x34 = def->field_0x4c;
    desc.field_0x38 = def->field_0x50;
    desc.field_0x3c = 1.0f;
    desc.field_0x40 = def->field_0x6d;
    desc.field_0x41 = def->field_0x6c;
    // fn_71009dacbc is given a third flag it does not read (src/game/CRumbleComponent.cpp).
    using Fn = UnkHandle (*)(CRumbleComponent*, const UnkRumbleParams*, bool, bool, bool);
    return reinterpret_cast<Fn>(&fn_71009dacbc)(self, reinterpret_cast<const UnkRumbleParams*>(&desc), b, b, false);
}
