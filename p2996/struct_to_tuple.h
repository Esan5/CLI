#include <experimental/meta>

#include <algorithm>
#include <ranges>
#include <tuple>
#include <vector>

namespace cli::p2996 {

consteval auto type_struct_to_tuple(std::meta::info type) -> std::meta::info {
  return std::meta::substitute(^^std::tuple,
                    std::meta::nonstatic_data_members_of(type)
                    | std::views::transform(std::meta::type_of)
                    | std::views::transform(std::meta::remove_cvref)
                    | std::ranges::to<std::vector>());
}

template <typename To, typename From, std::meta::info ... members>
constexpr auto struct_to_tuple_helper(From const& from) -> To {
  return To(from.[: members :]...);
}

template<typename From>
consteval auto get_struct_to_tuple_helper() {
  using To = [: type_struct_to_tuple(^^From) :];

  std::vector<std::meta::info> args = { ^^To, ^^From };
  for (auto e : std::meta::nonstatic_data_members_of(^^From)) {
      args.emplace_back(std::meta::reflect_value(e));
  }

  return extract<To(*)(From const&)>(
    std::meta::substitute(^^struct_to_tuple_helper, args));
}

template <typename From>
constexpr auto struct_to_tuple(From const& from) {
  return get_struct_to_tuple_helper<From>()(from);
}

} // namespace cli::p2996
