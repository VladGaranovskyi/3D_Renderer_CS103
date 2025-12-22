#ifndef USERQUERY_H
#define USERQUERY_H

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

// NOTE:
// The UI now supports typing an .obj path directly (ImGui "Load" button).
// This function remains for compatibility, but MUST NOT destroy/truncate files.

/* The file copying logic got from:
   https://en.cppreference.com/w/cpp/filesystem/copy_file
   This cppreference page explains how to safely copy files using C++17 filesystem.
*/

string chooseOBJ() {
    // Default to a safe built-in model; let the ImGui UI load anything else.
    return "Objects/monkey.obj";
}

// Safe copy helper (used if you still want to keep console "upload" in the future)
static bool CopyObjSafelyIntoObjects(const std::string& sourcePath, std::string& outDestinationPath)
{
    namespace fs = std::filesystem;

    try {
        fs::path src(sourcePath);
        if (!fs::exists(src)) return false;

        fs::path dest = fs::path("Objects") / src.filename();
        outDestinationPath = dest.string();

        // If source and destination are actually the same file, do nothing.
        if (fs::exists(dest))
        {
            std::error_code ec;
            if (fs::equivalent(src, dest, ec) && !ec)
            {
                return true;
            }

            // Create a simple backup next to destination before overwriting.
            fs::path backup = dest.parent_path() / (dest.stem().string() + "_backup" + dest.extension().string());
            if (!fs::exists(backup))
            {
                fs::copy_file(dest, backup, fs::copy_options::overwrite_existing);
            }
        }

        // Read source fully before writing dest (prevents truncation issues).
        std::ifstream in(src, std::ios::binary);
        if (!in.is_open()) return false;
        std::string bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();

        std::ofstream out(dest, std::ios::binary | std::ios::trunc);
        if (!out.is_open()) return false;
        out.write(bytes.data(), (std::streamsize)bytes.size());
        out.close();

        return true;
    } catch (...) {
        return false;
    }
}

#endif
