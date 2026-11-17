// CModelUpdaterComponent's unit: reads a queued model event from a property bag: its position, its three
// times (each stored as a float or an int) and the name of the node it applies to.
// Names and layouts are placeholders.
#include <algorithm>

#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct UnkModelEvent_ovb0032;

// The event as this function fills it.
struct UnkModelEventView_l16ed2c {
    f32 pos_x;      // 0x0
    f32 pos_y;      // 0x4
    f32 pos_z;      // 0x8
    f32 init_time;  // 0xc
    f32 time;       // 0x10
    f32 end_time;   // 0x14
    CStrId node;    // 0x18
};

// A value in the bag: where it is in the bag's stream, the hash of its name and its type.
struct UnkBagValue_l16ed2c {
    u64 offset;
    u64 key;
    s32 type;  // 'f' float, 'i' int, 's' string
};

static inline UnkBagValue_l16ed2c* Values_l16ed2c(UnkPropBag& bag) {
    return reinterpret_cast<UnkBagValue_l16ed2c*>(bag.values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkBagValue_l16ed2c* FindValue_l16ed2c(UnkPropBag& bag, const CStrId& id) {
    UnkBagValue_l16ed2c* values = Values_l16ed2c(bag);
    u64 key = id.data->Key();
    UnkIter<UnkBagValue_l16ed2c> first{values};
    UnkIter<UnkBagValue_l16ed2c> last{&values[bag.values.count]};
    return std::find_if(first, last, [key](const UnkBagValue_l16ed2c& v) { return v.key == key; }).p;
}

static inline UnkBagValue_l16ed2c* End_l16ed2c(UnkPropBag& bag) {
    return &Values_l16ed2c(bag)[bag.values.count];
}

// Reads `size` bytes of the value `it` into `dest`.
static inline void Read_l16ed2c(UnkPropBag& bag, const UnkBagValue_l16ed2c* it, void* dest, u64 size) {
    fn_710007c00c(&bag.lock, it->offset, 0);
    fn_710007c70c(&bag.lock, dest, size);
}

void fn_710116ed2c(UnkModelEvent_ovb0032* out, void* source) {
    UnkModelEventView_l16ed2c* event = reinterpret_cast<UnkModelEventView_l16ed2c*>(out);
    UnkPropBag& bag = *static_cast<UnkPropBag*>(source);
    {
        s32 value;
        CStrId id("PosX", true);
        UnkBagValue_l16ed2c* it = FindValue_l16ed2c(bag, id);
        if (it != End_l16ed2c(bag)) {
            if (it->type == 'f') {
                Read_l16ed2c(bag, it, &event->pos_x, 4);
            } else if (it->type == 'i') {
                Read_l16ed2c(bag, it, &value, 4);
                event->pos_x = value;
            }
        }
    }
    {
        s32 value;
        CStrId id("PosY", true);
        UnkBagValue_l16ed2c* it = FindValue_l16ed2c(bag, id);
        if (it != End_l16ed2c(bag)) {
            if (it->type == 'f') {
                Read_l16ed2c(bag, it, &event->pos_y, 4);
            } else if (it->type == 'i') {
                Read_l16ed2c(bag, it, &value, 4);
                event->pos_y = value;
            }
        }
    }
    {
        s32 value;
        CStrId id("PosZ", true);
        UnkBagValue_l16ed2c* it = FindValue_l16ed2c(bag, id);
        if (it != End_l16ed2c(bag)) {
            if (it->type == 'f') {
                Read_l16ed2c(bag, it, &event->pos_z, 4);
            } else if (it->type == 'i') {
                Read_l16ed2c(bag, it, &value, 4);
                event->pos_z = value;
            }
        }
    }
    {
        s32 value;
        CStrId id("InitTime", true);
        UnkBagValue_l16ed2c* it = FindValue_l16ed2c(bag, id);
        if (it != End_l16ed2c(bag)) {
            if (it->type == 'f') {
                Read_l16ed2c(bag, it, &event->init_time, 4);
            } else if (it->type == 'i') {
                Read_l16ed2c(bag, it, &value, 4);
                event->init_time = value;
            }
        }
    }
    {
        s32 value;
        CStrId id("Time", true);
        UnkBagValue_l16ed2c* it = FindValue_l16ed2c(bag, id);
        if (it != End_l16ed2c(bag)) {
            if (it->type == 'f') {
                Read_l16ed2c(bag, it, &event->time, 4);
            } else if (it->type == 'i') {
                Read_l16ed2c(bag, it, &value, 4);
                event->time = value;
            }
        }
    }
    {
        s32 value;
        CStrId id("EndTime", true);
        UnkBagValue_l16ed2c* it = FindValue_l16ed2c(bag, id);
        if (it != End_l16ed2c(bag)) {
            if (it->type == 'f') {
                Read_l16ed2c(bag, it, &event->end_time, 4);
            } else if (it->type == 'i') {
                Read_l16ed2c(bag, it, &value, 4);
                event->end_time = value;
            }
        }
    }
    const char* node = nullptr;
    {
        CStrId id("Node", true);
        UnkBagValue_l16ed2c* it = FindValue_l16ed2c(bag, id);
        if (it != End_l16ed2c(bag) && it->type == 's')
            node = reinterpret_cast<const char*>(bag.lock.data) + it->offset;
    }
    if (node)
        event->node = CStrId(node, true);
}
