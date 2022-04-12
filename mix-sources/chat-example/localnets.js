const { networkInterfaces } = require('os');

const nets = networkInterfaces();
const results = Object.create(null); // or just '{}', an empty object

//console.log(nets);
function getlocalips()
{
	
for (const name of Object.keys(nets)) {
	if (name.indexOf("VMware Network Adapter") == -1)
	{
    for (const net of nets[name]) {
        // skip over non-ipv4 and internal (i.e. 127.0.0.1) addresses
        if (net.family === 'IPv4' && !net.internal) {
            if (!results[name]) {
                results[name] = [];
            }

            results[name].push(net.address);
        }
    }
	}
  }
  return results;
}

/*console.log("result");
for (const name of Object.keys(results))  {
   console.log( name + ": " + results[name]);
}
*/

module.exports = {getlocalips};