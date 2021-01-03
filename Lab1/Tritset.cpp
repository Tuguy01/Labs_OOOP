#include <bitset>
#include "TritSet.h"

unsigned int get_bit(unsigned int x, unsigned int index)
{
	return (x >> index) & 1u;
}

 void set_bit(unsigned int& x, unsigned int value, unsigned int index)
{
	if (value)
		x |= 1u << index;
	else
		x &= ~(1u << index);
}

Tritset::trit_link Tritset::operator[](long long b)
{
	trit_link a(b, *this);
	return a;
}

Tritset Tritset::operator|(Tritset& x)
{
	auto y = Tritset(x.capacity() > this->capacity() ? x.capacity() : this->capacity());

	y.arr = x.capacity() > this->capacity() ? x.arr : this->arr;
	y.number_of_trits = x.capacity() > this->capacity() ? x.number_of_trits : this->number_of_trits;
	size_t s = x.capacity() > this->capacity() ? this->arr.size() : x.arr.size();

	for (size_t i = 0; i < s; ++i)
		y.arr[i] |= x.capacity() > this->capacity() ? this->arr[i] : x.arr[i];

	return move(y);
}

Tritset Tritset::operator&(Tritset& x)
{
	auto y = Tritset(x.capacity() > this->capacity() ? x.capacity() : this->capacity());

	y.arr = x.capacity() > this->capacity() ? x.arr : this->arr;
	y.number_of_trits = x.capacity() > this->capacity() ? x.number_of_trits : this->number_of_trits;
	size_t s = x.capacity() > this->capacity() ? this->arr.size() : x.arr.size();

	for (size_t i = 0; i < s; ++i)
		y.arr[i] &= x.capacity() > this->capacity() ? this->arr[i] : x.arr[i];

	return move(y);
}

Tritset Tritset::operator!()
{
	auto x = Tritset(this->capacity());
	x.arr = this->arr;
	x.number_of_trits = this->number_of_trits;

	for (unsigned int& a : x.arr)
	{
		a = ~a;
		for (size_t i = 0; i < 4 * sizeof(unsigned int); i++)
		{
			if (!(a >> (i * 2u) & 1u) && (a >> (i * 2u + 1u) & 1u))
			{
				set_bit(a, 1, i * 2u);
				set_bit(a, 0, i * 2u + 1u);
			}
		}
	}
	return move(x);
}

Tritset::Tritset(Tritset&& x) noexcept
{
	arr = move(x.arr);
	number_of_trits = x.number_of_trits;

	x.number_of_trits = 0;
}

Tritset::Tritset(size_t capacity)
{
	if (!unknown_mask)
		for (size_t i = 0; i < 8 * sizeof(unsigned int); ++i)
			unknown_mask |= i % 2 == 0 ? 1u << i : 0;

	vector<unsigned int> a(ceil((2.0 * capacity) / (8.0 * sizeof(unsigned int))), unknown_mask);

	number_of_trits = capacity;
	arr = a;
}

trit Tritset::get_value(long long index0, long long index1) const
{
	unsigned char value = 0;

	value |= get_bit(arr[index0], index1 * 2);
	value |= get_bit(arr[index0], index1 * 2 + 1) << 1u;

	switch (value)
	{
	case 0:
		return trit::False;
	case 1:
	case 2:
		return trit::Unknown;
	case 3:
		return trit::True;
	default:
		return trit::False;
	}
}

trit Tritset::get_value(long long index) const
{
	if (index >= number_of_trits)
		return trit::Unknown;

	long long bit_index = index % (8 * sizeof(unsigned int) / 2);
	long long byte_index = index / ((8.0 * sizeof(unsigned int)) / 2.0);

	unsigned char value = 0;

	value |= get_bit(arr[byte_index], bit_index * 2);
	value |= get_bit(arr[byte_index], bit_index * 2 + 1) << 1u;

	switch (value)
	{
	case 0:
		return trit::False;
	case 2:
		return trit::Unknown;
	case 3:
		return trit::True;
	}
}

