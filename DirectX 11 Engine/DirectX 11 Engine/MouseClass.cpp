#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y)
{
	m_leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	m_eventBuffer.push(me);
}

void MouseClass::OnLeftReleased(int x, int y)
{
	m_leftIsDown = false;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(int x, int y)
{
	m_rightIsDown = true;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void MouseClass::OnRightReleased(int x, int y)
{
	m_rightIsDown = false;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnMiddlePressed(int x, int y)
{
	m_mbuttonDown = true;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void MouseClass::OnMiddleReleased(int x, int y)
{
	m_mbuttonDown = false;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void MouseClass::OnWheelUp(int x, int y)
{
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y)
{
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
	m_x = x;
	m_y = y;
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	m_eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

bool MouseClass::IsLeftDown()
{
	return m_leftIsDown;
}

bool MouseClass::IsMiddleDown()
{
	return m_mbuttonDown;
}

bool MouseClass::IsRightDown()
{
	return m_rightIsDown;
}

int MouseClass::GetPosX()
{
	return m_x;
}

int MouseClass::GetPosY()
{
	return m_y;
}

MousePoint MouseClass::GetPos()
{
	return{ m_x, m_y };
}

bool MouseClass::EventBufferIsEmpty()
{
	return m_eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	MouseEvent e = m_eventBuffer.front(); //Get first event from buffer
	m_eventBuffer.pop(); //Remove first event from buffer
	return e;
}