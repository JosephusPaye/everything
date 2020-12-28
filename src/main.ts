const addon = require('bindings')('everything');

export interface SearchOptions {
  /**
   * Return results from the nth result, where offset is n
   * @default 0
   */
  offset?: number;

  /**
   * Maximum number of results to return
   * @default 100
   */
  limit?: number;

  /**
   * Match results case to query
   * @default false
   */
  matchCase?: boolean;

  /**
   * Match whole words only
   * @default false
   */
  matchWholeWord?: boolean;

  /**
   * Match directory paths in addition to file names
   * @default false
   */
  matchPath?: boolean;

  /**
   * Enable regular expression matching. Overrides default search syntax.
   * See https://www.voidtools.com/support/everything/searching/#regex.
   * @default false
   */
  enableRegex?: boolean;

  /**
   * Include the size of the result. Volume and folder results will have a size of -1.
   * @default false
   */
  includeSize?: boolean;

  /**
   * Include the created date of the result. This property is not indexed
   * by Everything by default and its inclusion could lead to slower search
   * performance.
   * @default false
   */
  includeDateCreated?: boolean;

  /**
   * Include the modified date of the result
   * @default false
   */
  includeDateModified?: boolean;

  /**
   * Include the accessed date of the result. This property is not indexed
   * by Everything by default and its inclusion could lead to slower search
   * performance.
   * @default false
   */
  includeDateAccessed?: boolean;

  /**
   * Include the Everything run count of the result
   * @default false
   */
  includeRunCount?: boolean;

  /**
   * Sort the results in ascending order
   * @default true
   */
  sortAsc?: boolean;

  /**
   * Sort the results by this property. Note that properties that are optional
   * (like `size` and `dateCreated`) should be included when searching before
   * sorting by them. For better performance, consider enabling indexing and
   * ("fast sort")[https://www.voidtools.com/support/everything/indexes/#fast_sorting]
   * in Everything's Options for the properties you want to sort by.
   * @default 'name'
   */
  sortBy?:
    | 'name'
    | 'dir'
    | 'size'
    | 'dateCreated'
    | 'dateModified'
    | 'dateAccessed'
    | 'runCount';
}

export interface SearchResult {
  /**
   * The type of result
   */
  type: 'file' | 'folder' | 'volume';

  /**
   * The name of the result
   */
  name: string;

  /**
   * The full path to the result's parent directory
   */
  dir: string;

  /**
   * The size of the result, in bytes. Only present if search had `includeSize` option.
   */
  size?: number;

  /**
   * The created date of the result, in milliseconds since the Unix Epoch.
   * Only present if search had `includeDateCreated` option.
   */
  dateCreated?: number;

  /**
   * The modified date of the result, in milliseconds since the Unix Epoch.
   * Only present if search had `includeDateModified` option.
   */
  dateModified?: number;

  /**
   * The accessed date of the result, in milliseconds since the Unix Epoch.
   * Only present if search had `includeDateAccessed` option.
   */
  dateAccessed?: number;

  /**
   * The Everything run count of the result. Only present if search had `includeRunCount` option.
   */
  runCount?: number;
}

export interface SearchResponse {
  /**
   * The total number of results in this response
   */
  totalResults: number;

  /**
   * The search results
   */
  results: SearchResult[];
}

/**
 * Run an Everything search with the given query and options.
 * See https://www.voidtools.com/support/everything/searching/ for query syntax.
 * @throws Throws for invalid options
 */
export function search(
  query: string,
  options: SearchOptions = {}
): SearchResponse {
  const allOptions = Object.assign(
    {
      offset: 0,
      limit: 100,
      matchCase: false,
      matchWholeWord: false,
      matchPath: false,
      enableRegex: false,
      includeSize: false,
      includeDateCreated: false,
      includeDateModified: false,
      includeDateAccessed: false,
      includeRunCount: false,
      sortAsc: true,
      sortBy: 'name',
    },
    options
  );

  return addon.search(query, allOptions);
}
