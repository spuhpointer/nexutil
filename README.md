# node.nexutil.js

A library to use Enduro/X tpdecrypt() function for data decryption

## Dependencies

* NodeJS
* `node-gyp`

## Install via NPM

```
npm install nexutil
```

## Usage

```javascript

const nexutil = require('nexutil');
console.log('Decrypted text:', nexutil.tpdecryptstr('AAAACzgTKee2vVcJ5pwuVvBegdk='))

```

## Testing


```
npm install
npm test
```

