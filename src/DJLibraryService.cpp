#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    //Create canonical AudioTrack objects from SessionConfig::library_tracks.
    library.clear();
    library.reserve(library_tracks.size());
    // For each track: 
    //(a) Check the format field (“MP3” or “WAV”) 
    //(b) Create appropriate track type using the provided metadata (check the config f ile layout). 
    //(c) Store the raw pointer in the library vector
    //(d) Log creation message:– MP3: MP3Track created: <bitrate> kbps– WAV: WAVTrack created: <sample_rate>Hz/<bit_depth>bit 

    for (const auto& track_info : library_tracks) {
        AudioTrack* track = nullptr;
        if (track_info.type == "MP3") {
            track = new MP3Track(track_info.title, track_info.artists, 
                                 track_info.duration_seconds, track_info.bpm, 
                                 track_info.extra_param1, // bitrate
                                 track_info.extra_param2  // has_tags
                                 );
            std::cout << "[INFO] MP3Track created: " << track_info.extra_param1 << " kbps\n";
        } else if (track_info.type == "WAV") {
            track = new WAVTrack(track_info.title, track_info.artists, 
                                 track_info.duration_seconds, track_info.bpm, 
                                 track_info.extra_param1, // sample_rate
                                 track_info.extra_param2  // bit_depth
                                 );
            std::cout << "[INFO] WAVTrack created: " << track_info.extra_param1 << "Hz/" 
                      << track_info.extra_param2 << "bit\n";
        } else {
            std::cout << "[WARNING] Unknown track type: " << track_info.type << "\n";
            continue; // Skip unknown types
        }
        library.push_back(track);
    }

    //Log summary: [INFO] Track library built: <count> tracks loaded
    
    std::cout << "[INFO] Tracks library built \n"<< library_tracks.size() << " tracks loaded.\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    //Search playlist for track by title

    AudioTrack* track = playlist.find_track(track_title);
    if (track) {
        return track;   // Found
    } 
    else {    
        return nullptr;     // Not found
    }
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
   
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    for (const auto& index : track_indices) {
        if (index < 1 || index > static_cast<int>(library.size())) {
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
            continue; // Skip invalid index
        }
        AudioTrack* original_track = library[index - 1];
        PointerWrapper<AudioTrack> cloned_track = original_track->clone();
        if (!cloned_track) {
            std::cout << "[ERROR] Failed to clone track at index: " << index << std::endl;
            continue; // Skip if clone failed
        }
        cloned_track->load();
        cloned_track->analyze_beatgrid();
        playlist.add_track(cloned_track.release()); // Transfer ownership
        std::cout << "Added '" << cloned_track->get_title() 
                  << "' to playlist '" << playlist_name << "'" << std::endl;
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" 
              << playlist.get_track_count() << " tracks)" << std::endl;
    
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {

    std::vector<std::string> titles;

    auto tracks = playlist.getTracks();
    titles.reserve(tracks.size());
    for (const auto& track : tracks) {
        if (&track)
        {
            titles.push_back(track->get_title());
        }
    }
    return titles; 
}
