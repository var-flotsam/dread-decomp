// The rumble manager's unit (tools/tu_map.py unit 51, base::input::CRumbleManager::CTunableRumbleManager):
// plays a rumble. While the manager is enabled and has a free effect, the rumble file named by the params
// is loaded (or found in the cache), a new effect is set up from the params with that file and started,
// and a handle to its slot is returned; otherwise the null handle. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkTU49.h"
#include "game/UnkTU50.h"
#include "game/operator_new.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::input::UnkRumbleManager;
using base::math::CVector3D;

// A handle to a playing rumble: current while the target's serial is the handle's.
struct UnkHandleTarget {
    void* field_0x0;
    u64 field_0x8;
};

struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

struct UnkRumbleParams;
struct UnkRumbleFile;
struct UnkTunableRumbleManager_b0068;
struct UnkRumbleEffect_b0068;
struct UnkUnit50Object;
struct UnkUnit50Desc;

UnkRumbleFile* fn_710007454c(UnkRumbleManager* self, const void* name);
UnkRumbleEffect_b0068* fn_7100074b38(UnkTunableRumbleManager_b0068* self);
void fn_7100071ef0(UnkUnit50Object* self, const UnkUnit50Desc* desc);
void fn_71000722c4(UnkUnit50Object* self);
// The slot vector's GrowAt (base::global::CRntVector<..>::GrowAt, instantiated in src/clones as
// UnkClone_fn_7100075b9c).
UnkHandleTarget* fn_7100075b9c(void* slots, u32 index);

// A loaded rumble file as this function reads it: its path.
struct UnkRumbleFile_l074354 {
    u8 pad_0x0[0x18];
    CFilePathStrId path;  // 0x18
};

// Three words and one word with user-written copies; the fields around them are copied as blocks.
struct UnkRumbleWords3_l074354 {
    u32 x, y, z;
    UnkRumbleWords3_l074354(const UnkRumbleWords3_l074354& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkRumbleWord_l074354 {
    u64 value;
    UnkRumbleWord_l074354(const UnkRumbleWord_l074354& o) : value(o.value) {}
};

// What a rumble is played from (0x48 bytes).
struct UnkRumbleParams_l074354 {
    CFilePathStrId path;
    UnkRumbleFile_l074354* file;  // 0x8
    u32 field_0x10;
    UnkRumbleWords3_l074354 field_0x14;
    UnkRumbleWord_l074354 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    u16 field_0x40;
};
static_assert(sizeof(UnkRumbleParams_l074354) == 0x48, "layout");

struct UnkTunableRumbleManager_l074354 {
    u8 pad_0x0[0x2190];
    u8 effect_blocks[0x28];  // 0x2190: the effects' pool
    u8 slots[0x40];          // 0x21b8: a vector of 16-byte slots (UnkHandleTarget)
    u64 free_effects;        // 0x21f8
    u8 pad_0x2200[9];
    bool enabled;            // 0x2209
};
static_assert(__builtin_offsetof(UnkTunableRumbleManager_l074354, enabled) == 0x2209, "layout");

UnkHandle fn_7100074354(UnkRumbleManager* manager, const UnkRumbleParams* desc) {
    auto* self = reinterpret_cast<UnkTunableRumbleManager_l074354*>(manager);
    auto* params = reinterpret_cast<const UnkRumbleParams_l074354*>(desc);
    if (!self->enabled || !self->free_effects)
        return g_unk_7101d586c0;
    // The path's text, counting the read (UnkStrIdBase::Text on the file-path id).
    ++params->path.data->reads;
    const char* name = params->path.data->text;
    auto* file = reinterpret_cast<UnkRumbleFile_l074354*>(fn_710007454c(manager, name ? name : g_unk_710162e2ed));
    if (!file)
        return g_unk_7101d586c0;
    UnkRumbleParams_l074354 local(*params);
    local.file = file;
    local.path = file->path;
    UnkHandle handle;
    auto* effect = fn_7100074b38(reinterpret_cast<UnkTunableRumbleManager_b0068*>(self));
    if (effect) {
        auto* object = reinterpret_cast<UnkUnit50Object*>(effect);
        fn_7100071ef0(object, reinterpret_cast<const UnkUnit50Desc*>(&local));
        fn_71000722c4(object);
        u64 index = fn_7100083854(reinterpret_cast<UnkVoicePool_b56*>(self->effect_blocks),
                                  reinterpret_cast<UnkSoundVoice2_b56*>(effect));
        UnkHandleTarget* slot = fn_7100075b9c(self->slots, index);
        handle.target = slot;
        handle.serial = slot->field_0x8;
    } else {
        handle = g_unk_7101d586c0;
    }
    return handle;
}

// fn_7100074c60 is defined here, next to fn_7100074354: in one file the compiler treats the params as
// read-only across that call, as the original does.
struct UnkRumbleOffset_l074c60 {
    f32 x, y, z;
    UnkRumbleOffset_l074c60(const CVector3D& v) : x(v.x), y(v.y), z(v.z) {}
    UnkRumbleOffset_l074c60& operator=(const UnkRumbleOffset_l074c60& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A file-path id (pool g_unk_7101d47200) whose count is a plain int.
struct UnkRumblePath_l074c60 {
    UnkStringData* data;

    explicit UnkRumblePath_l074c60(const base::global::UnkFilePathStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkRumblePath_l074c60& operator=(const UnkRumblePath_l074c60& other) {
        if (other.data != data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkRumblePath_l074c60() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// What a rumble is played from (UnkRumbleParams).
struct UnkRumbleDesc_l074c60 {
    UnkRumblePath_l074c60 path;
    u64 field_0x8;
    u32 kind;                        // 0x10
    UnkRumbleOffset_l074c60 offset;  // 0x14
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkRumbleDesc_l074c60()
        : path(g_unk_7101d470e8), field_0x8(0), kind(0x7fffffff), offset(g_unk_710164779c),
          field_0x20{0.0f, 0.0f, 1.0f, 1.0f}, field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};

// A tunable rumble: its kind (0x7fffffff for the default kind at +0x8), file and tunables.
struct UnkTunableRumble_l074c60 {
    u32 kind;
    u8 pad_0x4[0x4];
    u32 default_kind;  // 0x8
    u8 pad_0xc[0x14];
    UnkRumblePath_l074c60 path;      // 0x20
    UnkRumbleOffset_l074c60 offset;  // 0x28
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    u8 pad_0x54[0x18];
    bool field_0x6c;
    bool field_0x6d;
};

UnkHandle fn_7100074c60(UnkRumbleManager* self, const UnkTunableRumble_l074c60* rumble) {
    UnkRumbleDesc_l074c60 desc;
    desc.path = rumble->path;
    desc.field_0x8 = 0;
    u32 kind = rumble->kind;
    if (kind == 0x7fffffff)
        kind = rumble->default_kind;
    desc.kind = fn_7100071680(kind);
    desc.offset = rumble->offset;
    desc.field_0x20[0] = rumble->field_0x40;
    desc.field_0x20[1] = rumble->field_0x3c;
    desc.field_0x20[2] = rumble->field_0x38;
    desc.field_0x20[3] = rumble->field_0x44;
    desc.field_0x30 = rumble->field_0x48 * 0.5f + 0.5f;
    desc.field_0x34 = rumble->field_0x4c;
    desc.field_0x38 = rumble->field_0x50;
    desc.field_0x3c = 1.0f;
    desc.field_0x40 = rumble->field_0x6d;
    desc.field_0x41 = rumble->field_0x6c;
    return fn_7100074354(self, reinterpret_cast<const UnkRumbleParams*>(&desc));
}
