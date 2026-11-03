// CChozoCommanderAIComponent unit (tools/tu_map.py): for every entry of the list fn_710045bc94
// returns, resets its sentence-sphere movement (+0x10; null for an empty slot) with
// fn_710045c5e8(..., 0). Names and layouts are placeholders.
#include "types.h"

struct UnkSentenceSphereMovement_b46a1;
void fn_710045c5e8(UnkSentenceSphereMovement_b46a1*, u32);

struct UnkSentenceSphereEntry {
    u8 pad_0x0[0x10];
    UnkSentenceSphereMovement_b46a1* movement;  // 0x10
};

struct UnkSentenceSphereList {
    UnkSentenceSphereEntry** data;
    u32 count;
};

UnkSentenceSphereList* fn_710045bc94();

struct UnkComponent;

void fn_71004565f0(UnkComponent* component) {
    UnkSentenceSphereList* list = fn_710045bc94();
    for (UnkSentenceSphereEntry **it = list->data, **end = list->data + list->count; it != end; ++it)
        fn_710045c5e8(*it != nullptr ? (*it)->movement : nullptr, 0);
}
