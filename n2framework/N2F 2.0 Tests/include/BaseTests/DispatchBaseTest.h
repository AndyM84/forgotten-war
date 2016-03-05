#pragma once

#include <UnitTest++/UnitTest++.h>
#include <N2f.hpp>

class ValidBaseDispatch : public N2f::DispatchBase
{
public:
	void Initialize() { this->MakeValid(); }
	int NumResults() { return 0; }
	void SetResult() { }
};

class InvalidBaseDispatch : public N2f::DispatchBase
{
public:
	void Initialize() { }
	int NumResults() { return 0; }
	void SetResult() { }
};

SUITE(DispatchBase)
{
	TEST(ValidBasicDispatch)
	{
		ValidBaseDispatch disp;
		disp.Initialize();

		CHECK_EQUAL(true, disp.IsValid());
	}

	TEST(InvalidBasicDispatch)
	{
		InvalidBaseDispatch disp;
		disp.Initialize();

		CHECK_EQUAL(false, disp.IsValid());
	}
}
