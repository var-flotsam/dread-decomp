// The rumble manager's unit (tools/tu_map.py unit 51, base::input::CRumbleManager::CTunableRumbleManager):
// plays a rumble. While the manager is enabled and has a free effect, the rumble file named by the params
// is loaded (or found in the cache), a new effect is set up from the params with that file and started,
// and a handle to its slot is returned; otherwise the null handle. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "game/UnkTU50.h"
#include "game/operator_new.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::input::UnkRumbleManager;

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
