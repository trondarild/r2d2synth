#include "matrix.h"
#include <cmath>
#include <random>
#include <vector>

class R2D2Synth {
private:
    const int sampleRate;
    std::mt19937 rng;
    std::uniform_real_distribution<float> uniformDist;

    float generateChirp(float t, float baseFreq, float freqRange, float chirpRate) {
        float freq = baseFreq + freqRange * std::sin(chirpRate * t);
        return std::sin(2 * M_PI * freq * t);
    }

    float applyEnvelope(float t, float duration, float attackTime, float releaseTime) {
        if (t < attackTime) {
            return t / attackTime;
        } else if (t > duration - releaseTime) {
            return (duration - t) / releaseTime;
        }
        return 1.0f;
    }

    float generateNoise(float amplitude) {
        return amplitude * (uniformDist(rng) * 2 - 1);
    }

    float generateLaughPulse(float t, float baseFreq, float freqRange, float pulseRate) {
        float freq = baseFreq + freqRange * std::sin(2 * M_PI * pulseRate * t);
        return std::sin(2 * M_PI * freq * t);
    }

public:
    R2D2Synth(int sampleRate = 44100) : sampleRate(sampleRate), 
                                        rng(std::random_device{}()),
                                        uniformDist(0.0f, 1.0f) {}

    ikaros::matrix generateSound(float duration) {
        int numSamples = static_cast<int>(duration * sampleRate);
        ikaros::matrix sound(numSamples);

        float baseFreq = 1000 + uniformDist(rng) * 1000;
        float freqRange = 500 + uniformDist(rng) * 500;
        float chirpRate = 10 + uniformDist(rng) * 20;

        for (int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            float sample = generateChirp(t, baseFreq, freqRange, chirpRate);
            float envelope = applyEnvelope(t, duration, 0.1f, 0.1f);
            sound(i) = 0.5f * envelope * sample;
        }

        return sound;
    }

    ikaros::matrix generateHappySound(float duration) {
        int numSamples = static_cast<int>(duration * sampleRate);
        ikaros::matrix sound(numSamples);

        std::vector<float> baseFreqs = {1500, 2000, 2500};
        float chirpRate = 30 + uniformDist(rng) * 20;

        for (int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            float sample = 0;
            for (float baseFreq : baseFreqs) {
                sample += generateChirp(t, baseFreq, 200, chirpRate);
            }
            float envelope = applyEnvelope(t, duration, 0.05f, 0.1f);
            sound(i) = 0.3f * envelope * sample / baseFreqs.size();
        }

        return sound;
    }

    ikaros::matrix generateSurprisedSound(float duration) {
        int numSamples = static_cast<int>(duration * sampleRate);
        ikaros::matrix sound(numSamples);

        float startFreq = 500;
        float endFreq = 3000;
        float vibratoRate = 50;
        float vibratoDepth = 100;

        for (int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            float instantFreq = startFreq + (endFreq - startFreq) * t / duration;
            instantFreq += vibratoDepth * std::sin(2 * M_PI * vibratoRate * t);
            float phase = 2 * M_PI * instantFreq * t;
            float sample = std::sin(phase);
            float envelope = applyEnvelope(t, duration, 0.01f, 0.05f);
            sound(i) = 0.5f * envelope * sample;
        }

        return sound;
    }

