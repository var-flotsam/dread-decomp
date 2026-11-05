// CSoundProofTriggerComponent unit (tools/tu_map.py unit 1544): two destructors, copies of
// fn_710091d194 and fn_710091d244 (CHydrogigaZiplineComponent_Dtor_b51.cpp). Each frees an array of
// 0x40-byte entries at +0xac0, then runs the base destructor fn_7100a46b64; fn_7100a2fedc also frees
// the object. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkComponent.h"

struct UnkSoundProofEntry_b52 {
    virtual ~UnkSoundProofEntry_b52();
    u8 pad_0x8[0x38];
};

void fn_7100a46b64(char*);

extern void* g_unk_7101a8e0f8[];

struct UnkSoundProofTriggerComponent_b52 {
    void* vtable;
    u8 pad_0x8[0xab8];
    base::UnkVector<UnkSoundProofEntry_b52> entries;
};

static inline void SoundProofDtor_b52(UnkSoundProofTriggerComponent_b52* self) {
    self->vtable = g_unk_7101a8e0f8 + 2;
    self->entries.Free();
    fn_7100a46b64(reinterpret_cast<char*>(self));
}

void fn_7100a2fe2c(UnkSoundProofTriggerComponent_b52* self) {
    SoundProofDtor_b52(self);
}

void fn_7100a2fedc(UnkSoundProofTriggerComponent_b52* self) {
    SoundProofDtor_b52(self);
    fn_7100083c00(self);
}
