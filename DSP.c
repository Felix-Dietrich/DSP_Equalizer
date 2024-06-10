#include <stdio.h>
#include <stdlib.h>

// Define WAV file header structure
typedef struct {
    char riff[4];        // "RIFF"
    unsigned int size;   // File size
    char wave[4];        // "WAVE"
    char fmt[4];         // "fmt "
    unsigned int fmtSize;// Size of fmt chunk
    unsigned short format;// Format type
    unsigned short channels;// Number of channels
    unsigned int sampleRate;// Sample rate
    unsigned int byteRate;  // Byte rate
    unsigned short blockAlign;// Block align
    unsigned short bitsPerSample;// Bits per sample
    char data[4];        // "data"
    unsigned int dataSize;// Size of data chunk
} WAVHeader;

int main() {
    // Open the WAV file
    FILE *file = fopen("example.wav", "rb");
    if (!file) {
        printf("Could not open file.\n");
        return 1;
    }

    // Read the WAV file header
    WAVHeader header;
    fread(&header, sizeof(WAVHeader), 1, file);

    // Print some header information
    printf("Sample Rate: %u\n", header.sampleRate);
    printf("Bits per Sample: %u\n", header.bitsPerSample);
    printf("Number of Channels: %u\n", header.channels);

    // Allocate memory for audio data
    unsigned char *data = (unsigned char*)malloc(header.dataSize);
    if (!data) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    // Read the audio data
    fread(data, header.dataSize, 1, file);

    // Close the file
    fclose(file);

    // Process the audio data (e.g., play, analyze, etc.)
    // For now, just print the size of the data read
    printf("Read %u bytes of audio data.\n", header.dataSize);

    // Free allocated memory
    free(data);

    return 0;
}