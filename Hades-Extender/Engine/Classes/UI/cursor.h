#pragma once

namespace Engine
{
	class Cursor
	{
	public:
		float zero_x;
		float zero_y;
		bool can_make_current;
		bool added;
		bool force_use_check;
		int previous_use_target_id;
		float prev_lerp_amount;
		float pos_x;
		float pos_y;
		float prev_pos_x;
		float prev_pos_y;
		int type;
	public:
		static Cursor* Instance()
		{
			PVOID cursor_ptr = (PVOID)Engine::Addresses::CursorManager::instance;
			return *(Cursor**)(cursor_ptr);
		}
	};
}