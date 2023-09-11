#include "MainSynth.h"
#include "MainEditor.h"

//==============================================================================
MainEditor::MainEditor(MainSynth &p)
    : AudioProcessorEditor(&p), processorRef(p)
{

    // MainEditor::processorRef = p;
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // MainComponent::graph = GraphComponent(expression);
    juce::String expr = "sin(440 * 2 * 3.14 * x)";

    addAndMakeVisible(graph);

    addAndMakeVisible(expressionInput);
    expressionInput.setEditable(true);
    expressionInput.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);
    expressionInput.onTextChange = [this]
    {
        setExpressionText(expressionInput.getText());
    };
    expressionInput.setText(expr, juce::dontSendNotification);

    addAndMakeVisible(yEqualsLabel);
    yEqualsLabel.attachToComponent(&expressionInput, true);
    yEqualsLabel.setText("y = ", juce::dontSendNotification);

    addAndMakeVisible(errorLabel);
    errorLabel.setColour(juce::Label::textColourId, juce::Colours::red);

    expressionInput.setJustificationType(juce::Justification::bottom);

    setExpressionText(expr);

    setSize(600, 600);
}

MainEditor::~MainEditor()
{
}

//==============================================================================
void MainEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MainEditor::resized()
{
    errorLabel.setBounds(50, 450, getWidth() - 110, 20);
    expressionInput.setBounds(50, 500, getWidth() - 110, 20);
}

void MainEditor::setExpressionText(juce::String expr)
{
        
    errorLabel.setText("", juce::dontSendNotification);
    std::cout << "setting expr text" << std::endl;
    std::string s = expr.toStdString();
    Parser p = Parser(s);

    try
    {
        MainEditor::expression = p.parseExpression();

        MainEditor::graph.setExpression(MainEditor::expression);
        MainEditor::processorRef.setExpression(MainEditor::expression);
    }
    catch(EquationException e){
        errorLabel.setText(e.what(), juce::dontSendNotification);
    }
}