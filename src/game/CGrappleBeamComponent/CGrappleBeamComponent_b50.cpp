// CGrappleBeamComponent unit (tools/tu_map.py). fn_710081b920 asks the owner's (+0x10 of the
// object at +0x230) component at +0x228 (fn_710117364c) about an id: the one at +0x30 of the id
// table's entry at +0xd30 (fn_7100031894), or, while the owner's animation (+0x220) plays a track
// of the class fn_7100c5aedc names, the one its record gives (+0x30 of +0x18). Returns the answer
// (empty unless set). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkIdHolder_b50gb {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkIdTable_b50gb {
    UnkIdTable_b50gb();  // fn_7100031894
    ~UnkIdTable_b50gb();
    u8 pad_0x0[0xd30];
    UnkIdHolder_b50gb* field_0xd30;
};

inline UnkIdTable_b50gb& IdTable_b50gb() {
    static UnkIdTable_b50gb s_table;
    return s_table;
}

// The answer, built in place (not trivially copyable: returned in memory).
struct UnkGrappleAnswer_b50 {
    void* field_0x0;
    u32 field_0x8;

    UnkGrappleAnswer_b50() : field_0x0(nullptr), field_0x8(0) {}
    UnkGrappleAnswer_b50(const UnkGrappleAnswer_b50& other);
};

struct UnkGrappleRecord_b50 {
    u8 pad_0x0[0x18];
    UnkIdHolder_b50gb* holder;
};

struct UnkGrappleOwner_b50 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    void* field_0x228;
};

struct UnkGrappleLink_b50 {
    u8 pad_0x0[0x10];
    UnkGrappleOwner_b50* owner;
};

struct UnkGrappleBeam_b50 {
    u8 pad_0x0[0x230];
    UnkGrappleLink_b50* link;
};

void fn_710117364c(void* target, const u64* id, UnkGrappleAnswer_b50* out);

UnkGrappleAnswer_b50 fn_710081b920(UnkGrappleBeam_b50* self) {
    UnkGrappleAnswer_b50 answer;
    u64 id = IdTable_b50gb().field_0xd30->id;
    if (UnkAnimationComponent* animation = self->link->owner->animation) {
        UnkTrackQuery query = {fn_7100c5aedc(), nullptr, -1.0f};
        if (fn_710117b08c(animation, &query))
            id = reinterpret_cast<UnkGrappleRecord_b50*>(query.result)->holder->id;
    }
    if (void* target = self->link->owner->field_0x228)
        fn_710117364c(target, &id, &answer);
    return answer;
}
