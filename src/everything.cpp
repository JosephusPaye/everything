#include "headers.h"

/**
 * Convert the given LARGE_INTEGER struct to a single integer value
 */
static int64_t largeIntegerToInteger(LARGE_INTEGER const &integer) {
#ifdef INT64_MAX // Does the compiler natively support 64-bit integers?
  return integer.QuadPart;
#else
  return (static_cast<int64_t>(integer.HighPart) << 32) | integer.LowPart;
#endif
}

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL

/**
 * Convert the given Windows FILETIME struct to a single integer value
 * representing the number of milliseconds since the Unix Epoch
 */
uint64_t filetimeToMsSinceEpoch(FILETIME ft) {
  uint64_t input = ULARGE_INTEGER{ft.dwLowDateTime, ft.dwHighDateTime}.QuadPart;

  // Convert from 100ns intervals to seconds;
  uint64_t temp = input / TICKS_PER_SECOND;

  // Subtract number of seconds between the Windows and Unix epochs
  temp = temp - EPOCH_DIFFERENCE;

  // Convert to milliseconds to match JS's Date.now()
  return (uint64_t)temp * 1000;
}

/**
 * Run an Everything search using the SDK methods with the given query and
 * options
 */
SearchResponse everythingSearch(LPCWSTR query, SearchOptions options) {
  // Reset any previous results
  Everything_Reset();

  if (options.offset > 0) {
    Everything_SetOffset(options.offset);
  }

  if (options.limit > 0) {
    Everything_SetMax(options.limit);
  }

  if (options.matchCase) {
    Everything_SetMatchCase(options.matchCase);
  }

  if (!options.enableRegex && options.matchWholeWord) {
    Everything_SetMatchWholeWord(options.matchWholeWord);
  }

  if (options.matchPath) {
    Everything_SetMatchPath(options.matchPath);
  }

  if (options.enableRegex) {
    Everything_SetRegex(options.enableRegex);
  }

  // Sort by 'name' by default
  DWORD sortType = options.sortAsc ? EVERYTHING_SORT_NAME_ASCENDING
                                   : EVERYTHING_SORT_NAME_DESCENDING;

  if (options.sortBy == "dir") {
    sortType = options.sortAsc ? EVERYTHING_SORT_PATH_ASCENDING
                               : EVERYTHING_SORT_PATH_DESCENDING;
  } else if (options.sortBy == "size") {
    sortType = options.sortAsc ? EVERYTHING_SORT_SIZE_ASCENDING
                               : EVERYTHING_SORT_SIZE_DESCENDING;
  } else if (options.sortBy == "dateCreated") {
    sortType = options.sortAsc ? EVERYTHING_SORT_DATE_CREATED_ASCENDING
                               : EVERYTHING_SORT_DATE_CREATED_DESCENDING;
  } else if (options.sortBy == "dateModified") {
    sortType = options.sortAsc ? EVERYTHING_SORT_DATE_MODIFIED_ASCENDING
                               : EVERYTHING_SORT_DATE_MODIFIED_DESCENDING;
  } else if (options.sortBy == "dateAccessed") {
    sortType = options.sortAsc ? EVERYTHING_SORT_DATE_ACCESSED_ASCENDING
                               : EVERYTHING_SORT_DATE_ACCESSED_DESCENDING;
  } else if (options.sortBy == "runCount") {
    sortType = options.sortAsc ? EVERYTHING_SORT_RUN_COUNT_ASCENDING
                               : EVERYTHING_SORT_RUN_COUNT_DESCENDING;
  }

  // Gets the file name and directory path by default
  DWORD requestFlags = EVERYTHING_REQUEST_FILE_NAME | EVERYTHING_REQUEST_PATH;

  if (options.includeSize) {
    requestFlags = requestFlags | EVERYTHING_REQUEST_SIZE;
  }

  if (options.includeDateCreated) {
    requestFlags = requestFlags | EVERYTHING_REQUEST_DATE_CREATED;
  }

  if (options.includeDateModified) {
    requestFlags = requestFlags | EVERYTHING_REQUEST_DATE_MODIFIED;
  }

  if (options.includeDateAccessed) {
    requestFlags = requestFlags | EVERYTHING_REQUEST_DATE_ACCESSED;
  }

  if (options.includeDateAccessed) {
    requestFlags = requestFlags | EVERYTHING_REQUEST_DATE_ACCESSED;
  }

  if (options.includeRunCount) {
    requestFlags = requestFlags | EVERYTHING_REQUEST_RUN_COUNT;
  }

  // Set the state and run the search
  Everything_SetRequestFlags(requestFlags);
  Everything_SetSort(sortType);
  Everything_SetSearchW(query);
  Everything_QueryW(TRUE);

  // Setup the response
  SearchResponse response;
  response.totalResults = Everything_GetNumResults();

  // Populate the list of results
  for (DWORD i = 0; i < response.totalResults; i++) {
    ResultType result;

    if (Everything_IsFileResult(i)) {
      result.type = "file";
    } else if (Everything_IsFolderResult(i)) {
      result.type = "folder";
    } else {
      result.type = "volume";
    }

    LPCWSTR name = Everything_GetResultFileNameW(i);
    result.name = lpcwstrToString(name);

    LPCWSTR dir = Everything_GetResultPath(i);
    result.dir = lpcwstrToString(dir);

    if (options.includeSize) {
      if (result.type == "file") {
        LARGE_INTEGER sizeLi;
        Everything_GetResultSize(i, &sizeLi);
        result.size = largeIntegerToInteger(sizeLi);
      } else {
        result.size = -1;
      }
    } else {
      result.size = -1;
    }

    if (options.includeDateCreated) {
      FILETIME ft;
      Everything_GetResultDateCreated(i, &ft);
      result.dateCreated = filetimeToMsSinceEpoch(ft);
    } else {
      result.dateCreated = 0;
    }

    if (options.includeDateModified) {
      FILETIME ft;
      Everything_GetResultDateModified(i, &ft);
      result.dateModified = filetimeToMsSinceEpoch(ft);
    } else {
      result.dateModified = 0;
    }

    if (options.includeDateAccessed) {
      FILETIME ft;
      Everything_GetResultDateAccessed(i, &ft);
      result.dateAccessed = filetimeToMsSinceEpoch(ft);
    } else {
      result.dateAccessed = 0;
    }

    if (options.includeRunCount) {
      result.runCount = Everything_GetResultRunCount(i);
    } else {
      result.runCount = 0;
    }

    response.results.push_back(result);
  }

  return response;
}
