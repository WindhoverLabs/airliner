'use strict';

var path = require('path');

const CdrFlightAppsPlugin = require(path.join(global.CDR_INSTALL_DIR, '/commander/classes/CdrFlightAppsPlugin')).CdrFlightAppsPlugin;

module.exports = class CfeCdrFlightAppsPlugin extends CdrFlightAppsPlugin {
    constructor(config) {
        config.name = 'mpu6050';
        config.webRoot = path.join( __dirname, 'web');  
        super(config);

        var content = {
            mpu6050: {   
                shortDescription: 'MPU6050',
                longDescription: 'MPU6050 Driver.',
                nodes: {
				    main: {
					    type: CdrFlightAppsPlugin.ContentType.LAYOUT,
					    shortDescription: 'Main',
                        longDescription: 'Main MPU6050.',
                        filePath: 'main.lyt'
				    },
				    hk: {
					    type: CdrFlightAppsPlugin.ContentType.PANEL,
					    shortDescription: 'Housekeeping',
					    longDescription: 'MPU6050 Housekeeping.',
					    filePath: 'hk.pug'
				    },
                    diag: {
                        type: CdrFlightAppsPlugin.ContentType.PANEL,
                        shortDescription: 'Diag',
                        longDescription: 'MPU6050 Diag',
                        filePath: 'diag.pug'
                    }
                }
            }
        };
        
      	this.addContent(content);
    };
};