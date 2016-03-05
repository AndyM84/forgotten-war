#pragma once

#include <UnitTest++/UnitTest++.h>
#include <N2f.hpp>

SUITE(CliDispatchTest)
{
	TEST(CliDispatchProducesCorrectCount)
	{
		N2f::CliDispatch disp;
		disp.Initialize(2, new char*[2] { "test", "two" });

		CHECK_EQUAL(2, disp.NumResults());
	}

	TEST(CliDispatchProducesCorrectRawString)
	{
		N2f::CliDispatch disp;
		disp.Initialize(2, new char*[2] { "test", "two" });

		CHECK_EQUAL("test two", disp.GetParameterString());
	}

	TEST(CliDispatchProducesCorrectRawParameters)
	{
		N2f::CliDispatch disp;
		disp.Initialize(2, new char*[2]{ "test", "two" });

		auto params = disp.GetRawParameters();

		CHECK_EQUAL(2, params.size());
		CHECK_EQUAL("test", params[0]);
		CHECK_EQUAL("two", params[1]);
	}

	TEST(CliDispatchProducesCorrectVariantMapCount)
	{
		N2f::CliDispatch disp;
		disp.Initialize(5, new char*[5] { "teSt", "-test2", "value", "test3=hey dude", "tEst" });

		auto paramMap = disp.GetParameterMap(false);

		CHECK_EQUAL(4, paramMap.size());
	}

	TEST(CliDispatchProduceCorreVariantMapValues)
	{
		N2f::CliDispatch disp;
		disp.Initialize(5, new char*[5] { "teSt", "-test2", "value", "test3=hey dude", "tEst" });

		auto paramMap = disp.GetParameterMap(false);

		auto expectedMap = std::map<std::string, std::string>();
		expectedMap.insert(std::pair<std::string, std::string>("teSt", "true"));
		expectedMap.insert(std::pair<std::string, std::string>("test2", "value"));
		expectedMap.insert(std::pair<std::string, std::string>("test3", "hey dude"));
		expectedMap.insert(std::pair<std::string, std::string>("tEst", "true"));

		REQUIRE CHECK_EQUAL(expectedMap.size(), paramMap.size());

		auto pmIter = paramMap.begin();
		auto emIter = expectedMap.begin();

		for (; pmIter != paramMap.end(); )
		{
			CHECK_EQUAL(emIter->first, pmIter->first);
			CHECK_EQUAL(emIter->second, pmIter->second);

			++pmIter;
			++emIter;
		}
	}

	TEST(CliDispatchProducesCorrectInvariantMapCount)
	{
		N2f::CliDispatch disp;
		disp.Initialize(5, new char*[5]{ "teSt", "-test2", "value", "test3=hey dude", "tEst" });

		auto paramMap = disp.GetParameterMap(true);

		CHECK_EQUAL(3, paramMap.size());
	}

	TEST(CliDispatchProducesCorrectInvariantMapValues)
	{
		N2f::CliDispatch disp;
		disp.Initialize(5, new char*[5]{ "teSt", "-test2", "value", "test3=hey dude", "tEst" });

		auto paramMap = disp.GetParameterMap(true);

		auto expectedMap = std::map<std::string, std::string>();
		expectedMap.insert(std::pair<std::string, std::string>("test", "true"));
		expectedMap.insert(std::pair<std::string, std::string>("test2", "value"));
		expectedMap.insert(std::pair<std::string, std::string>("test3", "hey dude"));

		REQUIRE CHECK_EQUAL(expectedMap.size(), paramMap.size());

		auto pmIter = paramMap.begin();
		auto emIter = expectedMap.begin();

		for (; pmIter != paramMap.end(); )
		{
			CHECK_EQUAL(emIter->first, pmIter->first);
			CHECK_EQUAL(emIter->second, pmIter->second);

			++pmIter;
			++emIter;
		}
	}
}
