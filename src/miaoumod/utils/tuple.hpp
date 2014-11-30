#ifndef MIAOUMOD_UTILS_TUPLE_HPP
#define MIAOUMOD_UTILS_TUPLE_HPP

namespace utils
{
    namespace detail
    {
        template<typename Target>
        struct next_target;

        template<typename T0, typename... TX>
        struct next_target<std::tuple<T0, TX...>>
        {
            typedef std::tuple<TX...> Value;
        };

        template<int I, typename Target>
        struct skip_elements;

        template<int I>
        struct skip_elements<I, std::tuple<>>
        {
            template<typename... TX>
            static inline std::tuple<> get(const std::tuple<TX...> & args)
            {
                return std::tuple<>();
            }
        };

        template<int I, typename Target>
        struct skip_elements
        {
            template<typename... TX>
            static inline Target get(const std::tuple<TX...> & args)
            {
                return std::tuple_cat(
                    std::make_tuple(std::get<I>(args)),
                    skip_elements<I+1, typename next_target<Target>::Value>::get(args)
                );
            }
        };
    }

    template <class A, class...ARGS>
    inline std::tuple<ARGS...> get_tail(const std::tuple<A, ARGS...> &t)
    {
        return detail::skip_elements<1, std::tuple<ARGS...>>::get(t);
    }
}
#endif
