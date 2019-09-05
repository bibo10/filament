# matdbg

1. [Overview](#overview)
1. [C++ Server](#c++-server)
1. [JavaScript Client](#javascript-client)
1. [HTTP Requests](#http-requests)
1. [WebSocket Requests](#websocket-requests)
1. [Wish List](#wish-list)
1. [Screenshot](#screenshot)
1. [Material Chunks](#material-chunks)

## Overview

The matdbg library is responsible for instancing a [civetweb][1] server that handles HTTP and
WebSocket requests, and it provides a small web app that contains a view into an in-browser database
of materials.

The WebSocket server receives push-style notifications from the client (such as edits) while
the HTTP server responds to material queries using simple JSON messages.

When a new WebSocket connection is established, the client asks the server for a list of materials
in order to populate its in-browser database. If the connection is lost (e.g. if the app crashes),
then the database stays intact and the web app is still functional. If a new Filament app is
launched, the client inserts entries into its database rather than replacing the existing set.

The material database is cleared only when the web page is manually refreshed by the user.

## C++ Server

The civetweb server is wrapped by our `DebugServer` class, whose public interface is comprised of a couple methods
that are called from the Filament engine:

- **addMaterial** Notifies the debugger that the given material package is being loaded into the
  engine.
- **setEditCallback** Sets up a callback that allows the Filament engine to listen for shader edits.

## JavaScript Client

The web app is written in simple, modern JavaScript and avoids frameworks like React or Angular. It
uses the following third-party libraries which are downloaded directly from a CDN. This allows us to
avoid adding them to our git repo, and gives good caching behavior.

- **mustache** Converts template strings into HTML.
- **monaco** The engine behind Visual Studio Code.
    - We've configured this for C++, which provides good syntax highlighting for MSL and often looks
      reasonable for GLSL.
    - If desired we could extend the editor to better handle GLSL and SPIR-V.

All the source code for the web app is contained in a single file (`script.js`) and the mustache
template strings are specified using `<template>` tags in the `index.html`.

The web app basically provides a view over a poor man's "database" which is a just a global variable
that holds a dictionary that maps from material id's to objects that conform to the JSON described below.

## HTTP requests

TODO: material ids, JSON

## WebSocket requests

TODO: commands

## Wish List

- Add [railroad diagrams](https://github.com/tabatkins/railroad-diagrams) to this doc.
- Show which shader variants are actually being used in real time (perhaps by polling the WebSocket
  every second), since it is often difficult to find the "active" shaders.
- Allow SPIR-V edits.
- Expose the entire `engine.debug` struct in the web UI.
- When shader errors occur, send them back over the wire to the web client.
- Resizing the Chrome window causes layout issues.
- The sidebar in the web app is not resizeable.
- Refactor shader selection stuff to have "index" and "stage" attributes instead of glindex/vkindex/metalindex
    - Alternatively do something similar to makeKey in `MaterialChunk`
- For the material ids, SHA-1 would be better than murmur since the latter can easily have collisions.
- It would be easy to add diff decorations to the editor in our `onEdit` function:
     1. Examine "changes" (IModelContentChange) to get a set of line numbers.
     2. `shader.decorations = gEditor.deltaDecorations(shader.decorations, ...)`
     3. See [these monaco docs](https://microsoft.github.io/monaco-editor/playground.html#interacting-with-the-editor-line-and-inline-decorations).

## Screenshot

<img width="600px" src="https://user-images.githubusercontent.com/1288904/63553241-b043ba80-c4ee-11e9-816c-c6acb1d6cdf7.png">

[1]: https://github.com/civetweb/civetweb
[2]: https://microsoft.github.io/monaco-editor/
[3]: https://developer.mozilla.org/en-US/docs/Web/HTML/Element/template

## Material Chunks

This section is only tangentially related to matdbg but provides a reference for the
`ShaderExtractor` and `ShaderReplacer` features.

The relevant chunk types are listed here. These types are defined in the `filabridge` lib, in
the `filamat` namespace.

```c++
enum UTILS_PUBLIC ChunkType : uint64_t {
    ...
    MaterialGlsl = charTo64bitNum("MAT_GLSL"),    // MaterialTextChunk
    MaterialSpirv = charTo64bitNum("MAT_SPIR"),   // MaterialSpirvChunk
    MaterialMetal = charTo64bitNum("MAT_METL"),   // MaterialTextChunk
    ...
    DictionaryGlsl = charTo64bitNum("DIC_GLSL"),  // DictionaryTextChunk
    DictionarySpirv = charTo64bitNum("DIC_SPIR"), // DictionarySpirvChunk
    DictionaryMetal = charTo64bitNum("DIC_METL"), // DictionaryTextChunk
    ...
}
```

### MaterialTextChunk

These chunks have the following layout.

    [u64] FourCC code for ChunkType
    [u32] Remaining chunk size in bytes
    [u64] Shader count
    for each shader:
        [u8]  Shader model
        [u8]  Shader variant
        [u8]  Shader stage
        [u32] Offset in bytes from (and including) "Shader count" to first line index
    for each shader:
        [u32] Total string size including null terminator
        [u32] Number of line indices
        [u16 u16 u16...] Line indices

### DictionaryTextChunk

These chunks have the following layout.

    [u64] FourCC code for ChunkType
    [u32] Remaining chunk size in bytes
    [u32] Number of strings
    for each string:
        [u8 u8 u8 u8...] include null terminator after each string

### DictionarySpirvChunk

These chunks have the following layout.

    [u64] FourCC code for ChunkType
    [u32] Remaining chunk size in bytes
    [u32] Compression
    [u32] Blob count
    for each blob:
        [u64] Byte count
        [u8 u8 u8 ...]