void Tritset::set_value(long long index, trit value)
{
	long long bit_index = index % (8 * sizeof(unsigned int) / 2);
	long long byte_index = index / ((8.0 * sizeof(unsigned int)) / 2.0);
		
	unsigned int new_value = 1;
		
	switch (value)
	{
	case trit::False:
		new_value = 0;
		break;
	case trit::Unknown:
		new_value = 1;
		break;
	case trit::True:
		new_value = 3;
		break;
	}

	set_bit(arr[byte_index], new_value & 1u, bit_index * 2);
	set_bit(arr[byte_index], (new_value >> 1u) & 1u, bit_index * 2 + 1);
}

void Tritset::set_value(long long index0, long long index1, trit value)
{
	unsigned int int_val;
	switch (value)
	{
	case trit::False:
		int_val = 0;
		break;
	case trit::Unknown:
		int_val = 1;
		break;
	case trit::True:
		int_val = 3;
		break;
	}

	set_bit(arr[index0], int_val & 1u, index1 * 2);
	set_bit(arr[index0], (int_val >> 1u) & 1u, index1 * 2 + 1);
}

void Tritset::shrink()
{
	long long a = arr.size();
	for (unsigned int i = a - 1;; i--)
	{
		if (arr[i] ^ unknown_mask)
		{
			arr.resize(i + 1);
			number_of_trits -= number_of_trits % ((8 * sizeof(unsigned int)) / 2) + 1;
			number_of_trits -= (a - i - 2) * ((8 * sizeof(unsigned int)) / 2);
			break;
		}
		if (!i)
		{
			arr.resize(0);
			number_of_trits = 0;
			break;
		}
	}

	unsigned int b = arr[arr.size() - 1];
	for (unsigned int i = number_of_trits % ((8 * sizeof(unsigned int)) / 2);; i--)
		if ((b >> (i * 2u + 1u) & 1u) || !(b >> (i * 2u) & 1u))
		{
			number_of_trits -= number_of_trits % ((8 * sizeof(unsigned int)) / 2) - i - 1;
			break;
		}
}

void Tritset::trim(long long lastIndex)
{
	if (lastIndex >= number_of_trits)
		return;

	long long indexEl = lastIndex / ((8.0 * sizeof(unsigned int)) / 2.0);

	for (size_t i = lastIndex % ((8 * sizeof(unsigned int)) / 2); i < ((8 * sizeof(unsigned int)) / 2); ++i)
		set_value(indexEl, i, trit::Unknown);

	for (size_t j = indexEl + 1; j < arr.size(); ++j)
		arr[j] = unknown_mask;
}

size_t Tritset::length()
{
	for (unsigned int i = arr.size() - 1;; i--)
	{
		for (unsigned int j = ((8.0 * sizeof(unsigned int)) / 2.0) - 1;; j--)
		{
			if ((arr[i] >> (j * 2u + 1u) & 1u) || !(arr[i] >> (j * 2u) & 1u))
				return i * ((8.0 * sizeof(unsigned int)) / 2.0) + j + 1;
			if (!j)
				break;
		}
		if (!i)
			break;
	}
	return 0;
}

size_t Tritset::cardinality(trit value)
{
	bool enableCount = value != trit::Unknown;

	long long counter = 0;
	for (unsigned int i = arr.size() - 1;; i--)
	{
		for (unsigned int j = ((8.0 * sizeof(unsigned int)) / 2.0) - 1;; j--)
		{
			if (get_value(i, j) == value && enableCount)
				counter++;
			else if (value == trit::Unknown && ((arr[i] >> (j * 2u + 1u) & 1u) || !(arr[i] >> (j * 2u) & 1u)) && !enableCount)
				enableCount = true;
			if (!j)
				break;
		}
		if (!i)
			break;
	}

	return counter;
}

unordered_map<trit, int, Tritset::trit_hash> Tritset::cardinality()
{
	unordered_map<trit, int, Tritset::trit_hash> trit_info;
	trit_info.insert(make_pair<trit, int>(trit::True, cardinality(trit::True)));
	trit_info.insert(make_pair<trit, int>(trit::False, cardinality(trit::False)));
	trit_info.insert(make_pair<trit, int>(trit::Unknown, cardinality(trit::Unknown)));

	return trit_info;
}

size_t Tritset::capacity()
{
	return ceil((2.0 * number_of_trits) / (8.0 * sizeof(unsigned int)));
}

long long Tritset::tritset_size()
{
	return number_of_trits;
}
