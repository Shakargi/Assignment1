#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title
              << "\" at " << sample_rate << " Hz/" << bit_depth << " bit (uncompressed)...\n";
              long long size = (static_cast<long long>(duration_seconds) * sample_rate * (bit_depth / 8.0)) * 2; //Calculate estimated file size in bytes
    std::cout << "  → Estimated file size: " << size  << " bytes\n";
    std::cout << "  → Fast loading due to uncompressed format.\n";

}

void WAVTrack::analyze_beatgrid() {
    // 1. Print analysis message with track title
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement WAV-specific beat detection analysis
    
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    double beats = (duration_seconds / 60.0) * bpm;
    // 3. Print number of beats and mention uncompressed precision
    std::cout << "  → Estimated beats: " << beats 
              << "  → Precision factor: 1.0 (uncompressed audio)" << std::endl;
}

double WAVTrack::get_quality_score() const {
    // TODO: Implement WAV quality scoring

    // Start with a base score of 70 points 
    double score = 70.0;
    
    // Add 10 points if sample_rate >= 44100 (CD quality) 
    if (sample_rate >= 44100) {
        score += 10.0;

        // Add 5 more points if sample_rate >= 96000 (high-res audio) 
        if (sample_rate >= 96000) {
            score += 5.0;   
        }
    }
    
    // Add 10 points if bit_depth >= 16 (CD quality) 
    if (bit_depth >= 16) {
        score += 10.0;
        if (bit_depth >= 24) {
            score += 5.0;   //Add 5 more points if bit_depth >= 24 (professional quality) 
        }
    }
    
    if (score > 100.0) {
        score = 100.0;  //Cap maximum score at 100 points and return the final score
    }
    return score;
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method
    return PointerWrapper<AudioTrack>(new WAVTrack(*this)); // Deep copy via copy constructor
}
