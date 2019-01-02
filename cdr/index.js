'use strict';

var path = require( 'path' );

const CdrPlugin = require( path.join( global.CDR_INSTALL_DIR, '/commander/classes/CdrPlugin' ) ).CdrPlugin;

module.exports = class CfeCdrPlugin extends CdrPlugin {
  constructor( urlBase ) {
    super( 'vm', path.join( __dirname, 'web', urlBase ) );
  }

  getContent() {
    var result = {
      shortDescription: 'Vehicle Manager',
      longDescription: 'Vehicle State Manager.',
      nodes: {
        main: {
          type: CdrPlugin.ContentType.LAYOUT,
          shortDescription: 'Main',
          longDescription: 'Main VM.',
          filePath: '/main.lyt'
        },
        cdh: {
          type: CdrPlugin.ContentType.PANEL,
          shortDescription: 'Command and Data Handling',
          longDescription: 'Command counters.',
          filePath: '/cdh.pug'
        },
        vs: {
          type: CdrPlugin.ContentType.PANEL,
          shortDescription: 'Vehicle State',
          longDescription: 'Arming and Navigation States',
          filePath: '/vs.pug'
        }
      }
    };

    return result;
  }
};

//var plugin = new CfeCdrPlugin();

//let CfeCdrPlugin = new class extends CdrPlugin {
