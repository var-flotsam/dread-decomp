// Hands a list of nineteen flag values to fn_71008724f8 under a fixed key.
#include "UnkFlagListS7.h"

using unk_flag_list::UnkFlagList;
using unk_flag_list::UnkFlagListKey;

extern UnkFlagListKey g_unk_71729daee0 __attribute__((visibility("hidden")));

void fn_7100eed9ec(void* self, void* target) {
    UnkFlagList list;
    list.PushBack(0x1);
    list.PushBack(0x2);
    list.PushBack(0x20);
    list.PushBack(0x40);
    list.PushBack(0x80);
    list.PushBack(0x100);
    list.PushBack(0x200);
    list.PushBack(0x400);
    list.PushBack(0x800);
    list.PushBack(0x1000);
    list.PushBack(0x2000);
    list.PushBack(0x4000);
    list.PushBack(0x8000);
    list.PushBack(0x10000);
    list.PushBack(0x20000);
    list.PushBack(0x40000);
    list.PushBack(0x80000);
    list.PushBack(0x100000);
    list.PushBack(0x200000);
    fn_71008724f8(target, &g_unk_71729daee0, &list, 1, 1, 0, 1);
}
