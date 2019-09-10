#include <iostream>
#include "objects.hpp"
#include "relations.hpp"

/*
 * questions
 */

template<typename T, typename R>
constexpr bool is_father_v = is_true<father<T,R>>;

template<typename T, typename R>
constexpr bool is_child_v = is_true<father<R,T>>;

template<typename A, typename B>
struct is_grandfather
{
    template<typename T>
    struct predicate
    {
        static constexpr bool value = is_true<father<A,T>> and is_true<father<T,B>>;
    };

    static constexpr bool value = is_any<all_objects, predicate>::value;
};


template<typename T, typename R>
constexpr bool is_grandfather_v = is_grandfather<T, R>::value;

template<typename T, typename R>
constexpr bool is_grandchild_v = is_grandfather<R, T>::value;

int main()
{
    using std::cout;
    using std::endl;

    cout << "father" << endl;
    cout << is_father_v<A,B> << endl;
    cout << is_child_v<B,A> << endl;
    cout << is_father_v<A,C> << endl;
    cout << is_child_v<C,A> << endl;

    cout << "is grandfather" << endl;
    cout << is_grandfather_v<A,D> << endl;
    cout << is_grandchild_v<D,A> << endl;

}
