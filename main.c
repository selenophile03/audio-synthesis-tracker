#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tracker.h"

#define PI 3.14159265358979323846

float note_to_frequency(int note_index) {
    if (note_index == 0) return 0.0f; 
    return 440.0f * powf(2.0f, (note_index - 57) / 12.0f);
}

float generate_sample(float phase, WaveType type) {
    switch (type) {
        case WAVE_SINE:     return sinf(2.0f * PI * phase);
        case WAVE_SQUARE:   return (sinf(2.0f * PI * phase) >= 0) ? 1.0f : -1.0f;
        case WAVE_SAWTOOTH: return 2.0f * (phase - floorf(phase + 0.5f));
        case WAVE_TRIANGLE: return 2.0f * fabsf(2.0f * (phase - floorf(phase + 0.5f))) - 1.0f;
        default:            return 0.0f;
    }
}

void write_wav_file(const char* filename, float* buffer, int num_samples) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;

    uint32_t data_size = num_samples * sizeof(int16_t);
    WavHeader header = {
        .riff = {'R', 'I', 'F', 'F'},
        .overall_size = sizeof(WavHeader) + data_size - 8,
        .wave = {'W', 'A', 'V', 'E'},
        .fmt_chunk_marker = {'f', 'm', 't', ' '},
        .fmt_length = 16,
        .audio_format = 1, 
        .num_channels = CHANNELS,
        .sample_rate = SAMPLE_RATE,
        .byte_rate = SAMPLE_RATE * CHANNELS * sizeof(int16_t),
        .block_align = CHANNELS * sizeof(int16_t),
        .bits_per_sample = 16,
        .data_chunk_header = {'d', 'a', 't', 'a'},
        .data_size = data_size
    };

    fwrite(&header, sizeof(WavHeader), 1, f);

    for (int i = 0; i < num_samples; i++) {
        float sample = buffer[i];
        if (sample > 1.0f) sample = 1.0f;
        if (sample < -1.0f) sample = -1.0f;
        int16_t pcm_sample = (int16_t)(sample * 32767.0f);
        fwrite(&pcm_sample, sizeof(int16_t), 1, f);
    }
    fclose(f);
}

void render_song(NoteEvent* pattern, int total_rows, Instrument* insts, const char* filename) {
    int bpm = 130;
    int rows_per_beat = 4;
    float row_duration = 60.0f / (bpm * rows_per_beat); 
    int samples_per_row = (int)(SAMPLE_RATE * row_duration); 
    int total_samples = total_rows * samples_per_row;
    float* audio_buffer = calloc(total_samples, sizeof(float));
    float phase = 0.0f;
    
    for (int r = 0; r < total_rows; r++) {
        NoteEvent current_event = pattern[r];
        float freq = note_to_frequency(current_event.note_index);
        Instrument inst = insts[current_event.instrument];
        
        if (freq > 0 && (r == 0 || pattern[r-1].note_index != current_event.note_index)) {
            phase = 0.0f;
        }

        for (int s = 0; s < samples_per_row; s++) {
            int idx = (r * samples_per_row) + s;
            if (freq > 0.0f) {
                phase += freq / SAMPLE_RATE;
                if (phase > 1.0f) phase -= 1.0f;
                audio_buffer[idx] = generate_sample(phase, inst.type) * inst.volume;
            } else {
                audio_buffer[idx] = 0.0f;
            }
        }
    }
    write_wav_file(filename, audio_buffer, total_samples);
    free(audio_buffer);
}

int main() {
    Instrument square_lead = { .type = WAVE_SQUARE, .volume = 0.25f };
    Instrument sawtooth_bass = { .type = WAVE_SAWTOOTH, .volume = 0.20f };
    Instrument bank[] = { square_lead, sawtooth_bass };

    NoteEvent song_pattern[] = {
        {48, 0}, {52, 0}, {55, 0}, {60, 0},
        {48, 0}, {52, 0}, {55, 0}, {60, 0},
        {43, 1}, {47, 1}, {50, 1}, {55, 1},
        {43, 1}, {47, 1}, {50, 1}, {0,  0}  
    };

    printf("Synthesizing audio tracker tracks...\n");
    render_song(song_pattern, 16, bank, "synth_output.wav");
    printf("Success! Created 'synth_output.wav'.\n");
    return 0;
}
