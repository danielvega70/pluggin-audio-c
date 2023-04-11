class SynthModule {
public:
    SynthModule() {}
    virtual ~SynthModule() {}
    virtual void update() {}
    virtual float getOutput() { return 0.0f; }
};

class Oscillator : public SynthModule {
public:
    Oscillator(float frequency, float amplitude, float phase)
        : frequency(frequency), amplitude(amplitude), phase(phase), output(0.0f) {}
    
    void setFrequency(float frequency) { this->frequency = frequency; }
    
    void update() override {
        float sample = amplitude * std::sin(2 * M_PI * frequency * phase);
        phase += 1.0f / sampleRate;
        if (phase >= 1.0f) phase -= 1.0f;
        output = sample;
    }
    
    float getOutput() override { return output; }

private:
    float frequency;
    float amplitude;
    float phase;
    float output;
};

class Filter : public SynthModule {
public:
    Filter(float cutoffFrequency, float resonance)
        : cutoffFrequency(cutoffFrequency), resonance(resonance), buffer{0.0f, 0.0f} {}
    
    void setCutoffFrequency(float cutoffFrequency) { this->cutoffFrequency = cutoffFrequency; }
    void setResonance(float resonance) { this->resonance = resonance; }

    void update() override {
        float input = getInput();
        float output = buffer[0] * (1.0f - resonance) + input * resonance;

        float c = std::tan(M_PI * cutoffFrequency / sampleRate);
        float a1 = 1.0f / (1.0f + resonance * c + c * c);
        float a2 = 2.0f * a1;
        float a3 = a1;
        float b1 = 2.0f * (1.0f - c * c) * a1;
        float b2 = (1.0f - resonance * c + c * c) * a1;

        output = a1 * input + a2 * buffer[0] + a3 * buffer[1] - b1 * getOutput() - b2 * buffer[2];
        setOutput(output);

        buffer[1] = buffer[0];
        buffer[0] = input;
    }

private:
    float cutoffFrequency;
    float resonance;
    float buffer[2];
};

class Envelope : public SynthModule {
public:
    Envelope(float attackTime, float decayTime, float sustainLevel, float releaseTime)
        : attackTime(attackTime), decayTime(decayTime), sustainLevel(sustainLevel),
        releaseTime(releaseTime), envState(EnvState::IDLE), state(0.0f), time(0.0f), output(0.0f) {}

    void gate(bool on) {
        if (on) {
            envState = EnvState::ATTACK;
            time = 0.0f;
        }
        else if (envState != EnvState::IDLE) {
            envState = EnvState::RELEASE;
            time = 0.0f;
        }
    }

    void update() override {
        switch (envState) {
            case EnvState::IDLE:
                setOutput(0.0f);
                break;
            case EnvState::ATTACK:
                time += 1.0f / sampleRate;
                state = time / attackTime;
                if (state >= 1.0
#include <iostream>

class SynthInterface {
public:
    virtual ~SynthInterface() {}
    virtual void setParameter(int parameter, float value) = 0;
};

class AnalogSynth : public SynthInterface {
public:
    AnalogSynth() : cutoff(0.5), resonance(0.5), envAmount(0.5), envTime(0.5) {}

    void setParameter(int parameter, float value) override {
        switch (parameter) {
            case 0:
                cutoff = value;
                break;
            case 1:
                resonance = value;
                break;
            case 2:
                envAmount = value;
                break;
            case 3:
                envTime = value;
                break;
            default:
                std::cout << "Error: Invalid parameter index\n";
                break;
        }
    }

    void play() {
        std::cout << "Playing analog synth with parameters:\n";
        std::cout << "Cutoff: " << cutoff << "\n";
        std::cout << "Resonance: " << resonance << "\n";
        std::cout << "Envelope amount: " << envAmount << "\n";
        std::cout << "Envelope time: " << envTime << "\n";
    }

private:
    float cutoff;
    float resonance;
    float envAmount;
    float envTime;
};

int main() {
    AnalogSynth synth;
    synth.setParameter(0, 0.8);
    synth.setParameter(1, 0.3);
    synth.setParameter(2, 0.6);
    synth.setParameter(3, 0.2);
    synth.play();
    return 0;
}
