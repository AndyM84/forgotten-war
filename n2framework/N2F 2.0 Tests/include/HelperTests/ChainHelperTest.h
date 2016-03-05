#pragma once

#include <UnitTest++/UnitTest++.h>
#include <N2f.hpp>

#include <vector>

/* Dispatches */

class IntegerDispatch : public N2f::DispatchBase
{
public:
	virtual void Initialize() = 0;
	virtual int NumResults() = 0;
	virtual void SetResult(int Result) = 0;
	virtual const std::vector<int> GetResults() = 0;

	void SetResult() { };
};

class NonConsumableStatefulDispatch : public IntegerDispatch
{
public:
	void Initialize() { this->MakeStateful(); this->MakeValid(); }
	int NumResults() { return (int)this->_results.size(); }
	void SetResult(int Result) { this->_results.push_back(Result); }
	const std::vector<int> GetResults() { return this->_results; }

private:
	std::vector<int> _results;
};

class ConsumableStatefulDispatch : public IntegerDispatch
{
public:
	void Initialize() { this->MakeConsumable(); this->MakeStateful(); this->MakeValid(); }
	int NumResults() { return (int)this->_results.size(); }
	void SetResult(int Result) { this->_results.push_back(Result); }
	const std::vector<int> GetResults() { return this->_results; }

private:
	std::vector<int> _results;
};

class NonConsumableNonStatefulDispatch : public IntegerDispatch
{
public:
	NonConsumableNonStatefulDispatch() { this->_result = -1; }
	void Initialize() { this->MakeValid(); }
	int NumResults() { return this->_result == -1 ? 0 : 1; }
	void SetResult(int Result) { this->_result = Result; }
	const std::vector<int> GetResults() { return std::vector<int>(1, this->_result); }

private:
	int _result;
};

class ConsumableNonStatefulDispatch : public IntegerDispatch
{
public:
	ConsumableNonStatefulDispatch() { this->_result = -1; }
	void Initialize() { this->MakeConsumable(); this->MakeValid(); }
	int NumResults() { return this->_result == -1 ? 0 : 1; }
	void SetResult(int Result) { this->_result = Result; }
	const std::vector<int> GetResults() { return std::vector<int>(1, this->_result); }

private:
	int _result;
};

/* Nodes */

class IntegerNode : public N2f::NodeBase<IntegerDispatch>
{
public:
	IntegerNode(const char *Key, const char *Version) : N2f::NodeBase<IntegerDispatch>(Key, Version) { }

	virtual void Process(void *Sender, std::shared_ptr<IntegerDispatch> Dispatch) = 0;
};

class NonConsumerNode : public IntegerNode
{
public:
	NonConsumerNode() : IntegerNode("NonConsumerNode", "1") { }

	void Process(void *Sender, std::shared_ptr<IntegerDispatch> Dispatch)
	{
		int newResult = 0;

		for (auto res : Dispatch->GetResults())
		{
			if (res >= newResult)
			{
				newResult = res + 1;
			}
		}

		Dispatch->SetResult(newResult);
		
		return;
	}
};

class ConsumerNode : public IntegerNode
{
public:
	ConsumerNode() : IntegerNode("ConsumerNode", "1") { }

	void Process(void *Sender, std::shared_ptr<IntegerDispatch> Dispatch)
	{
		int newResult = 0;

		for (auto res : Dispatch->GetResults())
		{
			if (res >= newResult)
			{
				newResult = res + 1;
			}
		}

		Dispatch->SetResult(newResult);
		Dispatch->Consume();

		return;
	}
};

class OneEventNode : public IntegerNode
{
public:
	OneEventNode() : IntegerNode("OneEventNode", "1") { }

	void Process(void *Sender, std::shared_ptr<IntegerDispatch> Dispatch)
	{
		Dispatch->SetResult(5);

		return;
	}
};

class TwoEventNode : public IntegerNode
{
public:
	TwoEventNode() : IntegerNode("TwoEventNode", "1") { }

	void Process(void *Sender, std::shared_ptr<IntegerDispatch> Dispatch)
	{
		Dispatch->SetResult(10);

		return;
	}
};

/* Tests */

SUITE(ChainHelperTest)
{
	TEST(NonConsumableStatefulChainGetsCorrectCount)
	{
		N2f::ChainHelper<IntegerDispatch> chain;

		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<ConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());

		auto disp = std::make_shared<NonConsumableStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(4, disp->GetResults().size());
	}

	TEST(ConsumableStatefulChainGetsCorrectCount)
	{
		N2f::ChainHelper<IntegerDispatch> chain;

		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<ConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());

		auto disp = std::make_shared<ConsumableStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(3, disp->GetResults().size());
	}

	TEST(NonConsumableNonStatefulChainGetsCorrectCount)
	{
		N2f::ChainHelper<IntegerDispatch> chain;

		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());

		auto disp = std::make_shared<NonConsumableNonStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(1, disp->GetResults().size());
	}

	TEST(ConsumableNonStatefulChainGetsCorrectCount)
	{
		N2f::ChainHelper<IntegerDispatch> chain;

		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<ConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());

		auto disp = std::make_shared<ConsumableNonStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(1, disp->GetResults().size());
	}

	TEST(NonConsumableStatefulChainGetsCorrectValue)
	{
		N2f::ChainHelper<IntegerDispatch> chain;

		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<ConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());

		auto disp = std::make_shared<NonConsumableStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(3, disp->GetResults()[3]);
	}

	TEST(ConsumableStatefulChainGetsCorrectValue)
	{
		N2f::ChainHelper<IntegerDispatch> chain;

		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<ConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());

		auto disp = std::make_shared<ConsumableStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(2, disp->GetResults()[2]);
	}

	TEST(NonConsumableNonStatefulChainGetsCorrectValue)
	{
		N2f::ChainHelper<IntegerDispatch> chain;

		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<ConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());

		auto disp = std::make_shared<NonConsumableNonStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(3, disp->GetResults()[0]);
	}

	TEST(ConsumableNonStatefulChainGetsCorrectValue)
	{
		N2f::ChainHelper<IntegerDispatch> chain;

		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());
		chain.LinkNode(std::make_shared<ConsumerNode>());
		chain.LinkNode(std::make_shared<NonConsumerNode>());

		auto disp = std::make_shared<ConsumableNonStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(2, disp->GetResults()[0]);
	}

	TEST(EventChainReturnsCorrectCount)
	{
		N2f::ChainHelper<IntegerDispatch> chain(true, false);

		chain.LinkNode(std::make_shared<OneEventNode>());
		chain.LinkNode(std::make_shared<TwoEventNode>());

		auto disp = std::make_shared<ConsumableStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(1, disp->GetResults().size());
	}

	TEST(EventChainReturnsCorrectValue)
	{
		N2f::ChainHelper<IntegerDispatch> chain(true, false);

		chain.LinkNode(std::make_shared<OneEventNode>());
		chain.LinkNode(std::make_shared<TwoEventNode>());

		auto disp = std::make_shared<ConsumableStatefulDispatch>();
		disp->Initialize();

		chain.Traverse(nullptr, disp);

		CHECK_EQUAL(10, disp->GetResults()[0]);
	}
}
