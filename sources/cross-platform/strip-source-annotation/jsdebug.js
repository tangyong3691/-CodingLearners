var jsfDbg = null;
try{
   var jsfDbg = require("debug");
}catch(e){
    console.log('tips: debug disable');
}

function jsNoDebug(){

}
function getDebug(mod) {
    if(jsfDbg != null){
        //console.log('debug...');
        return jsfDbg(mod);
    }
    return jsNoDebug;
} 

exports.getDebug = getDebug;


