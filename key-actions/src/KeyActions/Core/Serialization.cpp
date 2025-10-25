#include "Serialization.h"

#include "Lumina/Core/Log.h"

#include "Settings.h"

#include <fstream>
#include <filesystem>
#include <json.hpp>

using json = nlohmann::json;

namespace KeyActions
{
    bool Serialization::SaveRecording(const Recording& recording, const std::string& folderPath)
    {
        const auto& settings = Settings::Data();
        std::filesystem::path recordingsFolder = settings.RecordingsFolder; 

        if (!std::filesystem::exists(recordingsFolder))
        {
            std::error_code errorCode;
            if (!std::filesystem::create_directories(recordingsFolder, errorCode))
            {
                LUMINA_LOG_ERROR("Failed to create recordings folder: {}", recordingsFolder.string());
				return false;
            }
        }
        
        std::string filename = recording.Name + ".rec";
        std::filesystem::path filePath = recordingsFolder / filename;

        // For now, we will just overwrite any existing recording with the same name
        if (std::filesystem::exists(filePath))
        {
            LUMINA_LOG_INFO("Overwriting existing recording: {}", filePath.string());
        }

        try
        {
            json j;
            j["name"] = recording.Name;
            j["recordsMouse"] = recording.RecordsMouse;
            j["totalDuration"] = recording.TotalDuration;

            json eventsArray = json::array();
            for (const auto& event : recording.Events)
            {
                json eventJson;
                eventJson["action"] = static_cast<int>(event.Action);
                eventJson["time"] = event.Time;

                if (event.Action == RecordedAction::KeyPressed || event.Action == RecordedAction::KeyReleased)
                {
                    eventJson["key"] = static_cast<int>(event.Key);
                }
                else if (event.Action == RecordedAction::MousePressed || event.Action == RecordedAction::MouseReleased)
                {
                    eventJson["button"] = static_cast<int>(event.Button);
                    eventJson["x"] = event.MouseX;
                    eventJson["y"] = event.MouseY;
                }
                else if (event.Action == RecordedAction::MouseMoved)
                {
                    eventJson["x"] = event.MouseX;
                    eventJson["y"] = event.MouseY;
                }
                else if (event.Action == RecordedAction::MouseScrolled)
                {
                    eventJson["dx"] = event.ScrollDX;
                    eventJson["dy"] = event.ScrollDY;
                }

                eventsArray.push_back(eventJson);
            }

            j["events"] = eventsArray;

            std::ofstream file(filePath);
            file << j.dump(2);
            file.close();

            LUMINA_LOG_INFO("Saved recording: {}", filePath.string());
            return true;
        }
        catch (const std::exception& e)
        {
            LUMINA_LOG_ERROR("Failed to save recording: {}", e.what());
            return false;
        }
    }

    bool Serialization::LoadRecording(Recording& recording, const std::string& filename)
    {
        const auto& settings = Settings::Data();
		std::filesystem::path recordingsFolder = settings.RecordingsFolder;

        std::filesystem::path filePath = recordingsFolder;

        if (std::filesystem::path(recordingsFolder).is_absolute())
        {
            filePath = recordingsFolder;
        }
        else
        {
            filePath = recordingsFolder / filename;
        }

        try
        {
			std::ifstream file(filePath);
            if (!file.is_open())
            {
                LUMINA_LOG_ERROR("Failed to open recording file: {}", filePath.string());
                return false;
            }

            json j;
            file >> j;
            file.close();

            recording.Name = j["name"];
            recording.RecordsMouse = j["recordsMouse"];
            recording.TotalDuration = j["totalDuration"];

            recording.Events.clear();
            for (const auto& eventJson : j["events"])
            {
                RecordedEvent event;
                event.Action = static_cast<RecordedAction>(eventJson["action"]);
                event.Time = eventJson["time"];

                if (event.Action == RecordedAction::KeyPressed || event.Action == RecordedAction::KeyReleased)
                {
                    event.Key = static_cast<Lumina::KeyCode>(eventJson["key"]);
                }
                else if (event.Action == RecordedAction::MousePressed || event.Action == RecordedAction::MouseReleased)
                {
                    event.Button = static_cast<Lumina::MouseCode>(eventJson["button"]);
                    event.MouseX = eventJson["x"];
                    event.MouseY = eventJson["y"];
                }
                else if (event.Action == RecordedAction::MouseMoved)
                {
                    event.MouseX = eventJson["x"];
                    event.MouseY = eventJson["y"];
                }
                else if (event.Action == RecordedAction::MouseScrolled)
                {
                    event.ScrollDX = eventJson["dx"];
                    event.ScrollDY = eventJson["dy"];
                }

                recording.Events.push_back(event);
            }

            LUMINA_LOG_INFO("Loaded recording: {}", filePath.string());
            return true;
        }
        catch (const std::exception& e)
        {
            LUMINA_LOG_ERROR("Failed to load recording: {}", e.what());
            return false;
        }
    }

    std::vector<std::string> Serialization::GetAvailableRecordings(const std::string& folderPath)
    {
        const auto& settings = Settings::Data();
        std::filesystem::path recordingsFolder = settings.RecordingsFolder;

        std::vector<std::string> recordings;

        if (!std::filesystem::exists(recordingsFolder))
        {
            LUMINA_LOG_WARN("Recordings folder does not exist: {}", recordingsFolder.string());
            return recordings;
        }

        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(recordingsFolder))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".rec")
                {
                    recordings.push_back(entry.path().stem().string());
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            LUMINA_LOG_ERROR("Error reading recordings directory: {}", e.what());
        }

        return recordings;
    }
}