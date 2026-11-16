// Unit 112 (the tunables' registry): registers the tunables' descriptor (when the registry of
// descriptors exists), marks every tunable (reading the second tunables file too when asked), then
// reads the tunables file when asked. Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU112.h"
#include "game/UnkTU27.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// As src/game/UnkTU112/UnkTU112_Tunables_ovb0235.cpp defines them.
struct UnkTunableEntry_b0235 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    u8 pad_0x8[0x18];
    u8 field_0x20;
};

struct UnkTunableObjects_b0235 {
    UnkTunableEntry_b0235** objects;
    u8 pad_0x8[8];
    u32 count;  // 0x10
};

void fn_710013af08(UnkTunableObjects_b0235* self);
bool fn_710013b0e0(UnkTunableRegistry* self, bool generate);

struct UnkDesc_7100d92158;
extern UnkDesc_7100d92158 g_unk_710b372808 __attribute__((visibility("hidden")));  // the tunables' descriptor

// A registry entry (UnkEntry_7100d92158 in src/game_s7/fn_7100d92158.cpp), built inline with the
// default id, the default path and the two default callbacks.
struct UnkRegistration_l13ac6c {
    UnkDesc_7100d92158* desc;
    void* factory;
    CStrId id;
    CFilePathStrId path;
    CFilePathStrId path_0x20;
    void* field_0x28;
    void (*callback_0x30)(void* context, void* arg);
    void* field_0x38;
    void (*callback_0x40)(void* context, void* arg);

    explicit UnkRegistration_l13ac6c(UnkDesc_7100d92158* desc)
        : desc(desc), factory(nullptr), id(g_unk_7101d470e0), path(g_unk_7101d470e8), field_0x28(nullptr),
          callback_0x30(fn_71000204c4), field_0x38(nullptr), callback_0x40(fn_71000204cc) {}
};

void fn_710013ac6c(UnkTunableObjects_b0235* self, bool read_file, UnkNoArg, bool read_second_file) {
    if (UnkRegistry_7100d92158* registry = g_unk_7101d4bbc0) {
        UnkRegistration_l13ac6c entry(&g_unk_710b372808);
        fn_710002002c(registry, reinterpret_cast<const UnkEntry_7100d92158&>(entry));
    }
    if (read_second_file) {
        fn_710013af08(self);
    } else if (self->count) {
        for (UnkTunableEntry_b0235** it = self->objects; it != self->objects + self->count;) {
            (*it)->vf_0x18();
            (*it++)->field_0x20 = 1;
        }
    }
    if (read_file)
        fn_710013b0e0(reinterpret_cast<UnkTunableRegistry*>(self), false);
}
