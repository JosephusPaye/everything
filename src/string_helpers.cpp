#pragma once

#include <string>
#include <windows.h>

/**
 * Convert an std::string to LPCWSTR
 * See https://stackoverflow.com/a/27296
 */
LPCWSTR stringToLpcwstr(const std::string &str) {
  int stringLength = (int)str.length() + 1;

  // The first call with a 0 target string returns the buffer length needed
  int bufferLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);

  wchar_t *buffer = new wchar_t[bufferLength];

  // The second call actually does the conversion
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, bufferLength);

  return buffer;
}

/**
 * Convert a wchar_t* (aka LPCWSTR) string to an std::string
 * https://docs.microsoft.com/en-us/cpp/text/how-to-convert-between-various-string-types?view=msvc-160#example-convert-from-wchar_t-
 */
std::string lpcwstrToString(const wchar_t *origStr) {
  // Convert the wchar_t string to a char* string. Record
  // the length of the original string and add 1 to it to
  // account for the terminating null character.
  size_t origSize = wcslen(origStr) + 1;

  // Allocate two bytes in the multibyte output string for every wide
  // character in the input string (including a wide character
  // null). Because a multibyte character can be one or two bytes,
  // you should allot two bytes for each character. Having extra
  // space for the new string is not an error, but having
  // insufficient space is a potential security problem.
  const size_t newSize = origSize * 2;

  // The new string will contain a converted copy of the original string
  char *newString = new char[newSize];

  // Convert the wchar_t* string to a C-style char* string
  size_t convertedChars = 0;
  wcstombs_s(&convertedChars, newString, newSize, origStr, _TRUNCATE);

  // Convert the c-style char* string to an std::string
  std::string converted = std::string(newString);

  // Delete the C-style char* string
  delete[] newString;

  return converted;
}
