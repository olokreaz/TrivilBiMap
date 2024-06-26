#pragma once
#include <optional>
#include <string_view>
#include <type_traits>

namespace deteil {

	template< typename First, typename Second > struct SwitchTypesDetected final
	{
		using first_type  = First;
		using second_type = Second;
		constexpr SwitchTypesDetected& Case( First, Second ) noexcept { return *this; };
	};

	struct SwitchTypesDetector final
	{
		template< typename First, typename Second > constexpr auto Case( First, Second ) noexcept
		{
			using first_type = std::conditional_t<
				std::is_convertible_v< First, std::string_view >, std::string_view, First >;
			using second_type = std::conditional_t<
				std::is_convertible_v< Second, std::string_view >, std::string_view, Second >;
			return SwitchTypesDetected< first_type, second_type > { };
		}
	};

	template< class First, class Second > class SwitchByFirst final
	{
	public:
		constexpr explicit SwitchByFirst( First search ) noexcept
			: search_ {search} { }

		constexpr SwitchByFirst& Case( First first, Second second ) noexcept
		{
			if ( !result_ && search_ == first ) result_ . emplace( second );
			return *this;
		}

		NODISCARD constexpr std::optional< Second > Extract( ) noexcept { return result_; }

	private
	:
		First                   search_;
		std::optional< Second > result_ { };
	};

	template< class First, class Second > class SwitchBySecond final
	{
	public:
		constexpr explicit SwitchBySecond( Second search ) noexcept
			: search_ {search} { }

		constexpr SwitchBySecond& Case( First first, Second second ) noexcept
		{
			if ( !result_ && search_ == second ) result_ . emplace( first );
			return *this;
		}

		NODISCARD constexpr std::optional< First > Extract( ) noexcept { return result_; }

	private
	:
		Second                 search_;
		std::optional< First > result_ { };
	};

}


	template< class BuilderFunc > class TrivialBiMap final
	{
		using TypesPire = std::invoke_result_t< const BuilderFunc&, deteil::SwitchTypesDetector >;

	public:
		using First  = typename TypesPire::first_type;
		using Second = typename TypesPire::second_type;

		constexpr TrivialBiMap( BuilderFunc builder )
			: func_ {builder} { }

		constexpr std::optional< Second > TryFindByFirst( First value ) const
		{
			return func_( deteil::SwitchByFirst< First, Second > {value} ) . Extract( );
		}

		constexpr std::optional< First > TryFindBySecond( Second value ) const
		{
			return func_( deteil::SwitchBySecond< First, Second > {value} ) . Extract( );
		}

		constexpr auto operator[]( First value ) const { return TryFindByFirst( value ); }
		constexpr auto operator[]( Second value ) const { return TryFindBySecond( value ); }

	private:
		const BuilderFunc func_;
	};

