'use strict';

var path = require( 'path' );

const CdrPlugin = require( path.join( global.CDR_INSTALL_DIR, '/commander/classes/CdrPlugin' ) ).CdrPlugin;

module.exports = class CfeCdrPlugin extends CdrPlugin {
  constructor( urlBase ) {
    super( 'sg33bl', path.join( __dirname, 'web', urlBase ) );
  }

  getContent() {
    var result = {
      shortDescription: 'SG33BL Servo',
      longDescription: 'SG33BL Servo Driver.',
      nodes: {
        main: {
          type: CdrPlugin.ContentType.LAYOUT,
          shortDescription: 'Main',
          longDescription: 'Main SG33BL.',
          filePath: '/main.lyt'
        },
        cdh: {
          type: CdrPlugin.ContentType.PANEL,
          shortDescription: 'Command and Data Handling',
          longDescription: 'Command counters.',
          filePath: '/cdh.pug'
        },
        diag: {
          type: CdrPlugin.ContentType.PANEL,
          shortDescription: 'Diagnostic',
          longDescription: 'SG33BL Diagnostic Message',
          filePath: '/diag.pug'
        }
      }
    };

    return result;
  }
};

//var plugin = new CfeCdrPlugin();

//let CfeCdrPlugin = new class extends CdrPlugin {
