# pragma once

# include <Siv3D.hpp>

class UtFInput
{
public:
	/// @brief Add the input state.
	/// @remark This function should be called at the beginning of the Siv3D update.
	void update();

	/// @brief Reset the input state.
	/// @remark This function should be called at the end of the game update.
	void reset();

	[[nodiscard]] bool pressed(uint32 key) const;

	[[nodiscard]] bool pressed(Input input) const;

	[[nodiscard]] bool pressed(const InputCombination& inputCombination) const;

	[[nodiscard]] bool pressed(const InputGroup& inputGroup) const;

	[[nodiscard]] bool down(uint32 key) const;

	[[nodiscard]] bool down(Input input) const;

	[[nodiscard]] bool down(const InputCombination& inputCombination) const;

	[[nodiscard]] bool down(const InputGroup& inputGroup) const;

private:
	OrderedTable<uint32, bool> m_pressed, m_down;
};
