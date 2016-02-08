#pragma once

#include <Common/Types.h>
#include <Libraries/Library.h>
#include <Logging/Logger.h>

#include <map>
#include <memory>
#include <type_traits>

namespace Libraries
{
	enum FW_LIBRARY_STATUS
	{
		LIBRARY_INVALID,
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

		fwvoid SetLogger(Logging::Logger &Logger)
		{
			this->Logger = Logger;

			return;
		}

		T *Load(const std::string path)
		{
			FW_LIBRARY_DESCRIPTOR *lib = new FW_LIBRARY_DESCRIPTOR();
			lib->fileName = std::wstring(path.begin(), path.end());

			if (!this->isValid)
			{
				this->log(Logging::LogLevel::LOG_ERROR, "Librarian - Cannot load library, librarian wasn't properly initialized");

				return NULL;
			}

			lib->instance = ::LoadLibrary(lib->fileName.c_str());

			if (lib->instance == NULL)
			{
				this->log(Logging::LogLevel::LOG_ERROR, "Librarian - Failed to load library from filesystem");

				return NULL;
			}

			lib->entry = (FW_DLL_ENTRY)GetProcAddress(lib->instance, "InitLibrary");

			if (lib->entry == NULL)
			{
				this->log(Logging::LogLevel::LOG_ERROR, "Librarian - Failed to retrieve proc address from library");

				FreeLibrary(lib->instance);

				return NULL;
			}

			lib->ptr = lib->entry();
			lib->status = LIBRARY_SUCCESS;
			this->libraries.insert(std::pair<const fwstr, FW_LIBRARY_DESCRIPTOR>(lib->fileName, lib));

			std::stringstream ss;
			ss << "Librarian - Successfully loaded the '" << path << "' library";
			this->log(Logging::LogLevel::LOG_INFO, ss.str());

			return lib->ptr;
		}

		fwbool Unload(const std::string path)
		{
			if (!this->isValid)
			{
				return false;
			}

			std::wstring tmp(path.begin(), path.end());
			auto lib = this->libraries.find(tmp);

			if (lib == this->libraries.end())
			{
				return false;
			}

			(*lib)->ptr->Destroy();
			FreeLibrary((*lib)->instance);
			this->libraries.erase(lib);

			return true;
		}

	protected:
		typedef T *(*FW_DLL_ENTRY)();

		struct FW_LIBRARY_DESCRIPTOR
		{
			T *ptr;
			std::wstring fileName;
			FW_LIBRARY_STATUS status;
			fwinstance instance;
			FW_DLL_ENTRY entry;
		};

		std::map<const fwstr, FW_LIBRARY_DESCRIPTOR> libraries;
		Logging::Logger *Logger;
		fwbool isValid;

		fwvoid log(Logging::LogLevel Level, const fwstr Message)
		{
			if (this->Logger)
			{
				this->Logger->Log(Message.c_str(), Level);
			}

			return;
		}
	};
}
