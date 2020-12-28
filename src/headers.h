#pragma once

#define UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601

#include <napi.h>
#include <string>
#include <vector>

#include "Everything.h"

typedef struct {
  size_t offset;
  size_t limit;
  bool matchCase;
  bool matchWholeWord;
  bool matchPath;
  bool enableRegex;
  bool includeSize;
  bool includeDateCreated;
  bool includeDateModified;
  bool includeDateAccessed;
  bool includeRunCount;
  bool sortAsc;
  std::string sortBy;
} SearchOptions;

typedef struct {
  std::string type;
  std::string name;
  std::string dir;
  int64_t size;
  uint64_t dateCreated;
  uint64_t dateModified;
  uint64_t dateAccessed;
  uint64_t runCount;
} ResultType;

using ResultList = std::vector<ResultType>;

typedef struct {
  size_t totalResults;
  ResultList results;
} SearchResponse;

extern SearchResponse everythingSearch(LPCWSTR query, SearchOptions options);
extern Napi::Value jsEverythingSearch(const Napi::CallbackInfo &info);

extern LPCWSTR stringToLpcwstr(const std::string &str);
extern std::string lpcwstrToString(const wchar_t *origStr);
