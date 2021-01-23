{
  "targets": [
    {
      "target_name": "IconExtractor",
      "includes": [
        "auto.gypi"
      ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include_dir\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ], 
      "sources": [
        "src/IconExtractor.cpp"
      ],
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1
        }
      },
      "conditions": [
        ['OS=="win"', {
          'sources': [
	    "src/IconExtractorWindows.cpp"
          ],
          "libraries": [
            "-lGdiplus"
          ],
        }],
        ['OS=="linux"', {
          'sources': [
            "src/IconExtractorLinux.cpp"
          ],
          "cflags!": [ "-fno-exceptions" ],
          "cflags_cc!": [ "-fno-exceptions" ],
          "cflags": [
            "-fexceptions"
          ],
          "cflags_cc": [
            "-fexceptions"
          ]
        }]
      ]
    }
  ]
}
