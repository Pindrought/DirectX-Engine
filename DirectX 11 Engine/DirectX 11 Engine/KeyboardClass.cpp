#include "KeyboardClass.h"

KeyboardClass::KeyboardClass()
{
	for (int i = 0; i < 256; i++)
		m_keyStates[i] = false; //Initialize all key states to off (false)
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode)
{
	return m_keyStates[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
	return m_keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
	return m_charBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
	if (m_keyBuffer.empty()) //If no keys to be read?
	{
		return KeyboardEvent(); //return empty keyboard event
	}
	else
	{
		KeyboardEvent e = m_keyBuffer.front(); //Get first Keyboard Event from queue
		m_keyBuffer.pop(); //Remove first item from queue
		return e; //Returns keyboard event
	}
}

unsigned char KeyboardClass::ReadChar()
{
	if (m_charBuffer.empty()) //If no keys to be read?
	{
		return 0u; //return 0 (NULL char)
	}
	else
	{
		unsigned char e = m_charBuffer.front(); //Get first char from queue
		m_charBuffer.pop(); //Remove first char from queue
		return e; //Returns char
	}
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
	m_keyStates[key] = true;
	m_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
	m_keyStates[key] = false;
	m_keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
	m_charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
	m_autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
	m_autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
	m_autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
	m_autoRepeatChars = false;
}

bool KeyboardClass::IsKeysAutoRepeat()
{
	return m_autoRepeatKeys;
}

bool KeyboardClass::IsCharsAutoRepeat()
{
	return m_autoRepeatChars;
}