#pragma once

#ifndef _LINQ_H
#define _LINQ_H

#ifndef _VECTOR_
#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif
#endif
#ifndef _UNORDERED_MAP_
#ifndef _GLIBCXX_UNORDERED_MAP
#include <unordered_map>
#endif
#endif
#ifndef _FUNCTIONAL_
#ifndef _GLIBCXX_FUNCTIONAL
#include <functional>
#endif
#endif
#ifndef _ALGORITHM_
#ifndef _GLIBCXX_ALGORITHM
#include <algorithm>
#endif
#endif
#ifndef _CSTRING_
#ifndef _GLIBCXX_CSTRING
#include <cstring>
#endif
#endif

#include <exception>
#include <stdexcept>

namespace linq {
	namespace core {
		// TEMPLATE STRUCT more
		template<class _Ty = void>
		struct more
		{	// functor for operator<
			typedef _Ty first_argument_type;
			typedef _Ty second_argument_type;
			typedef bool result_type;

			/// <summary>
			/// Performs a comparison of the two objects to see if the left is greater than the right.
			/// </summary>
			/// <param name="_Left">Left of the greater comparison.</param>
			/// <param name="_Right">Right of the greater comparison.</param>
			/// <returns>True if the <paramref name="_Left"/> is greater than the <paramref name="_Right"/>; otherwise false.</returns>
			constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
			{	// apply operator> to operands
				return (_Left > _Right);
			}
		};
		// TEMPLATE STRUCT SPECIALIZATION more
		template<>
		struct more<void>
		{	// transparent functor for operator<
			typedef int is_transparent;

