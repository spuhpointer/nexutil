'use strict';

var binary = require('node-pre-gyp');
var path = require('path');
var binding_path = binary.find(path.resolve(path.join(__dirname, './package.json')));
var bindings = require(binding_path);
var promises = require('./promises');

/// generate a salt (sync)
/// @param {String} [encstr] encrypted string, output from $ exencrypt
/// @return {String} salt
module.exports.tpdecryptstr = function tpdecryptstr(encstr) {
    return bindings.tpdecryptstr(encstr);
};

