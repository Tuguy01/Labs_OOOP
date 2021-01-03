#pragma once

#include "Trit.h"
#include <unordered_map>
#include <vector>


using namespace std;

static int unknown_mask = 0;

class Tritset
{
private:
	long long number_of_trits = 0;
	vector <unsigned int> arr;

	typedef struct enum_class_hash
	{
		template <typename T>
		size_t operator()(T t) const
		{
			return static_cast<size_t>(t);
		}
	}trit_hash;

	void set_value(long long index, trit value);
	void set_value(long long index0, long long index1, trit value);
	trit get_value(long long index) const;
	trit get_value(long long index0, long long index1) const;
public:
	Tritset(size_t capacity);
	Tritset(Tritset&& x) noexcept;
	size_t capacity();
	long long tritset_size();
	void shrink();
	void trim(long long last_index);
	size_t length();
	size_t cardinality(trit value);
	unordered_map<trit, int, trit_hash> cardinality();

	typedef struct trit_t
	{
	private:
		Tritset& set;
		long long number;
	public:
		trit_t(long long r, Tritset& s) : number(r), set(s) {}
		~trit_t() = default;

		operator trit() const
		{
			if (number >= set.number_of_trits)
				return trit::Unknown;
			else
				return set.get_value(number);
		}

		const Tritset& get_set()
		{
			return set;
		}

		long long get_number()
		{
			return number;
		}

		trit operator!() const
		{
			if (number >= set.number_of_trits)
				return trit::Unknown;
			else
				return !set.get_value(number);
		}

		bool operator==(const trit& t) const
		{
			if (number >= set.number_of_trits)
				return trit::Unknown == t;
			else
				return t == set.get_value(number);
		}

		bool operator!=(const trit& t) const
		{
			if (number >= set.number_of_trits)
				return false;
			else
				return t != set.get_value(number);
		}

		trit operator|(const trit& t) const
		{
			if (number >= set.number_of_trits)
				return trit::Unknown;
			else
				return t | set.get_value(number);
		}

		trit operator&(const trit& t) const
		{
			if (number >= set.number_of_trits)
				return trit::Unknown;
			else
				return t & set.get_value(number);
		}

		bool operator==(const trit_t& t) const
		{
			if (number >= set.number_of_trits)
				return false;
			else
				return t == set.get_value(number);
		}

		bool operator!=(const trit_t& t) const
		{
			if (number >= set.number_of_trits)
				return false;
			else
				return t != set.get_value(number);
		}

		trit operator|(const trit_t& t) const
		{
			if (number >= set.number_of_trits)
				return trit::Unknown;
			else
				return t | set.get_value(number);
		}

		trit operator&(const trit_t& t) const
		{
			if (number >= set.number_of_trits)
				return trit::Unknown;
			else
				return t & set.get_value(number);
		}

		trit_t& operator=(const trit& t)
		{
			if (!unknown_mask)
				for (size_t i = 0; i < 8 * sizeof(int); ++i)
					unknown_mask |= i % 2 == 0 ? 1u << i : 0;

			if (t != trit::Unknown && number >= set.number_of_trits)
			{
				set.arr.resize(ceil((2.0 * (1.0 + number)) / (8.0 * sizeof(int))), unknown_mask);
				set.number_of_trits = number + 1;
				set.set_value(number, t);
			}
			else if (number < set.number_of_trits)
				set.set_value(number, t);

			return *this;
		}
	}trit_link;

	friend ostream& operator<< (ostream& stream, Tritset::trit_link t)
	{
		if (t.get_number() >= t.get_set().number_of_trits)
			return stream << trit::Unknown;
		else
			return stream << t.get_set().get_value(t.get_number());
	}

	trit_link operator[](long long x);
	Tritset operator&(Tritset& x);
	Tritset operator|(Tritset& x);
	Tritset operator!();
};