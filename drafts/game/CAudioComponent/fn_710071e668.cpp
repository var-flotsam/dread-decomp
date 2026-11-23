// CAudioComponent unit (tools/tu_map.py): updates one of the component's sound entries for a frame: keeps a
// playing sound in range, visible and at its level, or counts down to playing it again, then links the
// playing sound to the preset record that made it. Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU101.h"
#include "game/UnkTU104.h"
#include "game/UnkTU23.h"
#include "game/UnkTU315.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkSoundObject_b50 {
    u8 pad_0x0[0x124];
    bool field_0x124;
};

struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

// What this file reads of a playing sound.
struct UnkSound_l71e668 {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0x6c - 0x48];
    s32 level;  // 0x6c
    u8 pad_0x70[0x88 - 0x70];
    void* preset_key;  // 0x88
    u8 pad_0x90[0x98 - 0x90];
    f32 x, y, z;  // 0x98
    u8 pad_0xa4[0xe4 - 0xa4];
    f32 field_0xe4;
    f32 field_0xe8;
    f32 field_0xec;
    u8 pad_0xf0[0xfc - 0xf0];
    f32 field_0xfc;
    u8 pad_0x100[0x108 - 0x100];
    f32 field_0x108;
};

// The sound a handle names while it is still the one the handle was made for.
inline UnkSound_l71e668* GetSound_l71e668(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return reinterpret_cast<UnkSound_l71e668*>(handle->ref->object);
    return nullptr;
}

// A request to play a sound, with its defaults.
struct UnkSoundRequest_l71e668 {
    s32 field_0x0 = 0;
    u32 field_0x4 = 0;
    u64 field_0x8 = 0;
    u32 field_0x10 = 0;
    CStrId id = g_unk_7101d470e0;
    CFilePathStrId path = g_unk_7101d470e8;
    u64 field_0x28 = 0;
    f32 field_0x30 = 0.0f;
    f32 field_0x34 = -1.0f;
    f32 volume = 1.0f;  // 0x38
    f32 field_0x3c = 500.0f;
    f32 field_0x40 = 2000.0f;
    f32 field_0x44 = 1.0f;
    u64 field_0x48 = 0;
    f32 field_0x50 = 0.0f;
    f32 field_0x54 = -1.0f;
    f32 field_0x58 = -1.0f;
    f32 field_0x5c = -1.0f;
    f32 field_0x60 = 0.5f;
    f32 field_0x64 = 5.0f;
    f32 field_0x68 = 10.0f;
    bool loop = false;        // 0x6c
    bool field_0x6d = false;
    bool field_0x6e = false;
};
static_assert(sizeof(UnkSoundRequest_l71e668) == 0x70, "UnkSoundRequest_l71e668 layout");

struct UnkSoundEntry_l71e668 {
    u8 pad_0x0[0x8];
    s32 value_index;   // 0x8
    s32 volume_index;  // 0xc
    u32 group;         // 0x10
    u8 pad_0x14[0x28 - 0x14];
    CFilePathStrId path;  // 0x28
    f32 x, y, z;          // 0x30
    u8 pad_0x3c[0x40 - 0x3c];
    f32 field_0x40;
    f32 field_0x44;
    f32 radius;  // 0x48
    f32 field_0x4c;
    u8 pad_0x50[0x54 - 0x50];
    f32 visible_value;  // 0x54
    f32 hidden_value;   // 0x58
    u8 pad_0x5c[0x68 - 0x5c];
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    u8 pad_0x74;
    bool limit_range;  // 0x75
    u8 pad_0x76[0x78 - 0x76];
    UnkSoundHandle handle;  // 0x78
    u8 pad_0x88[0x98 - 0x88];
    UnkStrIdPathPair ids;  // 0x98
    u8 pad_0xa8[0xb0 - 0xa8];
    f32 interval;          // 0xb0
    f32 timer;             // 0xb4
    f32 range_x;           // 0xb8
    f32 range_y;           // 0xbc
    bool stopped;          // 0xc0
    u8 pad_0xc1[3];
    bool loop;      // 0xc4
    u8 pad_0xc5[3];
    f32 volume;     // 0xc8
};
static_assert(__builtin_offsetof(UnkSoundEntry_l71e668, ids) == 0x98, "layout");
static_assert(__builtin_offsetof(UnkSoundEntry_l71e668, volume) == 0xc8, "layout");

