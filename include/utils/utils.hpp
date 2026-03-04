#pragma once
#include <array>
#include <bit>
#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>

namespace net
{
	enum class byte_order
	{
		little,
		big
	};
} // namespace net
namespace utils
{

	constexpr inline bool is_little_endian() noexcept
	{
		uint16_t num = 0x01;
		auto bytes = std::bit_cast<std::array<uint8_t, sizeof(uint16_t)>>(num);
		return bytes.front() == 1;
	}

	template <typename T>
	concept byte_swappable = (std::is_integral_v<T> && !std::is_same_v<T, bool> && sizeof(T) > 1);

	template <byte_swappable T>
	constexpr inline T byte_swap(T value) noexcept
	{
		auto bytes = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
		size_t size = sizeof(T);
		for (size_t i = 0; i < size / 2; i++)
		{
			std::swap(bytes[i], bytes[size - i - 1]);
		}
		return std::bit_cast<T>(bytes);
	}

	template <byte_swappable T>
	constexpr inline T order_bytes(T value, net::byte_order order) noexcept
	{
		auto system_order = (is_little_endian() ? net::byte_order::little : net::byte_order::big);

		if (system_order == order)
		{
			return value;
		}

		return byte_swap<T>(value);
	}

	bool is_valid_ipv4(const std::string& ip);
	bool is_valid_port(const int port_no);
	std::string_view trim(std::string_view str);
} // namespace utils