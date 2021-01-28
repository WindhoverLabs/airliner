var path = require('path');
var fs = require('fs');

var Commander       = require(path.join(global.CDR_INSTALL_DIR, 'commander'));
var BinaryEncoder   = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'binary-encoder'));
var BinaryDecoder   = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'binary-decoder'));
var UdpStdProvider  = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'data-providers', 'udp-std-provider'));
var VariableServer  = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'variable-server'));
var ProtobufEncoder = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'protobuf-encoder'));
var ProtobufDecoder = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'protobuf-decoder'));
var ConfigDatabase  = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'config-database'));
var EventRecorder   = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'event-recorder'));
var VideoServer     = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'video-server'));
var CfdpServer      = require(path.join(global.CDR_INSTALL_DIR, 'plugins', 'cfdp-server'));

var commander = new Commander(global.CDR_WORKSPACE, `${global.CDR_WORKSPACE}/etc/commander-config.json`);

global.COMMANDER = commander;

var airliner = commander.addInstance('airliner', function(namespace) {
    commander.setDefaultInstance(namespace);
    
    var binaryEncoder    = new BinaryEncoder({namespace: namespace, name: 'binary-encoder', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/binary-encoder-config.json`});
    var binaryDecoder    = new BinaryDecoder({namespace: namespace, name: 'binary-decoder', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/binary-decoder-config.json`});
    var variableServer   = new VariableServer({namespace: namespace, name: 'variable-server', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/variable-server-config.json`});
    var fswConnector     = new UdpStdProvider({namespace: namespace, name: 'fsw-connector', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/udpstdprovider-config.json`});
    var pylinerConnector = new UdpStdProvider({namespace: namespace, name: 'pyliner-connector', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/pyliner-connector-config.json`});
    var protobufEncoder  = new ProtobufEncoder({namespace: namespace, name: 'protobuf-encoder', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/protobuf-encoder-config.json`});
    var protobufDecoder  = new ProtobufDecoder({namespace: namespace, name: 'protobuf-decoder', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/protobuf-decoder-config.json`});
    var configDB         = new ConfigDatabase({namespace: namespace, name: 'config-database', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/config-database-config.json`});
    var eventRecorder    = new EventRecorder({namespace: namespace, name: 'event-recorder', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/event-recorder-config.json`});
    var videoServer      = new VideoServer({namespace: namespace, name: 'video-server', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/video-server-config.json`});
    var cfdpServer       = new CfdpServer({namespace: namespace, name: 'cfdp-server', workspace: global.CDR_WORKSPACE, configFile: `${global.CDR_WORKSPACE}/etc/cfdp-config.json`});
	
    var outFiles = [];
    var fullPath = path.join(global.CDR_WORKSPACE, 'plugins');
    
    parsePluginPath(namespace, path.join(fullPath, 'cfe/index.js'), '/');
    findIndex(path.join(fullPath, 'apps'), function(err, results) {
    	if(err) {
    	    throw err;
    	}
    	
    	for(var i = 0; i < results.length; ++i) {
            parsePluginPath(namespace, results[i], '/');
    	}
    });
});



function parsePluginPath(instance, indexFilePath, basePath) {
    if (fs.existsSync(indexFilePath)) {
        var config = {namespace: instance};
        var NewPluginClass = require(path.dirname(indexFilePath));
        var newPlugin = new NewPluginClass(config);
        
        newPlugin.initialize(config);
    } else {
	parsePluginPath(pluginPath, newBasePath);
    }
}



function findIndex(dir, cb) {
	var results = [];
	fs.readdir(dir, function(err, list) {
		if (err) return cb(err);
		var i = 0;
		(function next() {
			var file = list[i++];
			if (!file) return cb(null, results);
			file = dir + '/' + file;
			fs.stat(file, function(err, stat) {
				if (stat && stat.isDirectory()) {
					findIndex(file, function(err, res) {
						results = results.concat(res);
						next();
					});
				} else {
					if(path.basename(file) === 'index.js') {
						results.push(file);
						return cb(null, results)
					} 
					next();
				}
			});
		})();
	});
};
