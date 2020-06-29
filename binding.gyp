{
  'targets': [
    {
      'target_name': 'nexutil_lib',
      'sources': [
        'src/nex_tpdecryptstr.c'
      ]
      ,"libraries": [
            "-l m",
            "-l ubf",
            "-l nstd",
            "-l atmi",
        ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'include_dirs' : [
          "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      "conditions":[
      	["OS=='linux'", {
		"libraries": [
            		"-l rt",
        	]
      	  }],
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
