#include <gtest/gtest.h>
#include "Tritset.h"
#include "Trit.h"
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1

TEST(MemoryTests, ConstructorAllocationTest)
{
	for (int i = 0; i <= 1000; ++i)
	{
		Tritset set(i);

		size_t alloc_length = set.capacity();
		EXPECT_GE(alloc_length, i * 2 / 8 / sizeof(unsigned int));
	}
}

TEST(MemoryTests, TritMemoryUnknown)
{
	Tritset set(6);
	set[0] = trit::False;

	EXPECT_TRUE(set[6] == trit::Unknown);
}

TEST(MemoryTests, TritMemory)
{
	Tritset set(6);
	set[0] = trit::False;
	set[1] = trit::Unknown;
	set[2] = trit::True;
	set[3] = trit::False;
	set[4] = trit::Unknown;
	set[5] = trit::True;

	EXPECT_TRUE(set[0] == trit::False);
	EXPECT_TRUE(set[1] == trit::Unknown);
	EXPECT_TRUE(set[2] == trit::True);
	EXPECT_TRUE(set[3] == trit::False);
	EXPECT_TRUE(set[4] == trit::Unknown);
	EXPECT_TRUE(set[5] == trit::True);
}

TEST(MemoryTests, UnknownOutOfBound)
{
	Tritset set(1000);
	size_t alloc_length = set.capacity();

	set[1000000000] = trit::Unknown;
	EXPECT_EQ(alloc_length, set.capacity());
}

TEST(MemoryTests, ComparingOutOfBound)
{
	Tritset set(1000);
	size_t alloc_length = set.capacity();

	EXPECT_FALSE(set[2000000000] == trit::True);
	EXPECT_FALSE(set[2000000000] == trit::False);
	EXPECT_EQ(alloc_length, set.capacity());
}

TEST(MemoryTests, OutOfBoundAllocation)
{
	Tritset set(1000);
	size_t alloc_length = set.capacity();

	set[1000000000] = trit::True;
	EXPECT_LT(alloc_length, set.capacity());
}

TEST(MemoryTests, ShrinkMemoryFree)
{
	bool condTrue = true;

	Tritset set(1000);
	set[100100] = trit::True;
	set[100100] = trit::Unknown;

	long a = 42;

	set[a] = trit::False;

	set.shrink();
	unsigned int x = ceil((2.0 * (a + 1)) / (8.0 * sizeof(unsigned int)));

	EXPECT_EQ(x, set.capacity());
}

TEST(MemoryTests, tritOperationAllocation)
{
	Tritset setA(1000);
	Tritset setB(2000);

	Tritset setC = setA | setB;

	EXPECT_EQ(setB.capacity(), setC.capacity());
}

TEST(tritOperationTests, OrTest)
{
	Tritset setA(3);
	Tritset setB(3);

	setA[0] = trit::Unknown;
	setA[1] = trit::Unknown;
	setA[2] = trit::Unknown;

	setB[0] = trit::True;
	setB[1] = trit::False;
	setB[2] = trit::Unknown;

	Tritset setC = setA | setB;

	EXPECT_TRUE(setC[0] == trit::True);
	EXPECT_TRUE(setC[1] == trit::Unknown);
	EXPECT_TRUE(setC[2] == trit::Unknown);
}

TEST(tritOperationTests, AndTest)
{
	Tritset setA(3);
	Tritset setB(3);

	setA[0] = trit::Unknown;
	setA[1] = trit::Unknown;
	setA[2] = trit::Unknown;

	setB[0] = trit::True;
	setB[1] = trit::False;
	setB[2] = trit::Unknown;

	Tritset setC = setA & setB;

	EXPECT_TRUE(setC[0] == trit::Unknown);
	EXPECT_TRUE(setC[1] == trit::False);
	EXPECT_TRUE(setC[2] == trit::Unknown);
}

TEST(tritOperationTests, NotTest)
{
	Tritset setA(3);

	setA[0] = trit::True;
	setA[1] = trit::Unknown;
	setA[2] = trit::False;

	Tritset setC = !setA;

	EXPECT_TRUE(setC[0] == trit::False);
	EXPECT_TRUE(setC[1] == trit::Unknown);
	EXPECT_TRUE(setC[2] == trit::True);
}

TEST(OtherFunctionsTests, CardinalityFunctionTest)
{
	Tritset set(3);

	set[0] = trit::True;
	set[1] = trit::Unknown;
	set[2] = trit::False;

	EXPECT_EQ(set.cardinality(trit::False), 1);
	EXPECT_EQ(set.cardinality(trit::True), 1);
	EXPECT_EQ(set.cardinality(trit::Unknown), 1);
}

TEST(OtherFunctionsTests, CardinalityMapTest)
{
	Tritset set(11);

	set[0] = trit::True;
	set[1] = trit::Unknown;
	set[2] = trit::False;
	set[3] = trit::False;
	set[4] = trit::Unknown;
	set[5] = trit::False;
	set[6] = trit::Unknown;
	set[7] = trit::Unknown;
	set[8] = trit::Unknown;
	set[9] = trit::Unknown;
	set[10] = trit::Unknown;

	auto a = set.cardinality();

	EXPECT_EQ(a[trit::True], 1);
	EXPECT_EQ(a[trit::False], 3);
	EXPECT_EQ(a[trit::Unknown], 2);
}

TEST(OtherFunctionsTests, TrimTest)
{
	Tritset set(7);

	set[0] = trit::True;
	set[1] = trit::Unknown;
	set[2] = trit::False;
	set[3] = trit::False;
	set[4] = trit::Unknown;
	set[5] = trit::True;
	set[6] = trit::Unknown;

	set.trim(2);
	EXPECT_TRUE(set[2] == trit::Unknown && set[3] == trit::Unknown && set[4] == trit::Unknown && set[5] == trit::Unknown && set[6] == trit::Unknown);
}

TEST(OtherFunctionsTests, LenTest)
{
	Tritset set(4);

	set[0] = trit::True;
	set[1] = trit::Unknown;
	set[2] = trit::False;
	set[3] = trit::Unknown;

	EXPECT_EQ(set.length(), 3);
}