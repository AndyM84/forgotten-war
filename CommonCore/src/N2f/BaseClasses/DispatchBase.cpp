#include <N2f/BaseClasses/DispatchBase.h>

namespace N2f
{
	fwvoid DispatchBase::MakeConsumable()
	{
		this->_isConsumable = true;

		return;
	}

	fwvoid DispatchBase::MakeStateful()
	{
		this->_isStateful = true;

		return;
	}

	fwvoid DispatchBase::MakeValid()
	{
		this->_isValid = true;

		return;
	}

	fwbool DispatchBase::Consume()
	{
		if (this->_isConsumable && !this->_isConsumed)
		{
			this->_isConsumed = true;

			return true;
		}

		return false;
	}

	fwbool DispatchBase::IsConsumable()
	{
		return this->_isConsumable;
	}

	fwbool DispatchBase::IsConsumed()
	{
		return this->_isConsumed;
	}

	fwbool DispatchBase::IsStateful()
	{
		return this->_isStateful;
	}

	fwbool DispatchBase::IsValid()
	{
		return this->_isValid;
	}
}
