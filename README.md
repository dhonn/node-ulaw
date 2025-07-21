# node‑ulaw

**Blazing‑fast, native G.711 µ‑law encoder/decoder for Node.js**, built with a precomputed 256‑entry lookup table and optimized C++/N‑API binding for minimal overhead and maximum throughput.

## Key Features

- **Native‑speed performance**  
  Uses a single memory lookup per sample for decoding and minimal bit‑operations for encoding—no branching or floating‑point math in the hot path.

- **Zero‑copy integration**  
  Buffers flow directly between JavaScript and C++ with no extra copies, keeping latency low.

- **Small footprint**  
  The entire codec lives in a few C files and a 1 KB static table, with no heavy external dependencies.

- **Automatic build**  
  Compiles on install via `node-gyp` with a single command; works across Linux, macOS, and Windows (with build tools).


## Installation
```bash
npm install node-ulaw
```

**Prerequisites**

This package includes a native addon built with `node-gyp`. You’ll need a C/C++ toolchain and Python installed:

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential python3
```

### macOS
```bash
xcode-select --install
brew install python
```

On install, the addon will compile automatically via the `install` script.

## Usage

```js
const { decodeUlaw, encodeUlaw } = require('node-ulaw');
```

`decodeUlaw(buffer: Buffer<UInt8>) → Buffer<Int16LE>`
- **Input**: Buffer of 8‑bit G.711 µ‑law samples
- **Output**: Buffer of little‑endian 16‑bit signed PCM samples

```js
const fs = require('fs');
const { decodeUlaw } = require('node-ulaw');

const ulaw = fs.readFileSync('input.ulaw');
const pcm  = decodeUlaw(ulaw);
console.log(`Decoded ${pcm.length/2} samples`);
fs.writeFileSync('output.pcm', pcm);
```
#

`encodeUlaw(buffer: Buffer<Int16LE>) → Buffer<UInt8>`
- **Input**: Buffer of little‑endian 16‑bit signed PCM samples
- **Output**: Buffer of 8‑bit G.711 µ‑law samples

```js
const fs = require('fs');
const { encodeUlaw } = require('node-ulaw');

const pcm  = fs.readFileSync('output.pcm');
const ulaw = encodeUlaw(pcm);
fs.writeFileSync('reencoded.ulaw', ulaw);
```

## Streaming Usage
For large files or real‑time streams, wrap in a Transform:

```js
const { Transform } = require('stream');
const { decodeUlaw } = require('node-ulaw');

class UlawDecoder extends Transform {
  _transform(chunk, _, cb) {
    try {
      this.push(decodeUlaw(Buffer.from(chunk)));
      cb();
    } catch (err) {
      cb(err);
    }
  }
}

// Usage:
fs.createReadStream('input.ulaw')
  .pipe(new UlawDecoder())
  .pipe(fs.createWriteStream('output.pcm'));
```


## WAV Output Example
Wrap raw PCM in a playable WAV:

```js
const fs = require('fs');
const wav = require('wav-encoder');
const { decodeUlaw } = require('node-ulaw');

async function ulawToWav(inPath, outPath) {
  const ulawBuf = fs.readFileSync(inPath);
  const pcmBuf  = decodeUlaw(ulawBuf);
  const samples = new Int16Array(pcmBuf.buffer);
  const wavData = await wav.encode({
    sampleRate: 8000,
    channelData: [samples]
  });
  fs.writeFileSync(outPath, Buffer.from(wavData));
}

ulawToWav('input.ulaw', 'output.wav');
```


## Building From Source
If you clone the repo and want to rebuild:
```bash
npm install
npx node-gyp clean
npx node-gyp configure build
```

## Contributing
1. Fork the repository.
2. Create a feature branch: git checkout -b feature/your-feature
3. Build & test your changes.
4. Submit a pull request.

## License
(The MIT License)

Copyright (c) 2025 Dhonn Lushine

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.