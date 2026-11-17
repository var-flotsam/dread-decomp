// CHyperBeamGun's unit (tools/tu_map.py unit 1305): fn_7100846ec0 updates the beam's recoil track. While the
// recoil track (id at +0x1918) plays past 3.0 it is sent mode 2; when it stops, a fire-action track starts a
// new one (mode 1), and with none the gun's cooldown (+0x190c) counts down by `dt`. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkHyperBeamCooldown_l846ec0 {
    u8 pad_0x0[0x12a];
    bool field_0x12a;
    u8 pad_0x12b;
    f32 field_0x12c;
};

struct UnkHyperBeamOwner_l846ec0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a8 - 0x228];
    UnkHyperBeamCooldown_l846ec0* field_0x2a8;
};

struct UnkHyperBeamGun_l846ec0 {
    u8 pad_0x0[0x18];
    UnkHyperBeamOwner_l846ec0* owner;  // 0x18
    u8 pad_0x20[0x190c - 0x20];
    f32 cooldown;  // 0x190c
    u8 pad_0x1910[0x1918 - 0x1910];
    s64 recoil_id;  // 0x1918
};
static_assert(__builtin_offsetof(UnkHyperBeamGun_l846ec0, recoil_id) == 0x1918, "layout");

struct UnkRecoilDesc_l846ec0 {
    u64 words[2];
};

struct UnkRecoilBlend_l846ec0 {
    u64 kind;
    u32 value;
};

// A local blend descriptor: a null pointer and a float.
struct UnkRecoilParams_l846ec0 {
    void* ptr;
    f32 value;
};

struct UnkRecoilRequest_l846ec0 {
    s64 id;
    u64 desc[2];      // 0x8
    u64 blend_kind;   // 0x18
    u32 blend_value;  // 0x20
    u32 flags;        // 0x24
    u32 mask;         // 0x28
};

// The fire-action track that fills the request in.
struct UnkRecoilTrack_l846ec0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkRecoilRequest_l846ec0* request);
};

static constexpr u64 kEmptyKey_l846ec0 = 0xf4061ff25a2163d6ull;

void fn_7100846ec0(UnkHyperBeamGun_l846ec0* self, f32 dt) {
    // The recoil request's mode and mask live through the whole function (they get slots of their own).
    base::reflection::UnkReflTypeIdArg recoil_mode;
    base::reflection::UnkReflTypeIdArg recoil_mask;
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation) {
        if (self->recoil_id != -1) {
            if (fn_710117ae38(animation, self->recoil_id, false) && fn_710117b348(animation, fn_7100c25008())) {
                if (fn_7101177f58(animation, kEmptyKey_l846ec0) > 3.0f) {
                    s64 id = self->recoil_id;
                    recoil_mode.id = 2;
                    UnkRecoilParams_l846ec0 params;
                    params.ptr = nullptr;
                    params.value = 1.0f;
                    recoil_mask.id = UnkAll_SpitSend().value;
                    fn_7101176690(animation, id, kEmptyKey_l846ec0, &recoil_mode, &g_unk_710164b958, &params,
                                  &recoil_mask);
                }
            } else {
                self->recoil_id = -1;
            }
        }
        if (self->recoil_id == -1 && fn_710117b348(animation, fn_7100cf6ef8())) {
            UnkTrackQuery query = {fn_7100cf6ef8(), nullptr, -1.0f};
            fn_710117b08c(animation, &query);
            if (query.result->sources.data != &g_unk_7101d47108->empty) {
                UnkRecoilRequest_l846ec0 request;
                const UnkRecoilDesc_l846ec0* desc = reinterpret_cast<const UnkRecoilDesc_l846ec0*>(&g_unk_710164b958);
                u64 desc0 = desc->words[0];
                u64 desc1 = desc->words[1];
                const UnkRecoilBlend_l846ec0* blend_desc =
                    reinterpret_cast<const UnkRecoilBlend_l846ec0*>(&g_unk_7172769740);
                u64 kind = blend_desc->kind;
                u32 value = blend_desc->value;
                u32 all = UnkAll_SpitSend().value;
                request.id = -1;
                request.desc[0] = desc0;
                request.desc[1] = desc1;
                request.blend_kind = kind;
                request.blend_value = value;
                request.flags = 0;
                request.mask = all;
                reinterpret_cast<UnkRecoilTrack_l846ec0*>(query.result)->vf_0x18(&request);

                base::reflection::UnkReflTypeIdArg mode;
                mode.id = 1;
                UnkRecoilParams_l846ec0 params;
                params.ptr = nullptr;
                params.value = 2.0f;
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(animation, request.id, kEmptyKey_l846ec0, &mode, &g_unk_710164b958, &params, &mask);
                self->recoil_id = request.id;
            }
        }
    }

    if (self->recoil_id != -1)
        return;
    UnkHyperBeamCooldown_l846ec0* cooldown = self->owner->field_0x2a8;
    if (!cooldown)
        return;
    if (!cooldown->field_0x12a && !(cooldown->field_0x12c > 0.0f) && self->cooldown <= 0.0f)
        self->cooldown = 0.12f;
    if (self->cooldown > 0.0f) {
        self->cooldown -= dt;
        if (self->cooldown < 0.0f)
            cooldown->field_0x12c = 0.12f;
    }
}