			/// <summary>
			/// Performs a generic comparison of the two objects to see if the left is greater than the right.
			/// </summary>
			/// <param name="_Left">Left of the greater comparison.</param>
			/// <param name="_Right">Right of the greater comparison.</param>
			/// <returns>True if the <paramref name="_Left"/> is greater than the <paramref name="_Right"/>; otherwise false.</returns>
			template<class _Ty1, class _Ty2>
			constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
				-> decltype(static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right))
			{	// transparently apply operator> to operands
				return (static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right));
			}
		};
		// TEMPLATE STRUCT less
		template<class _Ty = void>
		struct less
		{	// functor for operator<
			typedef _Ty first_argument_type;
			typedef _Ty second_argument_type;
			typedef bool result_type;

			/// <summary>
			/// Performs a comparison of the two objects to see if the left is lesser than the right.
			/// </summary>
			/// <param name="_Left">Left of the lesser comparison.</param>
			/// <param name="_Right">Right of the lesser comparison.</param>
			/// <returns>True if the <paramref name="_Left"/> is lesser than the <paramref name="_Right"/>; otherwise false.</returns>
			constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
			{	// apply operator< to operands
				return (_Left < _Right);
			}
		};
		// TEMPLATE STRUCT SPECIALIZATION less
		template<>
		struct less<void>
		{	// transparent functor for operator<
			typedef int is_transparent;

			/// <summary>
			/// Performs a generic comparison of the two objects to see if the left is lesser than the right.
			/// </summary>
			/// <param name="_Left">Left of the lesser comparison.</param>
			/// <param name="_Right">Right of the lesser comparison.</param>
			/// <returns>True if the <paramref name="_Left"/> is lesser than the <paramref name="_Right"/>; otherwise false.</returns>
			template<class _Ty1, class _Ty2>
			constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
				-> decltype(static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right))
			{	// transparently apply operator< to operands
				return (static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right));
			}
		};

		/// <summary>
		/// Basic lightweight structure which holds two objects of differing types.
		/// </summary>
		/// <typeparam name="_Left">The first object</typeparam>
		/// <typeparam name="_Right">The second object</typeparam>
		template<class _Left, class _Right>
		struct merge_pair {
			_Left left;
			_Right right;
		};
		template<class _Key, class _Value>
		struct key_value_pair {
			_Key key;
			_Value value;
		};
	}

	/// <summary>predicate used for sorting objects in ascending order</summary>
	constexpr core::less<> ascending() { return core::less<>(); }
	/// <summary>predicate used for sorting objects in descending order</summary>
	constexpr core::more<> descending() { return core::more<>(); }


	/// <summary>
	/// Extension to the standard std::vector class. Provides specialized query methods for processing lists.
	/// </summary>
	/// <typeparam name="_Ty">Type for the elements to be held</typeparam>
	template<class _Ty>
	class linq_vec : public ::std::vector<_Ty> {
	public:
#ifdef _VECTOR_
		linq_vec() : vector() {}
		linq_vec(const ::std::size_t &_Count) : vector(_Count) {}
		linq_vec(const ::std::vector<_Ty>&_Vec) : vector(_Vec) {}
		linq_vec(::std::vector<_Ty>&& _Right) : vector(_Right) {}
		linq_vec(::std::initializer_list<_Ty> _Ilist) : vector(_Ilist) {}
		template<class _Iter, class = ::std::enable_if_t<::std::_Is_iterator<_Iter>::value>>
		linq_vec(_Iter _First, _Iter _Last) : vector(_First, _Last) {}
#elif _STL_VECTOR_H
		linq_vec() : ::std::vector<_Ty>() {}
		linq_vec(::std::size_t __n) : ::std::vector<_Ty>(__n) {}
		linq_vec(const ::std::vector<_Ty>&__x) : ::std::vector<_Ty>(__x) {}
		linq_vec(::std::vector<_Ty>&& __x) : ::std::vector<_Ty>(__x) {}
		linq_vec(::std::initializer_list<_Ty> __l) : ::std::vector<_Ty>(__l) {}
		template<typename _InputIterator, typename = std::_RequireInputIter<_InputIterator>>
		linq_vec(_InputIterator __first, _InputIterator __last) : ::std::vector<_Ty>(__first, __last) {}
#endif

	public:
		/// <summary>
		/// Delegate used for transforming the provided data into a new state which is returned by the call.
		/// </summary>
		template<class _Ret>
		using conversion = ::std::function<_Ret(const _Ty&)>;
		/// <summary>
		/// Delegate used for transforming the provided data into a new state which is returned by the call.
		/// </summary>
		template<class _Val, class _Ret>
		using selector = ::std::function<_Ret(const _Val&)>;
		/// <summary>
		/// Delegate for applying conditions on the provided item and returning a bool result.
		/// </summary>
		typedef ::std::function<bool(const _Ty&)> conditional;
		/// <summary>
		/// Delegate for comparing two items and returning a bool result.
		/// </summary>
		template<class _Ty1, class _Ty2>
		using comparison = ::std::function<bool(const _Ty1&, const _Ty2&)>;
		/// <summary>
		/// Delegate that is expected to merge two items into a new item which is then returned by the call.
		/// </summary>
		template<class _Ty2, class _Ret>
		using merger = ::std::function<_Ret(const _Ty&, const _Ty2&)>;

	public:
		/// <summary>
		/// Performs an item selection which is expected to transform the data in some way and return a new array.
		/// </summary>
		/// <param name="selector">Lambda which defines how each item is transformed into the new type.</param>
		/// <param name="result">The array to be filled with the new items.</param>
		/// <typeparam name="_Ret">The new type being created and returned for the new array.</typeparam>
		template<class _Ret>
		void select(const conversion<_Ret> &selector, linq_vec<_Ret> &result) const {
			result.resize(this->size());
			for (::std::size_t c = 0, l = this->size(); c < l; c++) {
				result[c] = selector((*this)[c]);
			}
		}

		/// <summary>
		/// Performs an item selection which is expected to transform the data in some way and return a new array.
		/// </summary>
		/// <param name="selector">Lambda which defines how each item is transformed into the new type.</param>
		/// <typeparam name="_Ret">The new type being created and returned for the new array.</typeparam>
		/// <returns>The new array of transformed items.</returns>
		template<class _Ret>
		inline linq_vec<_Ret> select(const conversion<_Ret> &selector) const {
			linq_vec<_Ret> result(this->size());
			select(selector, result);
			return result;
		}

		/// <summary>
		/// Performs a conditional for filtering the list and returns a new list containing those filtered items.
		/// </summary>
		/// <param name="conditional">The lambda which determines if an item is to be added to the new list.</param>
		/// <returns>The new list of filtered items.</returns>
		linq_vec<_Ty> where(const conditional &condition) const {
			// Record of elements to keep
			::std::vector<size_t> keep;
			// Loop through elements to see which are being kept and which are to be thrown
			for (::std::size_t c = 0, l = this->size(); c < l; c++) {
				// If we are to keep the element
				if (condition((*this)[c])) {
					// Add the element's index to the 'keep' list
					keep.push_back(c);
				}
			}
			linq_vec<_Ty> result(keep.size());
			// We will now take each element we are keeping and move them into sequential
			// order at the begining of our original list
			for (::std::size_t c = 0, l = keep.size(); c < l; c++) {
				result[c] = this->operator[](keep[c]);
			}
			// trim the tail off of our original array
			return result;
		}

		/// <summary>
		/// Performs a sort on this list.
		/// </summary>
		/// <param name="pred">Predicate used to determine if the left element should go before the right.</param>
		/// <typeparam name="_Pr">The type of the predicate object, method or lambda</typeparam>
		/// <returns>A reference to this list (used for chaining calls).</returns>
		template<class _Pred>
		inline linq_vec<_Ty>& orderby(const _Pred &pred) {
			::std::sort(this->begin(), this->end(), pred);
			return *this;
		}

		/// <summary>
		/// Performs a join on the current list and the provided list and performs a merge of the paired items.
		/// </summary>
		/// <param name="arr">Array to be joined with this one.</param>
		/// <param name="merge">Lambda that performs the merge.</param>
		/// <param name="on">Conditional lambda that determines if two elements should be paired.</param>
		/// <typeparam name="_Ty2">The type contained in the array being joined.</typeparam>
		/// <typeparam name="_Ret">The type to be returned in the new merged array.</typeparam>
		/// <returns>New array of merged items.</returns>
		template<class _Ty2, class _Ret>
		linq_vec<_Ret> join(const linq_vec<_Ty2> &arr, const merger<_Ty2, _Ret> &merge, const comparison<_Ty, _Ty2> &on) const {
			linq_vec<_Ret> merged;
			for (auto first : *this) {
				for (auto second : arr) {
					if (on(first, second)) {
						merged.push_back(merge(first, second));
					}
				}
			}
			return merged;
		}
		/// <summary>
		/// Performs a join on the current list and the provided list and pairs the items into <see cref="linq::core::merge_pair"/>.
		/// </summary>
		/// <param name="arr">Array to be joined with this one.</param>
		/// <param name="on">Conditional lambda that determines if two elements should be paired.</param>
		/// <typeparam name="_Ty2">The type contained in the array being joined.</typeparam>
		/// <returns>New array of paired items.</returns>
		template<class _Ty2>
		inline linq_vec<core::merge_pair<_Ty, _Ty2>> join(const linq_vec<_Ty2> &arr, const comparison<_Ty, _Ty2> &on) const {
			return join<_Ty2, core::merge_pair<_Ty, _Ty2>>(arr, [](auto left, auto right)->core::merge_pair<_Ty, _Ty2> { return { left, right }; }, on);
		}

		/// <summary>
		/// Returns the first element in the array or throws <see cref="std::logic_error"/> if the array is empty.
		/// </summary>
		/// <returns>The first element of the array; otherwise an error is thrown.</returns>
		_Ty first() const {
			if (this->size() > 0) {
				return *(this->begin());
			}
			throw ::std::logic_error("array is empty");
		}
		/// <summary>
		/// Searches the array for the first item which satisfies the provided condition.
		/// Throws <see cref="std::logic_error"/> if the condition is never satisfied.
		/// </summary>
		/// <param name="condition">Condition to be satisfied.</param>
		/// <exception cref="std::logic_error">Thrown if no items satisfy the condition.</exception>
		/// <returns>The first item found which satisfies the condition; otherwise an error is thrown.</returns>
		_Ty first(const conditional &condition) const {
			for (auto it = this->begin(), end = this->end(); it != end; it++) {
				if (condition(*it)) {
					return *it;
				}
			}
			throw ::std::logic_error("no elements match the given conditional");
		}
		/// <summary>
		/// Searches the array for the first item which satisfies the provided condition.
		/// Returns the provided <paramref name="defaultValue"/> if the condition is never satisfied.
		/// </summary>
		/// <param name="condition">Condition to be satisfied.</param>
		/// <param name="defaultValue">Default value to be returned if no items satisfy the condition.</param>
		/// <returns>The first item found which satisfies the condition; otherwise the provided <paramref name="defaultValue"/>.</returns>
		inline _Ty first_or_default(_Ty defaultValue, const conditional &condition) const {
			try { return first(condition); }
			catch (...) { return defaultValue; }
		}

		/// <summary>
		/// Returns the last element in the array or throws <see cref="std::logic_error"/> if the array is empty.
		/// </summary>
		/// <returns>The last element of the array; otherwise an error is thrown.</returns>
		_Ty last() const {
			size_t pos = this->size() - 1;
			if (pos >= 0) {
				return this->at(pos);
			}
			throw ::std::logic_error("array is empty");
		}
		/// <summary>
		/// Searches the array for the last item which satisfies the provided condition.
		/// Throws <see cref="std::logic_error"/> if the condition is never satisfied.
		/// </summary>
		/// <param name="condition">Condition to be satisfied.</param>
		/// <exception cref="std::logic_error">Thrown if no items satisfy the condition.</exception>
		/// <returns>The last item found which satisfies the condition; otherwise an error is thrown.</returns>
		_Ty last(const conditional &condition) const {
			for (auto it = this->rbegin(), end = this->rend(); it != end; it++) {
				if (condition(*it)) {
					return *it;
				}
			}
			throw ::std::logic_error("no elements match the given conditional");
		}
		/// <summary>
		/// Searches the array for the last item which satisfies the provided condition.
		/// Returns the provided <paramref name="defaultValue"/> if the condition is never satisfied.
		/// </summary>
		/// <param name="condition">Condition to be satisfied.</param>
		/// <param name="defaultValue">Default value to be returned if no items satisfy the condition.</param>
		/// <returns>The last item found which satisfies the condition; otherwise the provided <paramref name="defaultValue"/>.</returns>
		_Ty last_or_default(_Ty defaultValue, const conditional &condition) const {
			try { return last(condition); }
			catch (...) { return defaultValue; }
		}

		/// <summary>
		/// Determines if the array contains any elements.
		/// </summary>
		/// <returns>true if the array contains elements; otherwise false.</returns>
		inline bool any() const {
			return this->size() > 0;
		}
		/// <summary>
		/// Determines if any elements in the array satisfy a condition.
		/// </summary>
		/// <param name="conditional">The conditional expression to be satisfied.</param>
		/// <returns>true if an element satisfies the condition; otherwise false.</returns>
		bool any(const conditional &condition) const {
			for (auto it = this->begin(), end = this->end(); it != end; it++) {
				if (condition(*it)) {
					return true;
				}
			}
			return false;
		}

		/// <summary>
		/// Performs a summation on the elements by adding the result of each call to <paramref name="cumulator"/>.
		/// </summary>
		/// <param name="cumulator">Expression which provides a value from the given element, to be added to the final result.</param>
		/// <typeparam name="_Ret">The type of value being added together.</typeparam>
		/// <returns>The summed total of all the elements.</returns>
		template<class _Ret>
		_Ret sum(const selector<_Ty, _Ret> &value_selector) const {
			_Ret result;
			memset(&result, 0, sizeof(_Ret));
			for (auto it = this->begin(), end = this->end(); it != end; it++) {
				result += value_selector(*it);
			}
			return result;
		}

		/// <summary>
		/// Calculates the minimum of the selected values.
		/// </summary>
		/// <param name="value_selector">Used to select the value for the given element</param>
		/// <returns>The smallest value found.</returns>
		inline _Ty min() const { return comp_select<_Ty>([](_Ty x) { return x; }, ::linq::ascending()); }
		/// <summary>
		/// Calculates the minimum of the selected values.
		/// </summary>
		/// <param name="value_selector">Used to select the value for the given element</param>
		/// <typeparam name="_Ret">Value type to be compared for the smallest.</typeparam>
		/// <returns>The smallest value found.</returns>
		template<class _Ret>
		inline _Ret min(const selector<_Ty, _Ret> &value_selector) const { return comp_select(value_selector, ::linq::ascending()); }
		/// <summary>
		/// Calculates the minimum of the selected values.
		/// </summary>
		/// <param name="value_selector">Used to select the value for the given element</param>
		/// <returns>The smallest value found.</returns>
		inline _Ty max() const { return comp_select<_Ty>([](_Ty x) { return x; }, ::linq::descending()); }
		/// <summary>
		/// Calculates the maximum of the selected values.
		/// </summary>
		/// <param name="value_selector">Used to select the value for the given element</param>
		/// <typeparam name="_Ret">Value type to be compared for the largest.</typeparam>
		/// <returns>The largest value found.</returns>
		template<class _Ret>
		inline _Ret max(const selector<_Ty, _Ret> &value_selector) const { return comp_select(value_selector, ::linq::descending()); }
		/// <summary>
		/// Select values to be compared. The value selected most is returned.
		/// </summary>
		/// <param name="value_selector">Used to select the value on each element for comparison.</param>
		/// <param name="pred">Predicate used to compare the values. Returns true if the left is selected over the right.</param>
		/// <typeparam name="_Ret">Value being compared and returned.</typeparam>
		/// <typeparam name="_Pred">Predicate to perform a comparison between the left and right arguments. Returns true if left is selected over the right.</typeparam>
		/// <returns>Value that is selected above all others.</returns>
		template<class _Ret, class _Pred>
		_Ret comp_select(const selector<_Ty, _Ret> & value_selector, const _Pred &pred) const {
			_Ret result, tmp;
			memset(&result, 0, sizeof(_Ret));
			size_t c = 1, size = this->size();
			if (size > 0) result = value_selector(this->at(0));
			else return result;
			for (; c < size; c++) {
				tmp = value_selector(this->at(c));
				if (pred(tmp, result)) {
					result = tmp;
				}
			}
			return result;
		}

		/// <summary>
		/// Reverses the order of the elements in the array.
		/// </summary>
		/// <returns>Self reference.</returns>
		linq_vec<_Ty> &reverse() {
			_Ty tmp;
			size_t c = 0, size = this->size(), half;
			_Ty *data = this->data();
			for (half = size >> 1, size--; c < size; c++) {
				tmp = data[c];
				data[c] = data[size - c];
				data[size - c] = tmp;
			}
			return *this;
		}

		/// <summary>
		/// Helper method which converts this <see cref="linq::linq_vec"/> into a <see cref="std::unordered_map"/> using the
		/// provided <paramref name="key_selector"/> function to select the key to be used. Value becomes the original object.
		/// </summary>
		/// <param name="key_selector">Selector delegate for selecting the key value for each element.</param>
		/// <returns><see cref="std::unordered_map"/> containing the elements using the selected keys.</returns>
		template<class _Key>
		::std::unordered_map<_Key, _Ty> map(const selector<_Ty, _Key> &key_selector) const {
			::std::unordered_map<_Key, _Ty> map;
			for (auto it = this->begin(), end = this->end(); it != end; it++) {
				map[key_selector(*it)] = *it;
			}
			return map;
		}

		/// <summary>
		/// Helper method which converts this <see cref="linq::linq_vec"/> into a <see cref="std::unordered_map"/> using the
		/// provided <paramref name="key_selector"/> function to select the key to be used, and the <paramref name="value_selector"/>
		/// to select the value.
		/// </summary>
		/// <param name="key_selector">Selector delegate for selecting the key value for each element.</param>
		/// <returns><see cref="std::unordered_map"/> containing the elements using the selected keys.</returns>
		template<class _Key, class _Value>
		::std::unordered_map<_Key, _Value> map(const selector<_Ty, _Key> &key_selector, const selector<_Ty, _Value> &value_selector) const {
			::std::unordered_map<_Key, _Value> map;
			for (auto it = this->begin(), end = this->end(); it != end; it++) {
				map[key_selector(*it)] = value_selector(*it);
			}
			return map;
		}

		/// <summary>
		/// Helper method which converts this <see cref="linq::linq_vec"/> into a grouped <see cref="std::unordered_map"/> using the
		/// provided <paramref name="key_selector"/> function to select the group key to be used.
		/// </summary>
		/// <param name="key_selector">Selector delegate for selecting the group key value for each element.</param>
		/// <returns><see cref="std::unordered_map"/> containing the elements using the selected group keys.</returns>
		template<class _Key>
		::std::unordered_map<_Key, ::std::vector<_Ty>> group_by(const selector<_Ty, _Key> &key_selector) const {
			::std::unordered_map<_Key, ::std::vector<_Ty>> map;
			for (auto it = this->begin(), end = this->end(); it != end; it++) {
				map[key_selector(*it)].push_back(*it);
			}
			return map;
		}
		/// <summary>
		/// Helper method which converts this <see cref="linq::linq_vec"/> into a grouped <see cref="std::unordered_map"/> using the
		/// provided <paramref name="key_selector"/> function to select the group key to be used, and the <paramref name="value_selector"/>
		/// to select the value.
		/// </summary>
		/// <param name="key_selector">Selector delegate for selecting the group key value for each element.</param>
		/// <returns><see cref="std::unordered_map"/> containing the elements using the selected group keys.</returns>
		template<class _Key, class _Value>
		::std::unordered_map<_Key, ::std::vector<_Value>> group_by(const selector<_Ty, _Key> &key_selector, const selector<_Ty, _Value> &value_selector) const {
			::std::unordered_map<_Key, ::std::vector<_Value>> map;
			for (auto it = this->begin(), end = this->end(); it != end; it++) {
				map[key_selector(*it)].push_back(value_selector(*it));
			}
			return map;
		}

		/// <summary>
		/// Helper method which converts this <see cref="linq::linq_vec"/> into a <see cref="std::vector"/>.
		/// </summary>
		/// <returns><see cref="std::vector"/> containing a copy of the elements of this <see cref="linq::linq_vec"/>.</returns>
		inline ::std::vector<_Ty> to_vector() const {
			return ::std::vector<_Ty>(*this);
		}
	};

	/// <summary>
	/// Helper function for converting a <see cref="std::vector"/> to an <see cref="linq::linq_vec"/>.
	/// </summary>
	/// <param name="vec">The <see cref="std::vector"/> to be converted to a <see cref="linq::linq_vec"/>.</param>
	/// <returns><see cref="linq::linq_vec"/> containing a copy of the elements from the provided <paramref name="vec"/>.</returns>
	template<class _Ty>
	inline linq_vec<_Ty> from(const ::std::initializer_list<_Ty> &ilist) { return linq_vec<_Ty>(ilist); }
	/// <summary>
	/// Helper function for converting a <see cref="std::vector"/> to an <see cref="linq::linq_vec"/>.
	/// </summary>
	/// <param name="vec">The <see cref="std::vector"/> to be converted to a <see cref="linq::linq_vec"/>.</param>
	/// <returns><see cref="linq::linq_vec"/> containing a copy of the elements from the provided <paramref name="vec"/>.</returns>
	template<class _Ty>
	inline linq_vec<_Ty> from(const ::std::vector<_Ty> &vec) { return linq_vec<_Ty>(vec); }
	/// <summary>
	/// Helper function for converting a C-style array pointer into an <see cref="linq::linq_vec"/>.
	/// </summary>
	/// <param name="c_arr">The pointer to a C-Style array to be converted to a <see cref="linq::linq_vec"/>.</param>
	/// <param name="size">The size of the C-Style array.</param>
	/// <returns><see cref="linq::linq_vec"/> containing a copy of the elements from the provided <paramref name="c_arr"/>.</returns>
	template<class _Ty>
	inline linq_vec<_Ty> from(const _Ty *c_arr, const size_t &size) {
		return linq_vec<_Ty>(c_arr, c_arr + size);
	}
}


#ifdef LINQ_USE_MACROS
#ifndef FROM
#include "linq-macros.hpp"
namespace linq {
	/// <summary>
	/// Pass-through function to make things uniform with the <see cref="FROM"/> macro.
	/// </summary>
	/// <param name="arr">The array to be passed through as a result.</param>
	/// <returns>The same provided <see cref="arr"/>.</returns>
	template<class _Ty>
	constexpr inline linq_vec<_Ty> from(const linq_vec<_Ty> &arr) { return arr; }
}
#endif
#endif

#endif