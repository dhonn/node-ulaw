{
  "targets": [
    {
      "target_name": "ulaw",
      "sources": [ "addon.cc", "decode.c", "encode.c" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS=1" ]
    }
  ]
}
