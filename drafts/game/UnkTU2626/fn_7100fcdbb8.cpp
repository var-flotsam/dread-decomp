// Unit 2626: with `all`, stops every live sound of a keyed sound-handle table and empties it;
// otherwise drops the entries whose sound is gone or finished. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU101.h"
#include "types.h"

// The sound a handle names, as far as this unit reads it.
struct UnkSoundView_2626 {
    u8 pad_0x0[0x119];
    bool field_0x119;  // finished
};

struct UnkSoundRef_2626 {
    UnkSoundView_2626* object;
    s64 serial;
};

struct UnkSoundHandle_2626 {
    UnkSoundRef_2626* ref;
    s64 serial;

    // The sound while it is still the one the handle was made for.
    UnkSoundView_2626* Get() const {
        if (ref && serial != -1 && ref->serial == serial)
            return ref->object;
        return nullptr;
    }
};

// Sound handles kept in one array and their keys in a parallel one.
struct UnkSoundHandleTable_2626b {
    u8 pad_0x0[0x10];
    UnkSoundHandle_2626* handles;  // 0x10
    u64* keys;                     // 0x18
    u32 count;                     // 0x20

    UnkSoundHandle_2626* begin() { return handles; }
    UnkSoundHandle_2626* end() { return handles + count; }

    // The handle stored under `key`, or one past the last on a miss.
    UnkSoundHandle_2626* Find(const u64& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &handles[i];
        return &handles[count];
    }

    // Removes an entry by moving the last entry into its place.
    void Erase(UnkSoundHandle_2626* it) {
        u32 i = it - handles;
        u32 last = count - 1;
        if (i != last) {
            handles[i] = handles[last];
            keys[i] = keys[count - 1];
        }
        count = count - 1;
    }
};

// The keys gathered for removal, kept between calls so the array is reused.
struct UnkKeyList_2626 {
    base::global::CRntVector<u64> keys;

    UnkKeyList_2626() : keys(0xf) {}
    ~UnkKeyList_2626();  // fn_7100fca440
};

void fn_7100fcdbb8(void* self, bool all) {
    UnkSoundHandleTable_2626b* table = static_cast<UnkSoundHandleTable_2626b*>(self);
    if (all) {
        UnkSoundHandle_2626* end = table->end();
        for (UnkSoundHandle_2626* it = table->begin(); it < end; ++it) {
            if (UnkSoundView_2626* sound = it->Get())
                fn_7100105034(sound);
        }
        table->count = 0;
        return;
    }

    static UnkKeyList_2626 s_finished;
    base::global::CRntVector<u64>& finished = s_finished.keys;

    finished.count = 0;
    UnkSoundHandle_2626* end = table->end();
    for (UnkSoundHandle_2626* it = table->begin(); it < end; ++it) {
        UnkSoundView_2626* sound = it->Get();
        if (!sound || sound->field_0x119) {
            u32 index = it - table->handles;
            finished.PushBack(table->keys[index]);
        }
    }

    for (u32 i = 0; i < finished.count; ++i) {
        const u64& key = finished.GrowAt(i);
        UnkSoundHandle_2626* found = table->Find(key);
        if (found != &table->handles[table->count])
            table->Erase(found);
    }
}
