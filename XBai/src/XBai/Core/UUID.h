#ifndef XB_CORE_UUID_H
#define XB_CORE_UUID_H

#include <xhash>

namespace XBai
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;
		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<XBai::UUID>
	{
		std::size_t operator()(const XBai::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}

#endif
