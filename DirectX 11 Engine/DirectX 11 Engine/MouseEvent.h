#pragma once
#include <utility>

struct MousePoint
{
	int x;
	int y;
	MousePoint& operator -(const MousePoint& a)
	{
		MousePoint diff;
		diff.x = x - a.x;
		diff.y = y - a.y;
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
	EventType type;
	int x;
	int y;
public:
	MouseEvent();
	MouseEvent(const EventType type, const int x, const int y);
	bool IsValid() const;
	EventType GetType() const;
	MousePoint GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
};