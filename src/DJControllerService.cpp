    #include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if (cache.contains(track.get_title())){
        cache.get(track.get_title());
        return 0;
    }
    AudioTrack* rawPtr = track.clone().release();
    if (!rawPtr){
        std::cout << "[ERROR] Track: " << track.get_title() << "failed to clone.";
        return 0;
    }
    rawPtr->load();
    rawPtr->analyze_beatgrid();
    PointerWrapper<AudioTrack> wrap = PointerWrapper<AudioTrack>(rawPtr);
    bool result = cache.put(std::move(wrap));
    if (result){return -1;}
    return 0;
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title);
}
