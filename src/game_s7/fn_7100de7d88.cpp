// The destructor of thirteen groups of three pointer arrays (one member of fn_7100de60b0's object):
// each group first clears the back pointers of the objects in its first array, then empties all
// three, then frees them. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

namespace unk_link_groups {

// The layout of base::UnkVector, with a destructor that frees the array.
template <class T>
struct UnkPtrArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkPtrArray() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkLink {
    void* target;
};

struct UnkLinkGroup {
    UnkPtrArray<UnkLink*> links;
    UnkPtrArray<void*> field_0x28;
    UnkPtrArray<void*> field_0x50;
    u64 field_0x78;

    ~UnkLinkGroup() {
        for (UnkLink **p = links.data, **e = links.data + links.count; p != e; ++p)
            (*p)->target = nullptr;
        links.count = 0;
        field_0x28.count = 0;
        field_0x50.count = 0;
    }
};

// Separate members: an array of thirteen is destroyed in a loop.
struct UnkLinkGroups {
    UnkLinkGroup field_0x0;
    UnkLinkGroup field_0x80;
    UnkLinkGroup field_0x100;
    UnkLinkGroup field_0x180;
    UnkLinkGroup field_0x200;
    UnkLinkGroup field_0x280;
    UnkLinkGroup field_0x300;
    UnkLinkGroup field_0x380;
    UnkLinkGroup field_0x400;
    UnkLinkGroup field_0x480;
    UnkLinkGroup field_0x500;
    UnkLinkGroup field_0x580;
    UnkLinkGroup field_0x600;

    ~UnkLinkGroups();
};
static_assert(sizeof(UnkLinkGroups) == 0x680, "");

UnkLinkGroups::~UnkLinkGroups() = default;

}  // namespace unk_link_groups

extern "C" void fn_7100de7d88() __attribute__((alias("_ZN15unk_link_groups13UnkLinkGroupsD2Ev")));
