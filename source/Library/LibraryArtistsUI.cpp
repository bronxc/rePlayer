#include "LibraryArtistsUI.h"

#include <ImGui.h>
#include <IO/File.h>

#include <Database/Database.h>
#include <Library/LibrarySongsUI.h>

#include <ctime>

namespace rePlayer
{
    Library::ArtistsUI::ArtistsUI(Window& owner)
        : DatabaseArtistsUI(DatabaseID::kLibrary, owner)
    {}

    Library::ArtistsUI::~ArtistsUI()
    {}

    Library& Library::ArtistsUI::GetLibrary()
    {
        return reinterpret_cast<Library&>(m_owner);
    }

    void Library::ArtistsUI::SourcesUI(Artist* selectedArtist)
    {
        ImGui::TableNextColumn();
        ImGui::Separator();
        auto pos = ImGui::GetCursorPos();
        ImGui::TextUnformatted("Source:\n\n");
        ImGui::TableNextColumn();
        if (selectedArtist)
        {
            char txt[256];
            for (auto& source : selectedArtist->Sources())
            {
                ImGui::PushID(source.id.value);
                if (source.timeFetch)
                {
                    tm t;
                    localtime_s(&t, &source.timeFetch);
                    sprintf_s(txt, "%4d/%02d/%02d %02d:%02d %06X %s", 1900 + t.tm_year, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, source.id.internalId, source.GetName());
                }
                else
                    sprintf_s(txt, "----/--/-- --:-- %06X %s", source.id.internalId, source.GetName());
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputText("##source", txt, sizeof(txt), ImGuiInputTextFlags_ReadOnly);
                ImGui::PopID();
            }
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(5.0f / 7.0f, 0.6f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(5.0f / 7.0f, 0.7f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(5.0f / 7.0f, 0.8f, 1.0f));
            if (ImGui::Button("Import", ImVec2(-FLT_MIN, 0)))
            {
                auto& libary = GetLibrary();
                libary.m_imports = {};
                for (auto& source : selectedArtist->Sources())
                    libary.ImportArtist(source.id);
                libary.m_importArtists.isOpened = true;
                libary.m_imports.isOpened = &libary.m_importArtists.isOpened;
            }
            ImGui::PopStyleColor(3);
        }
        ImGui::Spacing();

    }

    void Library::ArtistsUI::OnSavedChanges(Artist* selectedArtist)
    {
        auto* songs = GetLibrary().m_songs;
        Array<std::pair<Song*, std::string>> toRename;
        std::string oldDirectory = songs->GetDirectory(selectedArtist);
        if (strcmp(selectedArtist->GetHandle(), m_selectedArtistCopy.handles[0].Items()))
        {
            auto selectedArtistId = selectedArtist->GetId();
            for (auto* song : m_db.Songs())
            {
                if (song->GetFileSize() == 0)
                    continue;
                for (auto artistId : song->ArtistIds())
                {
                    if (artistId == selectedArtistId)
                    {
                        toRename.Push();
                        toRename.Last().first = song;
                        toRename.Last().second = songs->GetFullpath(song);
                        break;
                    }
                }
            }
        }

        auto selectedArtistEdit = selectedArtist->Edit();
        selectedArtistEdit->realName = m_selectedArtistCopy.realName;
        selectedArtistEdit->handles = m_selectedArtistCopy.handles;
        selectedArtistEdit->countries = m_selectedArtistCopy.countries;
        selectedArtistEdit->groups = m_selectedArtistCopy.groups;

        if (toRename.IsNotEmpty())
        {
            for (auto& entry : toRename)
            {
                auto filename = songs->GetFullpath(entry.first);
                io::File::Move(entry.second.c_str(), filename.c_str());
            }
            //call rename if it's only a case change
            io::File::Rename(oldDirectory.c_str(), songs->GetDirectory(selectedArtist).c_str());
        }

        m_db.Raise(Database::Flag::kSaveArtists);
    }
}
// namespace rePlayer