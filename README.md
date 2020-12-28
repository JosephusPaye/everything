# everything

> A native Node.js module for fast programmatic access to [Everything](https://voidtools.com) search on Windows.

This project is part of [#CreateWeekly](https://twitter.com/JosephusPaye/status/1214853295023411200), my attempt to create something new publicly every week in 2020.

## Requirements

- A 64-bit version of Windows
- Node.js v10 or later
- [Everything](https://voidtools.com) 1.4 or later, installed and running when used

## Installation

```
npm install @josephuspaye/everything --save
```

## Usage

## Run a basic search

```js
const { search } = require('@josephuspaye/everything');

// Search for files with "report.pdf" in their name, with the defualt options
const response = search('report.pdf');

console.log(JSON.stringify(response, null, '  '));
```

<details>
<summary>View output</summary>

```json
{
  "totalResults": 2,
  "results": [
    {
      "type": "file",
      "name": "JosephusPayeII-Work_Integrated_Learning_Final_Report.pdf",
      "dir": "C:\\Users\\jpaye\\OneDrive\\Documents"
    },
    {
      "type": "file",
      "name": "Report.pdf",
      "dir": "C:\\Users\\jpaye\\OneDrive\\Documents\\UON\\SENG2130\\Assignment 2\\Compilation"
    }
  ]
}
```

</details>

## Include additional metadata with results

```js
const { search } = require('@josephuspaye/everything');

// Search for files with "report.pdf" in their name, including size and date modified
const response = search('report.pdf', {
  includeSize: true,
  includeDateModified: true,
});

console.log(JSON.stringify(response, null, '  '));
```

<details>
<summary>View output</summary>

```json
{
  "totalResults": 2,
  "results": [
    {
      "type": "file",
      "name": "JosephusPayeII-Work_Integrated_Learning_Final_Report.pdf",
      "dir": "C:\\Users\\jpaye\\OneDrive\\Documents",
      "size": 2367729,
      "dateModified": 1596784835000
    },
    {
      "type": "file",
      "name": "Report.pdf",
      "dir": "C:\\Users\\jpaye\\OneDrive\\Documents\\UON\\SENG2130\\Assignment 2\\Compilation",
      "size": 2247789,
      "dateModified": 1495810102000
    }
  ]
}
```

</details>

## Use regular expression in query, sort, and limit results

```js
const { search } = require('@josephuspaye/everything');

// Find the top 10 largest files using a regular expression,
// sorted in descending order of size
const response = search('.*', {
  enableRegex: true,
  includeSize: true,
  sortBy: 'size',
  sortAsc: false,
  limit: 10,
});

console.log(JSON.stringify(response, null, '  '));
```

<details>
<summary>View output (partial)</summary>

```json
{
  "totalResults": 10,
  "results": [
    {
      "type": "file",
      "name": "ext4.vhdx",
      "dir": "C:\\Users\\jpaye\\AppData\\Local\\Docker\\wsl\\data",
      "size": 30343692288
    },
    {
      "type": "file",
      "name": "hiberfil.sys",
      "dir": "C:",
      "size": 13712793600
    },
    {
      "type": "file",
      "name": "pagefile.sys",
      "dir": "C:",
      "size": 10472558592
    }
    // ...
  ]
}
```

</details>

## API

```ts
interface SearchOptions {
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

interface SearchResult {
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

interface SearchResponse {
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
function search(query: string, options: SearchOptions = {}): SearchResponse;
```

## What's Next

- Add an async version of `search()`
- Add a function to launch Everything with the query entered (using an `es://` URL)
- Add support for the `Everything_GetRunCountFromFileName()`, `Everything_SetRunCountFromFileName()` and `Everything_IncRunCountFromFileName()` functions

## Licence

[MIT](LICENCE)
