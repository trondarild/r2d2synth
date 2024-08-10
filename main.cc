#include "matrix.h"
#include <AudioToolbox/AudioToolbox.h>
#include <cmath>
#include <atomic>
#include "r2d2synth.h"

const int SAMPLE_RATE = 44100;
const int BUFFER_SIZE = 4096; // Increased buffer size
const int NUM_BUFFERS = 3;    // Use multiple buffers

struct AudioData {
    const float* buffer;
    int size;
    std::atomic<int> currentPosition;
    bool isFinished;
};

void audioQueueOutputCallback(void* inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer) {
    AudioData* audioData = static_cast<AudioData*>(inUserData);
    int remainingFrames = audioData->size - audioData->currentPosition;
    int framesToCopy = std::min(remainingFrames, static_cast<int>(inBuffer->mAudioDataBytesCapacity / sizeof(float)));

    if (framesToCopy > 0) {
        memcpy(inBuffer->mAudioData, 
               audioData->buffer + audioData->currentPosition, 
               framesToCopy * sizeof(float));
        inBuffer->mAudioDataByteSize = framesToCopy * sizeof(float);
        audioData->currentPosition += framesToCopy;
    } else {
        inBuffer->mAudioDataByteSize = 0;
        audioData->isFinished = true;
    }

    AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, NULL);
}

void playMatrixAsAudio(ikaros::matrix& audioMatrix) {
    AudioStreamBasicDescription asbd;
    memset(&asbd, 0, sizeof(asbd));
    asbd.mSampleRate = SAMPLE_RATE;
    asbd.mFormatID = kAudioFormatLinearPCM;
    asbd.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked | kAudioFormatFlagIsNonInterleaved;
    asbd.mBitsPerChannel = 32;
    asbd.mChannelsPerFrame = 1;
    asbd.mFramesPerPacket = 1;
    asbd.mBytesPerFrame = 4;
    asbd.mBytesPerPacket = 4;

    AudioQueueRef queue;
    AudioData audioData = { audioMatrix.data(), static_cast<int>(audioMatrix.size()), 0, false };
    AudioQueueNewOutput(&asbd, audioQueueOutputCallback, &audioData, NULL, NULL, 0, &queue);

    for (int i = 0; i < NUM_BUFFERS; ++i) {
        AudioQueueBufferRef buffer;
        AudioQueueAllocateBuffer(queue, BUFFER_SIZE * sizeof(float), &buffer);
        audioQueueOutputCallback(&audioData, queue, buffer);
    }

    AudioQueueStart(queue, NULL);

    while (!audioData.isFinished) {
        usleep(10000); // Sleep for 10ms
    }

    usleep(500000); // Wait an additional 500ms to ensure all audio is played

    AudioQueueStop(queue, true);
    AudioQueueDispose(queue, true);
}

ikaros::matrix generateSineWave(int duration, float frequency) {
    int numSamples = duration * SAMPLE_RATE;
    ikaros::matrix sineWave(numSamples);

    int fadeInSamples = SAMPLE_RATE;
    int fadeOutSamples = SAMPLE_RATE;

    for (int i = 0; i < numSamples; ++i) {
        float t = static_cast<float>(i) / SAMPLE_RATE;
        float amplitude = 0.5f; // Reduced amplitude to prevent clipping

        if (i < fadeInSamples) {
            amplitude *= static_cast<float>(i) / fadeInSamples;
        } else if (i > numSamples - fadeOutSamples) {
            amplitude *= static_cast<float>(numSamples - i) / fadeOutSamples;
        }

        sineWave(i) = amplitude * std::sin(2 * M_PI * frequency * t);
    }

    return sineWave;
}

int main() {
    R2D2Synth synth(SAMPLE_RATE);
    
    // Generate and play a normal R2D2 sound
    /*
    std::cout << "Playing normal R2D2 sound..." << std::endl;
    ikaros::matrix normalSound = synth.generateSound(1.5f);
    playMatrixAsAudio(normalSound);
    usleep(500);

    // Generate and play a happy R2D2 sound
    std::cout << "Playing happy R2D2 sound..." << std::endl;
    ikaros::matrix happySound = synth.generateHappySound(1.0f);
    playMatrixAsAudio(happySound);
    usleep(500);
    */
    // Generate and play a surprised R2D2 sound
    std::cout << "Playing surprised R2D2 sound..." << std::endl;
    ikaros::matrix surprisedSound = synth.generateSurprisedSound(0.5f);
    playMatrixAsAudio(surprisedSound);
    usleep(500);
    

    // Generate and play a wow R2D2 sound
    std::cout << "Playing wow R2D2 sound..." << std::endl;
    ikaros::matrix wowSound = synth.generateWowSound(2.0f);
    playMatrixAsAudio(wowSound);
    return 0;
}