#include "Serialization.h"

#include "Lumina/Core/Log.h"

#include <fstream>
#include <filesystem>
#include <json.hpp>

using json = nlohmann::json;

namespace KeyActions
{
    void Serialization::EnsureRecordingsFolderExists(const std::string& folderPath)
    {
        if (!std::filesystem::exists(folderPath))
        {
            std::filesystem::create_directories(folderPath);
            LUMINA_LOG_INFO("Created recordings folder: {}", folderPath);
        }
    }

    bool Serialization::SaveRecording(const Recording& recording, const std::string& folderPath)
    {
        EnsureRecordingsFolderExists(folderPath);

        std::string filepath = folderPath + "/" + recording.Name + ".rec";

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

            std::ofstream file(filepath);
            file << j.dump(2);
            file.close();

            LUMINA_LOG_INFO("Saved recording: {}", filepath);
            return true;
        }
        catch (const std::exception& e)
        {
            LUMINA_LOG_ERROR("Failed to save recording: {}", e.what());
            return false;
        }
    }

    bool Serialization::LoadRecording(Recording& recording, const std::string& filepath)
    {
        try
        {
			std::ifstream file(filepath);
            if (!file.is_open())
            {
                LUMINA_LOG_ERROR("Failed to open recording file: {}", filepath);
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

            LUMINA_LOG_INFO("Loaded recording: {}", filepath);
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
        std::vector<std::string> recordings;

        if (!std::filesystem::exists(folderPath))
            return recordings;

        for (const auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            if (entry.path().extension() == ".rec")
            {
                recordings.push_back(entry.path().stem().string());
            }
        }

        return recordings;
    }
}