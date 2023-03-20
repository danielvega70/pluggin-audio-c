class EQAudioProcessorEditor  : public AudioProcessorEditor,
                                 public Slider::Listener,
                                 public Button::Listener
{
public:
    EQAudioProcessorEditor (EQAudioProcessor& p)
        : AudioProcessorEditor (&p), processor (p)
    {
        // Add the frequency slider
        frequencySlider.setSliderStyle (Slider::Rotary);
        frequencySlider.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        frequencySlider.setRange (20.0, 20000.0);
        frequencySlider.setValue (1000.0);
        frequencySlider.addListener (this);
        addAndMakeVisible (frequencySlider);

        // Add the gain knob
        gainKnob.setSliderStyle (Slider::Rotary);
        gainKnob.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        gainKnob.setRange (-24.0, 24.0);
        gainKnob.setValue (0.0);
        gainKnob.addListener (this);
        addAndMakeVisible (gainKnob);

        // Add the Q knob
        qKnob.setSliderStyle (Slider::Rotary);
        qKnob.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        qKnob.setRange (0.1, 10.0);
        qKnob.setValue (1.0);
        qKnob.addListener (this);
        addAndMakeVisible (qKnob);

        // Add the low cut button
        lowCutButton.setButtonText ("Low Cut");
        lowCutButton.addListener (this);
        addAndMakeVisible (lowCutButton);

        // Add the high cut button
        highCutButton.setButtonText ("High Cut");
        highCutButton.addListener (this);
        addAndMakeVisible (highCutButton);

        setSize (400, 300);
    }

    ~EQAudioProcessorEditor() {}

    void paint (Graphics& g) override
    {
        // Draw the background
        g.fillAll (Colour (0xff1c1c1c));

        // Draw the labels
        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("Frequency", 30, 50, 100, 20, Justification::centred);
        g.drawText ("Gain", 170, 50, 50, 20, Justification::centred);
        g.drawText ("Q", 290, 50, 50, 20, Justification::centred);
    }

    void resized() override
    {
        // Set the positions and sizes of the controls
        frequencySlider.setBounds (20, 80, 120, 120);
        gainKnob.setBounds (150, 80, 100, 100);
        qKnob.setBounds (270, 80, 100, 100);
        lowCutButton.setBounds (20, 220, 100, 40);
        highCutButton.setBounds (270, 220, 100, 40);
    }

    void sliderValueChanged (Slider* slider) override
    {
        // Update the parameter values when a slider is changed
        if (slider == &frequencySlider)
            processor.setParameterNotifyingHost (EQAudioProcessor::FrequencyParam, (float) frequencySlider.getValue());
        else if (slider == &gainKnob)
            processor.setParameterNotifyingHost (EQ
