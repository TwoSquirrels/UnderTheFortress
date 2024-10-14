# include "UtFInput.hpp"

void UtFInput::update()
{
	for (const Input input : Keyboard::GetAllInputs())
	{
		m_pressed[input.asUint32()] |= input.pressed();
		m_down[input.asUint32()] |= input.down();
	}
	for (const Input input : Mouse::GetAllInputs())
	{
		m_pressed[input.asUint32()] |= input.pressed();
		m_down[input.asUint32()] |= input.down();
	}
}

void UtFInput::reset()
{
	m_pressed.clear();
	m_down.clear();
}

bool UtFInput::pressed(const uint32 key) const
{
	return m_pressed.contains(key) && m_pressed.at(key);
}

bool UtFInput::pressed(const Input input) const
{
	return pressed(input.asUint32());
}

bool UtFInput::pressed(const InputCombination& inputCombination) const
{
	return pressed(inputCombination.input1()) && pressed(inputCombination.input2());
}

bool UtFInput::pressed(const InputGroup& inputGroup) const
{
	return inputGroup.inputs().any([this](const Input input) { return pressed(input); }) ||
		inputGroup.inputCombinations().any([this](const InputCombination& inputCombination) { return pressed(inputCombination); });
}

bool UtFInput::down(const uint32 key) const
{
	return m_down.contains(key) && m_down.at(key);
}

bool UtFInput::down(const Input input) const
{
	return down(input.asUint32());
}

bool UtFInput::down(const InputCombination& inputCombination) const
{
	return pressed(inputCombination.input1()) && down(inputCombination.input2()) ||
		down(inputCombination.input1()) && pressed(inputCombination.input2());
}

bool UtFInput::down(const InputGroup& inputGroup) const
{
	return inputGroup.inputs().any([this](const Input input) { return down(input); }) ||
		inputGroup.inputCombinations().any([this](const InputCombination& inputCombination) { return down(inputCombination); });
}
