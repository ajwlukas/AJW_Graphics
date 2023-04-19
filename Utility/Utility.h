#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>


namespace Utility
{
    using namespace std;

	inline std::string  ToString(std::wstring value);
	inline std::wstring ToWstring(std::string value);
	inline std::wstring ToWstring(const char* value);

	inline void SplitString(std::vector<std::string>& dest, std::string origin, std::string tok);
	inline void Replace(std::string* str, std::string comp, std::string rep);

	inline std::wstring GetExtension(std::wstring path);
	inline std::string GetDirectoryName(std::string path);
	inline std::wstring GetDirectoryName(std::wstring path);
	inline std::string GetFileName(std::string path);
	inline std::string GetFileNameWithoutExtension(std::string path);
	inline std::string GetPathNameWithoutExtension(std::string path);

	inline void ToUpper(std::string& value);

	inline bool ExistFile(std::string path);
	inline bool ExistFile(std::wstring path);
	inline bool ExistDirectory(std::string path);
	inline void CreateFolders(std::string path);

	inline std::vector<char> GetBinary(std::string path);
	inline std::vector<char> GetBinaryW(std::wstring path);


    inline string Utility::ToString(wstring value)
    {
        string temp = "";
        temp.assign(value.begin(), value.end());

        return temp;
    }

    inline wstring Utility::ToWstring(string value)
    {
        wstring temp = L"";
        temp.assign(value.begin(), value.end());

        return temp;
    }

    inline std::wstring Utility::ToWstring(const char* value)//상연이 코드
    {
        const int _stringLength = static_cast<int>(std::strlen(value));
        const int _bufferLength = _stringLength + 1;
        wchar_t* _buffer = new wchar_t[_bufferLength];

        // MultiByteToWideChar 함수가 문자열의 끝에 자동으로 null 문자 ('\0')을 넣어주지 않습니다.
        // 따라서 문자열을 변환을 마친 후 그 뒤에다 수동으로 null문자를 넣어주어야 합니다.
        int _end = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, value, _stringLength,
            _buffer, _bufferLength);
        _buffer[_end] = '\0';

        auto _retVal = std::wstring{ _buffer };

        delete[] _buffer;
        return _retVal;
    }

    inline void Utility::SplitString(std::vector<std::string>& dest, string origin, string tok)
    {
        vector<string> result;

        size_t cutAt = 0;

        while ((cutAt = origin.find_first_of(tok)) != origin.npos)
        {
            if (cutAt > 0)
                result.emplace_back(origin.substr(0, cutAt));

            origin = origin.substr(cutAt + 1);
        }
        dest = result;
    }

    inline void Utility::Replace(string* str, string comp, string rep)
    {
        string temp = *str;

        size_t at = 0;
        while ((at = temp.find(comp, at)) != string::npos)
        {
            temp.replace(at, comp.length(), rep);
            at += rep.length();
        }
        *str = temp;
    }

    inline wstring Utility::GetExtension(wstring path)
    {
        size_t index = path.find_last_of('.');

        return path.substr(index + 1, path.length());
    }

    inline string Utility::GetDirectoryName(string path)
    {
        Replace(&path, "\\", "/");

        size_t index = path.find_last_of('/');

        return path.substr(0, index + 1);
    }
    
    inline wstring Utility::GetDirectoryName(wstring path)
    {
        string str = ToString(path);

        Replace(&str, "\\", "/");

        size_t index = str.find_last_of('/');

        return ToWstring(str.substr(0, index + 1));
    }

    inline string Utility::GetFileName(string path)
    {
        Replace(&path, "\\", "/");

        size_t index = path.find_last_of('/');

        return path.substr(index + 1, path.length());
    }

    inline string Utility::GetFileNameWithoutExtension(string path)
    {
        string fileName = GetFileName(path);

        size_t index = fileName.find_last_of('.');

        return fileName.substr(0, index);
    }

    inline std::string Utility::GetPathNameWithoutExtension(std::string path)
    {
        Replace(&path, "\\", "/");

        size_t index = path.find_last_of('.');

        return path.substr(0, index);
    }

    inline void Utility::ToUpper(string& value)
    {
        for (int i = 0; i < value.length(); i++)
        {
            value[i] = toupper(value[i]);
        }
    }

    inline bool Utility::ExistFile(string path)
    {
        DWORD fileValue = GetFileAttributesA(path.c_str());

        return fileValue < 0xffffffff;
    }

    inline bool Utility::ExistFile(wstring path)
    {
        return ExistFile(ToString(path));
    }

    inline bool Utility::ExistDirectory(string path)
    {
        DWORD fileValue = GetFileAttributesA(path.c_str());

        BOOL temp = (fileValue != INVALID_FILE_ATTRIBUTES &&
            (fileValue & FILE_ATTRIBUTE_DIRECTORY));

        return temp == TRUE;
    }

    inline void Utility::CreateFolders(string path)
    {
        vector<string> folders;
        SplitString(folders, path, "/");

        string temp = "";
        for (string folder : folders)
        {
            temp += folder + "/";

            if (!ExistDirectory(temp))
                CreateDirectoryA(temp.c_str(), 0);
        }
    }

    inline std::vector<char> Utility::GetBinary(std::string path)
    {
        std::ifstream fin(path, std::ios::binary);

        fin.seekg(0, std::ios_base::end);
        int size = (int)fin.tellg();
        assert(size >= 0);//file not Found

        fin.seekg(0, std::ios_base::beg);
        std::vector<char> binary(size);

        fin.read(binary.data(), size);
        fin.close();

        return binary;
    }

    inline std::vector<char> Utility::GetBinaryW(std::wstring path)
    {
        std::ifstream fin(path, std::ios::binary);

        fin.seekg(0, std::ios_base::end);
        int size = (int)fin.tellg();
        fin.seekg(0, std::ios_base::beg);
        std::vector<char> binary(size);

        fin.read(binary.data(), size);
        fin.close();

        return binary;
    }
};