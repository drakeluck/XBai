#ifndef XB_TIMESTEP_H
#define XB_TIMESTEP_H

namespace XBai
{
	class TimeStep
	{
	public:
		TimeStep(float time = 0.0f)
			:m_Time(time) { }

		operator float() const { return m_Time; }
		//返回 秒
		float GetSeconds() const { return m_Time; }
		//返回 毫秒
		float GetMillseconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time = 0.0f;//以 秒 为单位
	};


}

#endif

