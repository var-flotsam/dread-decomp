// sound::CMusicManager::CTunableMusicManager's unit (2637): whether the music a name stands for is what
// the manager plays now, by the playing requests' names or their first file's name. Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringPool;

// A track of a music request: its file and two words (0x18 bytes).
struct UnkMusicFile {
    CFilePathStrId path;
    u64 field_0x8;
    u64 field_0x10;
};

// The files, released before the request's name.
struct UnkMusicFiles : base::global::CRntVector<UnkMusicFile> {
    explicit UnkMusicFiles(u32 flags) : base::global::CRntVector<UnkMusicFile>(flags) {}
    ~UnkMusicFiles() { Free(); }
};

// A music request (0x60 bytes; the manager's request array at +0xb8 holds the playing ones): the
// channel it plays on, volumes, its files and its name.
struct UnkMusicRequest {
    u32 channel = 0;
    u32 field_0x4 = 1;
    f32 field_0x8 = 1.0f;
    u32 field_0xc = 0;
    u32 field_0x10 = 0;
    f32 field_0x14 = 1.0f;
    u8 field_0x18[6] = {};
    u16 field_0x1e = 0;
    u8 field_0x20[6] = {};
    bool field_0x26 = true;
    UnkMusicFiles files{0xf};  // 0x28
    u64 field_0x50 = 0;
    CStrId name{"", true};  // 0x58
};
static_assert(sizeof(UnkMusicRequest) == 0x60, "UnkMusicRequest placeholder layout");

// Fills the two requests a music id stands for.
void fn_7100fd73a4(UnkMusicManager* self, const CStrId* id, UnkMusicRequest* first, UnkMusicRequest* second);

// The music id built from a name (fn_7100000850 builds it in place), released in one step.
struct UnkMusicNameId_fd76d4 {
    base::global::UnkStringData* data;

    explicit UnkMusicNameId_fd76d4(const CRntString& name) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), name, true);
    }
    ~UnkMusicNameId_fd76d4() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = &g_unk_7101d47108->empty;
        }
    }
    const CStrId* Id() const { return reinterpret_cast<const CStrId*>(this); }
};

// The manager's playing request `index` (read from the array each time).
inline const UnkMusicRequest& Playing(const UnkMusicManager* self, u32 index) {
    return reinterpret_cast<const UnkMusicRequest*>(self->entries.data)[index];
}

// Whether an id's text is `name` (its key compared; the text read is counted).
inline bool IsNamed(const CStrId& id, const CRntString& name) {
    if (id.data == &g_unk_7101d47108->empty)
        return false;
    id.Text();
    return id.data->Key() == static_cast<u64>(name.key);
}

bool fn_7100fd76d4(void* manager, const CRntString* name) {
    auto* self = static_cast<UnkMusicManager*>(manager);
    UnkMusicRequest requests[2];
    UnkMusicRequest& first = requests[0];
    UnkMusicRequest& second = requests[1];
    {
        UnkMusicNameId_fd76d4 id(*name);
        fn_7100fd73a4(self, id.Id(), &first, &second);
    }
    if (first.files.count == 0 && second.files.count == 0)
        return false;
    if (first.name.data != g_unk_7101d470e0.data && first.name.data == Playing(self, 0).name.data)
        return true;
    bool result = false;
    if (first.files.count != 0 && Playing(self, 0).files.count != 0) {
        if (!IsNamed(Playing(self, 0).name, *name))
            return false;
        result = true;
    }
    if (second.name.data != g_unk_7101d470e0.data && second.name.data == Playing(self, 1).name.data)
        return true;
    if (second.files.count != 0 && Playing(self, 1).files.count != 0) {
        if (!IsNamed(Playing(self, 1).name, *name))
            return false;
        result = true;
    }
    return result;
}
