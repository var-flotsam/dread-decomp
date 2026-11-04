// Unit 1441 of tools/tu_map.py: after the base fn_7100969130, when the owner (+0x30) tracks an object at +0x220,
// hooks the callback fn_7100969e84 (link at +0xd0, handle at +0xe8) up to it, as the track-link family
// (src/game/CCooldownXBossStrongWhipAttack/CCooldownXBossStrongWhipAttack_Start_A2_b72.cpp); then resets the
// fields after it. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkImpulseUser_A5_b68;
struct UnkImpulseMessage_A5_b68;
void fn_7100969130(char* self);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
bool fn_7100969e84(UnkImpulseUser_A5_b68* self, const UnkImpulseMessage_A5_b68* msg);

struct UnkUnit1441TrackLink_A4_b74 {
    u8 pad_0x0[0x8];
    void* context;
    bool (*callback)(UnkImpulseUser_A5_b68*, const UnkImpulseMessage_A5_b68*);
};

struct UnkUnit1441Range_A4_b74 {
    f32 lo;
    f32 hi;
};

struct UnkUnit1441Start_A4_b74 {
    u8 pad_0x0[0x30];
    char* owner;
    u8 pad_0x38[0x8];
    char* field_0x40;
    u8 pad_0x48[0x88];
    UnkUnit1441TrackLink_A4_b74 link;  // 0xd0
    UnkWeakHandle* handle;             // 0xe8
    u8 pad_0xf0[0xc];
    u32 field_0xfc;
    u32 field_0x100;
    UnkUnit1441Range_A4_b74 field_0x104;
    u8 pad_0x10c[0xc];
    bool field_0x118;
};

void fn_7100969604(UnkUnit1441Start_A4_b74* self) {
    fn_7100969130(reinterpret_cast<char*>(self));
    if (UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220)) {
        UnkUnit1441TrackLink_A4_b74* link = &self->link;
        link->context = self;
        link->callback = fn_7100969e84;
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    self->field_0xfc = 0;
    self->field_0x100 = *reinterpret_cast<u32*>(self->field_0x40 + 0x1ac);
    self->field_0x104 = {-1.0f, 1.0f};
    self->field_0x118 = false;
}
