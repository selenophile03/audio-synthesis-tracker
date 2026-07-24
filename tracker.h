#ifndef TRACKER_H
#define TRACKER_H

#include <stdint.h>

#define SAMPLE_RATE 44100
#define CHANNELS 1

typedef enum { WAVE_SINE, WAVE_SQUARE, WAVE_SAWTOOTH, WAVE_TRIANGLE } WaveType;

typedef struct {
    WaveType type;
    float volume; 
} Instrument;

typedef struct {
    uint8_t note_index; 
    uint8_t instrument; 
} NoteEvent;

typedef struct __attribute__((packed)) {
    char riff[4];              
    uint32_t overall_size;     
    char wave[4];              
    char fmt_chunk_marker[4];  
    uint32_t fmt_length;       
    uint16_t audio_format;     
    uint16_t num_channels;     
    uint32_t sample_rate;      
    uint32_t byte_rate;        
    uint16_t block_align;      
    uint16_t bits_per_sample;  
    char data_chunk_header[4]; 
    uint32_t data_size;        
} WavHeader;

#endif
