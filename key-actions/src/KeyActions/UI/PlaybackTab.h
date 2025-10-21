#pragma once
#include "Tab.h"

#include "KeyActions/Core/Recording.h"
#include "KeyActions/Core/PlaybackSession.h"
#include "KeyActions/Core/Serialization.h"

#include <vector>
#include <string>

namespace Lumina
{
    class PlaybackTab : public Tab
    {
    public:
        PlaybackTab();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(float timestep) override;
        virtual void OnEvent(Event& e) override;
        virtual void OnRender() override;

    private:
        void LoadRecordingsList();
        void LoadSelectedRecording();

    private:
        PlaybackSession m_PlaybackSession;

        // Available recordings
        std::vector<std::string> m_AvailableRecordings;
        int m_SelectedRecordingIndex = -1;

        // Loaded recording
        Recording m_LoadedRecording;
        bool m_HasLoadedRecording = false;

        // Playback settings
        PlaybackSettings m_Settings;

        // Progress tracking
        float m_CurrentProgress = 0.0f;
        size_t m_CurrentEventIndex = 0;

        // Stats
        int m_TotalPlays = 0;
    };
}