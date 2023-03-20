class CompressorPluginAudioProcessorEditor  : public AudioProcessorEditor,
                                               private Slider::Listener
{
public:
    CompressorPluginAudioProcessorEditor (CompressorPluginAudioProcessor& p)
        : AudioProcessorEditor (&p), processor (p)
    {
        // Set the size of the window
        setSize (400, 300);
        
        // Create the sliders
        addAndMakeVisible (thresholdSlider);
        thresholdSlider.setSliderStyle (Slider::LinearBar);
        thresholdSlider.setRange (0.0, 1.0);
        thresholdSlider.setValue (*processor.thresholdParam);
        thresholdSlider.addListener (this);
        
        addAndMakeVisible (ratioSlider);
        ratioSlider.setSliderStyle (Slider::LinearBar);
        ratioSlider.setRange (1.0, 10.0);
        ratioSlider.setValue (*processor.ratioParam);
        ratioSlider.addListener (this);
        
        addAndMakeVisible (attackSlider);
        attackSlider.setSliderStyle (Slider::LinearBar);
        attackSlider.setRange (0.0, 1.0);
        attackSlider.setValue (*processor.attackParam);
        attackSlider.addListener (this);
        
        addAndMakeVisible (releaseSlider);
        releaseSlider.setSliderStyle (Slider::LinearBar);
        releaseSlider.setRange (0.0, 1.0);
        releaseSlider.setValue (*processor.releaseParam);
        releaseSlider.addListener (this);
        
        addAndMakeVisible (gainSlider);
        gainSlider.setSliderStyle (Slider::LinearBar);
        gainSlider.setRange (-12.0, 12.0);
        gainSlider.setValue (*processor.gainParam);
        gainSlider.addListener (this);
        
        // Set the layout of the sliders
        int x = 10;
        int y = 10;
        int w = 380;
        int h = 30;
        thresholdSlider.setBounds (x, y, w, h);
        y += h + 10;
        ratioSlider.setBounds (x, y, w, h);
        y += h + 10;
        attackSlider.setBounds (x, y, w, h);
        y += h + 10;
        releaseSlider.setBounds (x, y, w, h);
        y += h + 10;
        gainSlider.setBounds (x, y, w, h);
    }

    ~CompressorPluginAudioProcessorEditor() {}

    void paint (Graphics&) override {}
    void resized() override {}

private:
    CompressorPluginAudioProcessor& processor;
    Slider thresholdSlider;
    Slider ratioSlider;
    Slider attackSlider;
    Slider releaseSlider;
    Slider gainSlider;

    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &thresholdSlider)
            *processor.thresholdParam = thresholdSlider.getValue();
        else if (slider == &ratioSlider)
            *processor.ratioParam = ratioSlider.getValue();
        else if (slider == &attackSlider)
            *processor.attackParam = attackSlider.getValue();
        else if (slider == &releaseSlider)
            *processor.releaseParam = releaseSlider.getValue();
        else if (slider == &gainSlider)
            *processor.gainParam = gainSlider.getValue();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorPluginAudioProcessorEditor)
};
