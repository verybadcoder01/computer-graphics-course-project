#include "modelLibrary.h"
#include <algorithm>

namespace fs = std::filesystem;

ModelLibrary::ModelLibrary(fs::path root) : root(std::move(root)) {
    scan();
}

void ModelLibrary::scan() {
    entries.clear();
    scanFolder(root / "parts", LibraryEntryKind::Part);
    scanFolder(root / "nozzles", LibraryEntryKind::Nozzle);
}

void ModelLibrary::scanFolder(const fs::path &folder, LibraryEntryKind kind) {
    std::error_code error;
    if (!fs::is_directory(folder, error)) {
        return;
    }

    std::vector<LibraryEntry> found;
    for (const auto &file : fs::directory_iterator(folder, error)) {
        if (!file.is_regular_file() || file.path().extension() != ".obj") {
            continue;
        }
        LibraryEntry entry{
            .name = file.path().stem().string(), .objPath = file.path(), .kind = kind};
        for (const char *extension : {".png", ".jpg", ".jpeg"}) {
            fs::path texture = file.path();
            texture.replace_extension(extension);
            if (fs::is_regular_file(texture, error)) {
                entry.texturePath = texture;
                break;
            }
        }
        found.push_back(std::move(entry));
    }

    std::sort(found.begin(), found.end(),
              [](const auto &a, const auto &b) { return a.name < b.name; });
    entries.insert(entries.end(), found.begin(), found.end());
}
