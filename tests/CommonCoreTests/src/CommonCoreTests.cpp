#include <CommonCoreTests.h>

TEST(Sanity)
{
	CHECK_EQUAL(1, 1);
}

struct True
{
	bool operator()(const UnitTest::Test* const) const
	{
		return true;
	}
};

int main(int argc, char *argv[])
{
	if (argc > 1 && strcmp("verbose", argv[1]) == 0)
	{
		UnitTest::XmlTestReporter reporter(std::cout);
		UnitTest::TestRunner runner(reporter);
		return runner.RunTestsIf(UnitTest::Test::GetTestList(), NULL, True(), 0);
	}

	return UnitTest::RunAllTests();
}
