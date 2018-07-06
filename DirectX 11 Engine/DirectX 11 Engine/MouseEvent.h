#pragma once
#include <utility>

struct MousePoint
{
	int m_x;
	int m_y;
	MousePoint& operator -(const MousePoint& a)
	{
		MousePoint diff;
		diff.m_x = this->m_x - a.m_x;
		diff.m_y = this->m_y - a.m_y;
		return diff;
	}
};

class MouseEvent
{
public:
	enum EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid
	};
private:
	EventType m_type;
	int m_x;
	int m_y;
public:
	MouseEvent();
	MouseEvent(const EventType type, const int x, const int y);
	bool IsValid() const;
	EventType GetType() const;
	MousePoint GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
};