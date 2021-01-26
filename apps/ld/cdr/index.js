'use strict';

var path = require( 'path' );

const CdrFlightAppsPlugin = require( path.join( global.CDR_INSTALL_DIR, '/commander/classes/CdrFlightAppsPlugin' ) ).CdrFlightAppsPlugin;

module.exports = class CfeCdrFlightAppsPlugin extends CdrFlightAppsPlugin {
    constructor(config) {
        config.name = 'ld';
        config.webRoot = path.join( __dirname, 'web');  
        super(config);
    
        var content = {
            ld: {
                shortDescription: 'Land Detector',
                longDescription: 'Land Detector.',
                nodes: {
                    main: {
                        type: CdrFlightAppsPlugin.ContentType.LAYOUT,
                        shortDescription: 'Main',
                        longDescription: 'Main LD.',
                        filePath: 'main.lyt'
                    },
                    cdh: {
                        type: CdrFlightAppsPlugin.ContentType.PANEL,
                        shortDescription: 'Command and Data Handling',
                        longDescription: 'Command counters.',
                        filePath: 'cdh.pug'
                    },
                    diag: {
                        type: CdrFlightAppsPlugin.ContentType.PANEL,
                        shortDescription: 'Diagnostic',
                        longDescription: 'Diagnostic',
                        filePath: 'diag.pug'
                    }
                }
            }
        };
        
      	this.addContent(content);
    };
};