struct UnkPreset_l71e668 : UnkReflObject {
    u8 pad_0x8[0x58 - 0x8];
    void* key;  // 0x58
};

struct UnkPresetRecord_l71e668 {
    UnkSoundHandle handle;
    u8 pad_0x10[0x90 - 0x10];
    UnkStrIdPathPair ids;       // 0x90
    UnkReflObject* preset;      // 0xa0
    u8 pad_0xa8[0xb0 - 0xa8];
};
static_assert(sizeof(UnkPresetRecord_l71e668) == 0xb0, "UnkPresetRecord_l71e668 layout");

struct UnkAudio_l71e668 {
    void* vtable;
    u8 pad_0x8[0x30 - 0x8];
    void* owner;  // 0x30
    u8 pad_0x38[0x40 - 0x38];
    u8 field_0x40[0x30];
    UnkPresetRecord_l71e668* records;  // 0x70
    u32 record_count;                  // 0x78
    u8 pad_0x7c[0xc0 - 0x7c];
    bool fade_far;      // 0xc0
    bool check_view;    // 0xc1
    u8 pad_0xc2[2];
    f32 x, y, z;        // 0xc4
    u8 pad_0xd0[0xec - 0xd0];
    f32 volumes[11];    // 0xec
    s32 values[16];     // 0x118

    bool CanPlay() { return (*reinterpret_cast<bool (***)(UnkAudio_l71e668*)>(this))[0x178 / 8](this); }
};

struct UnkAudio_b50;
struct UnkSoundRequest_b50;
struct UnkSoundRequest_b0038;
UnkSoundHandle fn_710071a7dc(UnkAudio_b50* self, const UnkSoundRequest_b50* request);
UnkSoundHandle fn_710102540c(UnkDropTarget* self, const UnkStrIdPathPair* ids, const UnkSoundRequest_b0038* request);
void fn_710071b39c(UnkSoundRequest_l71e668* request, const void* params);
void fn_71001060e8(UnkSound_l71e668* sound, f32 a, f32 b, f32 c);
s32 fn_7100108c24(UnkSoundMgr* self, u32 group);

inline UnkGameManagerIdTable& GameIds_l71e668() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

inline const CRntString* PathString_l71e668(const CFilePathStrId& path) {
    ++path.data->reads;
    return reinterpret_cast<const CRntString*>(reinterpret_cast<const char*>(path.data) + 0x10);
}

inline f32 DistanceSq_l71e668(const UnkAudio_l71e668* self, const UnkSoundEntry_l71e668* entry) {
    f32 dx = entry->x - self->x;
    f32 dy = entry->y - self->y;
    f32 dz = entry->z - self->z;
    return dx * dx + dy * dy + dz * dz;
}

// Plays the entry's sound, by its ids when it has both, else by its request.
static void Play_l71e668(UnkAudio_l71e668* self, UnkSoundEntry_l71e668* entry, bool flag) {
    if (!CRntString_KeysDiffer(PathString_l71e668(entry->path), PathString_l71e668(GameIds_l71e668().field_0x5f8)))
        return;
    UnkSoundRequest_l71e668 request;
    fn_710071b39c(&request, &entry->value_index);
    request.path = entry->path;
    request.field_0x6d = flag;
    request.volume = entry->volume * self->volumes[entry->volume_index];
    request.field_0x10 = self->values[request.field_0x0];
    request.loop = entry->loop;
    if (entry->ids.id.data == &g_unk_7101d47108->empty || entry->ids.path.data == &g_unk_7101d47200->empty) {
        entry->handle = fn_710071a7dc(reinterpret_cast<UnkAudio_b50*>(self),
                                      reinterpret_cast<const UnkSoundRequest_b50*>(&request));
    } else if (!self->CanPlay()) {
        entry->handle = g_unk_7108b6c460;
    } else {
        UnkSoundHandle handle = fn_710102540c(g_unk_7101d47318, &entry->ids,
                                              reinterpret_cast<const UnkSoundRequest_b0038*>(&request));
        if (handle.ref && handle.serial != -1 && handle.ref->serial == handle.serial) {
            UnkSoundObject_b50* object = handle.ref->object;
            if (object && object->field_0x124)
                fn_7100106024(object, self->field_0x40);
        }
        entry->handle = handle;
    }
}

