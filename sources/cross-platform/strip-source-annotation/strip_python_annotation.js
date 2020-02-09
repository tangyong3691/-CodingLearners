const fs = require('fs');

console.log('python source strip annotation start..');
console.log('Usage:  argument:  -s <src> -d <dst>');

var debugstrip = require("./jsdebug.js").getDebug('STRIP');
var argv = require('optimist').argv;
debugstrip('%o', argv);
var src_dirname = argv.s;
var dst_dirname = argv.d;

debugstrip('src:%s', src_dirname || 'null');
debugstrip('dst:%s', dst_dirname || 'null');

if(!fs.existsSync(src_dirname))
{
  console.log("file isn't exist:" + src_dirname);
}

/*if(fs.existsSync(dst_dirname))
{
   console.log("file exist:" + dst_dirname);
}*/

if ( src_dirname && dst_dirname /* && (!fs.existsSync(dst_dirname)) */ && fs.existsSync(src_dirname) )
{

var lineReader = require('readline').createInterface({
  input: require('fs').createReadStream(src_dirname)
});

var writefl = require('fs').createWriteStream(dst_dirname, {
  flags: 'w' //'a' 'a' means appending (old data will be preserved)
})

var debugline = require("./jsdebug.js").getDebug('LINE');
var multianflag = false;
var multisign = 1;
var mutilleaved = false;
var lasttailassign = false;
lineReader.on('line', function (line) {
  debugline('Line from file: %s', line);
  var skipl = false;
  var issame = false;
  var mutilleaended = false;
  var mutilstartingstr = false;
  var lastmultianflag = multianflag;
  var lineisempty = false;
/* when is is empty line, remove*/
  if(!line.trim()) { 
     skipl = true; 
     lineisempty = true;
   }

  var findan1 = line.indexOf('#');
  if (findan1 >= 0) {
      if (!line.substring(0,findan1).trim()) { skipl = true; }
  }
  
  var findan2 = line.indexOf("'''");
  var findan3 = findan2;
  if (findan3 < 0) { findan3 = line.indexOf('"""'); }

   if (findan3 >= 0) {
       //var lastmulea = mutilleaved;
      var stleft = line.substring(0, findan3).trim();
        debugline('Line befor """: %s', stleft);
       if  ( ((!stleft) && lasttailassign) ||  (stleft.length && stleft.lastIndexOf("=") == stleft.length - 1) ) { 
            if(!mutilleaved) mutilleaved = true; 
              if(!lastmultianflag) mutilstartingstr = true;
        }
       //if (lastmulea) { mutilleaended = true;}
   }
  
  if (findan3 >= 0) {
       var f2s = -1;
      if (findan2 >= 0) {f2s = line.lastIndexOf("'''");}
        else { f2s = line.lastIndexOf('"""'); }
      if (f2s > 0 && f2s != findan3) { 
          debugline('Line same li """: %s', line);
         issame = true; 
            if (!mutilleaved) {
             if (f2s + 3 == line.length) { line = line.substring(0, findan3); }
              else { line = line.substring(0, findan3) + line.slice(f2s+3,  -1); }
               if(!line.trim()) {skipl = true;}
               }else {
                    mutilleaved = false;
                }
           }
        
   }

  debugline('Line mutilleaved : %o', mutilleaved);

  if( (!issame) && (!skipl) && (!lastmultianflag) && findan3 >= 0 && (mutilstartingstr || (!line.substring(0,findan3).trim()) ) )
  {
     multianflag = true;
      if (findan2 >= 0) { multisign = 1 } 
        else { multisign = 2}
      if(!mutilleaved){
      skipl = true;}
  }

   debugline('Line multianflag : %o', multianflag);
   debugline('Line findan3 : %d', findan3);
   debugline('Line lastmultianflag : %o', lastmultianflag);

  if( (!issame) && lastmultianflag && findan3 < 0) {
      debugline('Line no """ and befor has """ ');
      if(!mutilleaved) {skipl = true;}
   }

  if( (!issame) && lastmultianflag && ((multisign == 1 && findan2 >= 0) || (multisign == 2 && findan3 >= 0)) )
   {
      multianflag = false;
       if (!mutilleaved) {
       if (findan3 + 3 == line.length) { skipl = true }
        else if (!line.slice(findan3 + 3, -1).trim()) { skipl = true }
         else { line = line.slice(findan3 + 3, -1)  }
       }
      mutilleaved = false;
   }
   
   debugline('Line multianflag fin: %o', multianflag);
  
   if (!lineisempty) {
       lasttailassign = false;
       if(!skipl) {
           var nstr = line.trim();
            if(nstr.charAt(nstr.length - 1) == '=') {
                  debugline('find Line tail is =');
                  lasttailassign = true;
              }
        }  
   }
   

  if(!skipl) {
    writefl.write(line);
    writefl.write('\n');
   } else {
        debugline('----skip this line----');
    }
})
.on('close', function() {
  debugline('file end');
  writefl.end();
});


}
else {
  console.log('parameter is wrong');
}



