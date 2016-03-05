#include "BaseClasses/DispatchBase.hpp"

namespace N2f
{
	void DispatchBase::MakeConsumable()
	{
		this->_isConsumable = true;

		return;
	}

	void DispatchBase::MakeStateful()
	{
		this->_isStateful = true;

		return;
	}

	void DispatchBase::MakeValid()
	{
		this->_isValid = true;

		return;
	}

	bool DispatchBase::Consume()
	{
		if (this->_isConsumable && !this->_isConsumed)
		{
			this->_isConsumed = true;

			return true;
		}

		return false;
	}

	bool DispatchBase::IsConsumable()
	{
		return this->_isConsumable;
	}

	bool DispatchBase::IsConsumed()
	{
		return this->_isConsumed;
	}

	bool DispatchBase::IsStateful()
	{
		return this->_isStateful;
	}

	bool DispatchBase::IsValid()
	{
		return this->_isValid;
	}
}
