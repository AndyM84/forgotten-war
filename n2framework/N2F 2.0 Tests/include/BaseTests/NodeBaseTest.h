#pragma once

#include <UnitTest++/UnitTest++.h>
#include <N2f.hpp>

class ValidBaseNode : public N2f::NodeBase<N2f::DispatchBase>
{
public:
	ValidBaseNode()
		: N2f::NodeBase<N2f::DispatchBase>("ValidBaseNode", "0.0.1")
	{ }

	void Process(void *Sender, std::shared_ptr<N2f::DispatchBase> Dispatch) { }
};

class InvalidBaseNode : public N2f::NodeBase<N2f::DispatchBase>
{
public:
	InvalidBaseNode()
		: N2f::NodeBase<N2f::DispatchBase>("", "")
	{ }

	void Process(void *Sender, std::shared_ptr<N2f::DispatchBase> Dispatch) { }
};

SUITE(NodeBase)
{
	TEST(ValidBasicNode)
	{
		ValidBaseNode node;

		CHECK_EQUAL(true, node.IsValid());
	}

	TEST(InvalidBasicNode)
	{
		InvalidBaseNode node;

		CHECK_EQUAL(false, node.IsValid());
	}
}
