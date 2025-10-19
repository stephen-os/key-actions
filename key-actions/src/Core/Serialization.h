#pragma once

#include <Core/Recording.h>

#include <string>

namespace Lumina
{
    class Serialization
    {
    public:
        static bool SaveRecording(const Recording& recording, const std::string& folderPath = "recordings");
        static bool LoadRecording(Recording& recording, const std::string& filepath);
        static std::vector<std::string> GetAvailableRecordings(const std::string& folderPath = "recordings");

    private:
        static std::string GetRecordingsFolder();
        static void EnsureRecordingsFolderExists(const std::string& folderPath);
    };
}