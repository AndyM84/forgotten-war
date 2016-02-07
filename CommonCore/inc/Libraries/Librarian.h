#pragma once

#include <Common/Types.h>
#include <Libraries/Library.h>
#include <type_traits>
#include <vector>

namespace Libraries
{
	typedef Library *(*FW_DLL_ENTRY)();

	enum FW_LIBRARY_STATUS
	{
		LIBRARY_SUCCESS,
		LIBRARY_FILELOAD_ERROR,
		LIBRARY_ENTRYPOINT_ERROR
	};

	template<class T>
	class Librarian
	{
	public:
		Librarian()
		{
			this->isValid = std::is_base_of<Library, T>::value;

			return;
		}

		fwbool LoadLibrary(const fwstr path)
		{
			return false;
		}

	protected:
		struct FW_LIBRARY_DESCRIPTOR
		{
			T *ptr;
			wfwstr fileName;
			FW_LIBRARY_STATUS status;
			fwinstance instance;
			FW_DLL_ENTRY entry;
		};

		std::vector<FW_LIBRARY_DESCRIPTOR> libraries;
		fwbool isValid;

		fwbool loadLibrary(const fwstr path, FW_LIBRARY_DESCRIPTOR *descriptor)
		{
			if (!this->isValid)
			{
				return false;
			}


		}
	};
}
