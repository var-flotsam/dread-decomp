// Unit 2742 (no reliable name): the destructor of the character class table's entry (read by
// fn_71011ad60c, src/game/CCharClassComponent/CCharClassComponent_Read_ovb0127.cpp): its property bag is
// destroyed and freed, then the command, then the string id is released. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkCharClassEntry_ovb0127;
struct UnkPropBag;
struct UnkCommand_b0163;
void fn_710050e8e4(UnkPropBag*);
void fn_71010c7328(UnkCommand_b0163* self);

// An entry of a character class table: an id, an owned property bag and a command.
struct UnkEntryBody_b0216 {
    CStrId id;
    UnkPropBag* bag;
    u8 pad_0x10[0x8];
    u8 command[0x80];
};

void fn_71011ad580(UnkCharClassEntry_ovb0127* entry) {
    UnkEntryBody_b0216* self = reinterpret_cast<UnkEntryBody_b0216*>(entry);
    UnkPropBag* bag = self->bag;
    if (bag) {
        fn_710050e8e4(bag);
        fn_7100083c00(bag);
        self->bag = nullptr;
    }
    fn_71010c7328(reinterpret_cast<UnkCommand_b0163*>(self->command));
    self->id.Reset();
}
