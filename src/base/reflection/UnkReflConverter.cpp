// The registry of conversions between reflected types (base/reflection/UnkReflConverter.h): a
// singly linked list of converter records, newest first, that the static initialisers push
// onto. A record's link is only written when the list already has a head, so the first record
// keeps its zero-initialised link.
#include "base/reflection/UnkReflConverter.h"

void fn_71000a16c4(UnkConverter* converter) {
    if (UnkConverter* head = g_unk_7101d5bdc8)
        converter->next = head;
    g_unk_7101d5bdc8 = converter;
}
