const path = require('path');
// Directly load the built .node file:
const { decodeUlaw, encodeUlaw } = require(path.join(__dirname, 'build/Release/ulaw.node'));

module.exports = { decodeUlaw, encodeUlaw };
