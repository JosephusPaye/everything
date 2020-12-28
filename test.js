const { search } = require('.');

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
