#include "headers.h"

/**
 * An node-addon-api function which wraps the native everythingSearch() for JS
 * use.
 *
 * @param  info The Napi arguments and callback info
 * @return      Napi::Value (an array of search results)
 */
Napi::Value jsEverythingSearch(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env,
                         "search() expects two arguments: query and options")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "the search query must be a string")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[1].IsObject()) {
    Napi::TypeError::New(env, "the search options must be an object")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  // Prepare the query
  LPCWSTR query;
  std::string jsQuery = info[0].As<Napi::String>().Utf8Value();
  query = stringToLpcwstr(jsQuery);

  // Prepare the options
  SearchOptions options;
  Napi::Object jsOptions = info[1].As<Napi::Object>();

  // options.offset
  Napi::Value offset = jsOptions.Get("offset");
  if (offset.IsNumber()) {
    options.offset = offset.As<Napi::Number>().Uint32Value();
  } else {
    options.offset = 0;
  }

  // options.limit
  Napi::Value limit = jsOptions.Get("limit");
  if (limit.IsNumber()) {
    options.limit = limit.As<Napi::Number>().Uint32Value();
  } else {
    options.limit = 0;
  }

  // options.matchCase
  Napi::Value matchCase = jsOptions.Get("matchCase");
  if (matchCase.IsBoolean()) {
    options.matchCase = matchCase.As<Napi::Boolean>().Value();
  } else {
    options.matchCase = false;
  }

  // options.matchWholeWord
  Napi::Value matchWholeWord = jsOptions.Get("matchWholeWord");
  if (matchWholeWord.IsBoolean()) {
    options.matchWholeWord = matchWholeWord.As<Napi::Boolean>().Value();
  } else {
    options.matchWholeWord = false;
  }

  // options.matchPath
  Napi::Value matchPath = jsOptions.Get("matchPath");
  if (matchPath.IsBoolean()) {
    options.matchPath = matchPath.As<Napi::Boolean>().Value();
  } else {
    options.matchPath = false;
  }

  // options.enableRegex
  Napi::Value enableRegex = jsOptions.Get("enableRegex");
  if (enableRegex.IsBoolean()) {
    options.enableRegex = enableRegex.As<Napi::Boolean>().Value();
  } else {
    options.enableRegex = false;
  }

  // options.includeSize
  Napi::Value includeSize = jsOptions.Get("includeSize");
  if (includeSize.IsBoolean()) {
    options.includeSize = includeSize.As<Napi::Boolean>().Value();
  } else {
    options.includeSize = false;
  }

  // options.includeDateCreated
  Napi::Value includeDateCreated = jsOptions.Get("includeDateCreated");
  if (includeDateCreated.IsBoolean()) {
    options.includeDateCreated = includeDateCreated.As<Napi::Boolean>().Value();
  } else {
    options.includeDateCreated = false;
  }

  // options.includeDateModified
  Napi::Value includeDateModified = jsOptions.Get("includeDateModified");
  if (includeDateModified.IsBoolean()) {
    options.includeDateModified =
        includeDateModified.As<Napi::Boolean>().Value();
  } else {
    options.includeDateModified = false;
  }

  // options.includeDateAccessed
  Napi::Value includeDateAccessed = jsOptions.Get("includeDateAccessed");
  if (includeDateAccessed.IsBoolean()) {
    options.includeDateAccessed =
        includeDateAccessed.As<Napi::Boolean>().Value();
  } else {
    options.includeDateAccessed = false;
  }

  // options.includeRunCount
  Napi::Value includeRunCount = jsOptions.Get("includeRunCount");
  if (includeRunCount.IsBoolean()) {
    options.includeRunCount = includeRunCount.As<Napi::Boolean>().Value();
  } else {
    options.includeRunCount = false;
  }

  // options.sortAsc
  Napi::Value sortAsc = jsOptions.Get("sortAsc");
  if (sortAsc.IsBoolean()) {
    options.sortAsc = sortAsc.As<Napi::Boolean>().Value();
  } else {
    options.sortAsc = false;
  }

  // options.sortBy
  Napi::Value sortBy = jsOptions.Get("sortBy");
  if (sortBy.IsString()) {
    options.sortBy = sortBy.As<Napi::String>().Utf8Value();
  } else {
    options.sortBy = "name";
  }

  // Run the search delete the query string
  SearchResponse response = everythingSearch(query, options);
  delete[] query;

  // Create the array of results
  Napi::Array jsResults = Napi::Array::New(info.Env(), response.results.size());

  // Add the query results to the JS array of results
  size_t i = 0;
  for (const ResultType result : response.results) {
    Napi::Object jsResult = Napi::Object::New(env);

    jsResult.Set("type", result.type);
    jsResult.Set("name", result.name);
    jsResult.Set("dir", result.dir);

    if (options.includeSize) {
      jsResult.Set("size", result.size);
    }

    if (options.includeDateCreated) {
      jsResult.Set("dateCreated", result.dateCreated);
    }

    if (options.includeDateModified) {
      jsResult.Set("dateModified", result.dateModified);
    }

    if (options.includeDateAccessed) {
      jsResult.Set("dateAccessed", result.dateAccessed);
    }

    if (options.includeRunCount) {
      jsResult.Set("runCount", result.runCount);
    }

    jsResults[i++] = jsResult;
  }

  Napi::Object jsResponse = Napi::Object::New(env);

  jsResponse.Set("totalResults", response.totalResults);
  jsResponse.Set("results", jsResults);

  return jsResponse;
}

/**
 * Initialize the native addon and create the values exported to JS
 */
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Napi::Object addonExports = Napi::Object::New(env);

  addonExports.Set("search",
                   Napi::Function::New(env, jsEverythingSearch, "search"));

  return addonExports;
}

NODE_API_MODULE(addon, Init)
