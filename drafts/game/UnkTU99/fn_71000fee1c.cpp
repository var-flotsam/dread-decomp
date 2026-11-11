// Unit 99 (sound system): appends to a result list the slot of every active voice with the given id.
// Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "types.h"

extern u8 g_unk_7108b6ab60 __attribute__((visibility("hidden")));
bool fn_7100132cd0();
u64 fn_7100083854(void* pool, void* voice);

struct UnkVoiceInfo {
    u8 pad_0x0[0xb0];
    s32 id;
    u8 pad_0xb4[0x64];
    u8 active;
};

struct UnkVoiceSlot {
    void* field_0x0;
    void* field_0x8;
};

struct UnkVoiceResult {
    u32 count;
    UnkVoiceSlot slots[1];
};

struct UnkVoiceSystem {
    u8 pad_0x0[0x398];
    u8 pool[0x28];
    UnkVoiceSlot* slots;  // 0x3c0
    u8 pad_0x3c8[0x50];
    UnkVoiceInfo** voices;  // 0x418
    u32 voice_count;        // 0x420
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;
};

void fn_71000fee1c(UnkVoiceSystem* self, s32 id, UnkVoiceResult* out) {
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    UnkVoiceSlot* base = out->slots;
    asm("" : "+r"(base));
    UnkVoiceInfo** it = self->voices;
    UnkVoiceInfo** end = it + self->voice_count;
    for (; it != end; ++it) {
        UnkVoiceInfo* voice = *it;
        if (voice->id != id || !voice->active)
            continue;
        u32 index = fn_7100083854(self->pool, voice);
        UnkVoiceSlot* slot = &self->slots[index];
        UnkVoiceSlot* dst = &base[out->count];
        dst->field_0x0 = slot;
        dst->field_0x8 = slot->field_0x8;
        out->count++;
    }
    fn_7100066920(&self->mutex);
}
