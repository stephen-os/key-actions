#pragma once

#include "Recording.h"

#include <string>

namespace KeyActions
{
    class Serialization
    {
    public:
        static bool SaveRecording(const Recording& recording, const std::string& folderPath = "recordings");
        static bool LoadRecording(Recording& recording, const std::string& filename);
        static std::vector<std::string> GetAvailableRecordings(const std::string& folderPath = "recordings");
    };
}