    ikaros::matrix generateWowSound(float duration) {
        int numSamples = static_cast<int>(duration * sampleRate);
        ikaros::matrix sound(numSamples);

        float startFreq = 500;
        float endFreq = 1000;
        float wowRate = 0.75f; // Controls the speed of the "wow" effect

        for (int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sampleRate;
            
            // Create a slow, sweeping frequency modulation
            float modulation = 0.5f * (1 - std::cos(2 * M_PI * wowRate * t / duration));
            float instantFreq = startFreq + (endFreq - startFreq) * modulation;

            // Generate the primary tone
            float sample = std::sin(2 * M_PI * instantFreq * t);
            
            // Add harmonics for richness
            sample += 0.5f * std::sin(4 * M_PI * instantFreq * t);
            sample += 0.25f * std::sin(6 * M_PI * instantFreq * t);

            // Apply envelope
            float envelope = applyEnvelope(t, duration, 0.1f, 0.2f);
            
            sound(i) = 0.3f * envelope * sample;
        }

        return sound;
    }
    ikaros::matrix generateProtestSound(float duration) {
        int numSamples = static_cast<int>(duration * sampleRate);
        ikaros::matrix sound(numSamples);

        float baseFreq = 800;
        float freqRange = 400;
        float chirpRate = 20;
        int numChirps = 5;
        float chirpDuration = duration / numChirps;

        for (int chirp = 0; chirp < numChirps; chirp++) {
            int startSample = chirp * chirpDuration * sampleRate;
            int endSample = std::min((chirp + 1) * chirpDuration * sampleRate, (float)numSamples);

            for (int i = startSample; i < endSample; i++) {
                float t = static_cast<float>(i - startSample) / sampleRate;
                float sample = generateChirp(t, baseFreq, freqRange, chirpRate);
                float envelope = applyEnvelope(t, chirpDuration, 0.01f, 0.05f);
                sound(i) = 0.5f * envelope * sample;
            }
        }

        return sound;
    }

    ikaros::matrix generateIndignationSound(float duration) {
        int numSamples = static_cast<int>(duration * sampleRate);
        ikaros::matrix sound(numSamples);

        float baseFreq = 1200;
        float freqRange = 600;
        float chirpRate = 30;
        float noiseFactor = 0.2f;

        for (int i = 0; i < numSamples; i++) {
            float t = static_cast<float>(i) / sampleRate;
            float modulation = std::pow(std::sin(2 * M_PI * 2 * t / duration), 2);
            float instantFreq = baseFreq + freqRange * modulation;
            float sample = std::sin(2 * M_PI * instantFreq * t);
            sample += 0.5f * std::sin(4 * M_PI * instantFreq * t);
            sample += generateNoise(noiseFactor);
            float envelope = applyEnvelope(t, duration, 0.05f, 0.1f);
            sound(i) = 0.4f * envelope * sample;
        }

        return sound;
    }

    ikaros::matrix generateLaughterSound(float duration, float intensity) {
        intensity = std::clamp(intensity, 0.1f, 1.0f);
        int numSamples = static_cast<int>(duration * sampleRate);
        ikaros::matrix sound(numSamples);

        float baseFreq = 1000 + intensity * 500;  // Higher pitch for more intense laughter
        float freqRange = 200 + intensity * 300;  // Wider frequency range for more intense laughter
        float pulseRate = 5 + intensity * 15;     // Faster pulses for more intense laughter
        
        int numPulses = static_cast<int>(4 + intensity * 8);  // More pulses for more intense laughter
        float pulseDuration = duration / numPulses;
        float pulseSpacing = pulseDuration * 0.2f;  // 20% of pulse duration for spacing

        for (int pulse = 0; pulse < numPulses; pulse++) {
            float pulseStart = pulse * pulseDuration;
            float pulseEnd = pulseStart + pulseDuration - pulseSpacing;

            for (int i = 0; i < numSamples; i++) {
                float t = static_cast<float>(i) / sampleRate;
                if (t >= pulseStart && t < pulseEnd) {
                    float pulseT = t - pulseStart;
                    float sample = generateLaughPulse(pulseT, baseFreq, freqRange, pulseRate);
                    float envelope = applyEnvelope(pulseT, pulseDuration - pulseSpacing, 0.01f, 0.05f);
                    
                    // Add some randomness to the amplitude for a more natural sound
                    float randomFactor = 1.0f + 0.2f * (uniformDist(rng) - 0.5f);
                    
                    sound(i) += 0.5f * envelope * sample * randomFactor * intensity;
                }
            }
        }

        return sound;
    }
};