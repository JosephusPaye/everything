{
  "targets": [
    {
      "target_name": "everything",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "sources": ["src/*.cpp"],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "everything"
      ],
      "libraries": [ "<(module_root_dir)/everything/lib/Everything64.lib" ],
      "copies": [
        {
          "destination": "<(module_root_dir)/build/Release/",
          "files": [ "<(module_root_dir)/everything/dll/Everything64.dll" ]
        }
      ],
    }
  ]
}