void fn_710071e668(UnkAudio_l71e668* self, UnkSoundEntry_l71e668* entry, f32 dt) {
    if (UnkSound_l71e668* sound = GetSound_l71e668(&entry->handle)) {
        if (self->fade_far && entry->field_0x6c > 0 && sound->field_0x108 > entry->field_0x6c)
            fn_71001060e8(sound, entry->field_0x68, entry->field_0x70, 1.0f);
        bool in_range = true;
        if (entry->limit_range && entry->radius > entry->field_0x44) {
            f32 r = entry->radius;
            if (!self->owner || __builtin_fabsf(self->x - entry->x) > r * entry->range_x ||
                __builtin_fabsf(self->y - entry->y) > r * entry->range_y || DistanceSq_l71e668(self, entry) > r * r)
                in_range = false;
        }
        if (!in_range) {
            // Out of range: keep where the sound got to and stop it.
            if (UnkSound_l71e668* s = GetSound_l71e668(&entry->handle)) {
                entry->field_0x4c = s->field_0x44;
                entry->field_0x40 = s->field_0xec;
                entry->visible_value = s->field_0xfc;
                entry->x = s->x;
                entry->y = s->y;
                entry->z = s->z;
                UnkSound_l71e668* again = reinterpret_cast<UnkSound_l71e668*>(entry->handle.ref->object);
                entry->field_0x44 = again->field_0xe8;
                entry->radius = again->field_0xe4;
                fn_7100105034(s);
                entry->handle = g_unk_7108b6c460;
                entry->stopped = false;
            }
            return;
        }
        if (!self->check_view) {
            s32 value = self->values[entry->value_index];
            s32 level = fn_710010aaa0(fn_7100108c24(g_unk_7101d4d820, entry->group));
            if (value != 0 && level == 16000) {
                UnkSound_l71e668* s = reinterpret_cast<UnkSound_l71e668*>(entry->handle.ref->object);
                if (s->level > fn_710010aaa0(value))
                    fn_7100105f94(reinterpret_cast<UnkInterp_7100105f94*>(GetSound_l71e668(&entry->handle)), value, 0.2f);
            }
            return;
        }
        bool visible = fn_7100219588(g_unk_7101d47300, &entry->x, 0, 0);
        UnkPlayingSound_ovb0074* s = reinterpret_cast<UnkPlayingSound_ovb0074*>(GetSound_l71e668(&entry->handle));
        if (visible)
            fn_7100105cf0(s, entry->visible_value);
        else
            fn_7100105c9c(s, entry->hidden_value);
        return;
    }

    f32 distance_sq = DistanceSq_l71e668(self, entry);
    bool play = false;
    bool near = false;
    if (entry->loop) {
        if (entry->field_0x40 > 0.0f && !entry->stopped) {
            if (entry->radius < entry->field_0x44) {
                entry->timer -= entry->interval;
                play = true;
            } else if (distance_sq < entry->radius * entry->radius) {
                play = true;
            }
        }
    } else if (entry->interval > 0.0f && !entry->stopped) {
        entry->timer += dt;
        if (entry->timer >= entry->interval) {
            if (entry->radius < entry->field_0x44) {
                entry->timer -= entry->interval;
                play = true;
            } else if (!(distance_sq > entry->radius * entry->radius)) {
                entry->timer -= entry->interval;
                play = true;
                near = true;
            }
        }
    }
    if (play)
        Play_l71e668(self, entry, near);

    // Links the playing sound to the preset record whose preset made it.
    UnkSound_l71e668* sound = GetSound_l71e668(&entry->handle);
    if (!sound)
        return;
    for (UnkPresetRecord_l71e668 *r = self->records, *e = r + self->record_count; r != e; ++r) {
        if (r->ids.id.data == &g_unk_7101d47108->empty || r->ids.path.data == &g_unk_7101d47200->empty)
            continue;
        UnkReflObject* preset = r->preset;
        if (!preset || !fn_710009d50c(preset->vf_0x0(), fn_710102df18()))
            continue;
        if (static_cast<UnkPreset_l71e668*>(preset)->key !=
            reinterpret_cast<UnkSound_l71e668*>(entry->handle.ref->object)->preset_key)
            continue;
        r->handle = entry->handle;
        return;
    }
}
