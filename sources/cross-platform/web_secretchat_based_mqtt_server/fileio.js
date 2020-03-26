//$("#bfileimport").click(function() { //点击导入按钮，使files触发点击事件，然后完成读取文件的操作。
//    $("#filein").click();
//});

function fimport(evt) {
    var selectedFile = document.getElementById("filein").files[0]; //获取读取的File对象
    var name = selectedFile.name; //读取选中文件的文件名
    var size = selectedFile.size; //读取选中文件的大小
    console.log("文件名:" + name + "大小：" + size);

    var reader = new FileReader(); //这里是核心！！！读取操作就是由它完成的。
    reader.readAsText(selectedFile); //读取文件的内容

    reader.onload = function() {
        console.log(this.result); //当读取完成之后会回调这个函数，然后此时文件的内容存储到了result中。直接操作即可。
    };
}

function fimportpeerkey(evt) {
    var selectedFile = document.getElementById("peerkeyfilein").files[0]; 
    var name = selectedFile.name; 
    var size = selectedFile.size; 
    
    var reader = new FileReader(); 
    reader.readAsText(selectedFile); 

    reader.onload = function() {
        //console.log(this.result); 
        var peerj = JSON.parse(forge.util.decode64(this.result));
        console.log(peerj);
        if(peerj && peerj.publickey && peerj.aliasn && window.curretrsakey.publickey !== peerj.publickey ){
            peerj.mdtit = getmd5key(peerj.publickey);
            window.peerinfos.push(peerj);
            peer_addto_select_items(peerj);
        }
    };
}

function fkloadclick(evt){
    document.getElementById('filein').click();
}

function fileioonload() {
    console.log("load fielin onload");
    document.getElementById("filein").addEventListener('change', fimport, false);
    document.getElementById("peerkeyfilein").addEventListener('change', fimportpeerkey, false);
    document.getElementById("loadKeyFile").addEventListener('click', fkloadclick);
    document.getElementById("bfileout").addEventListener('click', fsavetofile);
}

function clickPeerKeybutton() {
    document.getElementById('peerkeyfilein').click();
}

function fsavetofile(evt) {
    result = prompt("保存配置", "wsecchat-config");
    if(result) {
        //saveTextAsFile(result);
    }
}

function fsavepublicinfo(publicaccount, name) {
    result = prompt("保存帐号公开密钥", "wsecchat-public-key-" + name);
    if(result) {
        saveTextAsFile(result, publicaccount);
    }
}


function saveTextAsFile(fileNameToSaveAs, textdata) {
    var textToWrite = textdata;//document.getElementById('textAreaToFile').value;//innerHTML
    console.log("text is:" + textToWrite);
    var textFileAsBlob = new Blob([ textToWrite ], { type: 'text/plain' });
    //var fileNameToSaveAs = "ecc.plist";
  
    var downloadLink = document.createElement("a");
    downloadLink.download = fileNameToSaveAs;
    downloadLink.innerHTML = "Download File";
    if (window.webkitURL != null) {
      // Chrome allows the link to be clicked without actually adding it to the DOM.
      downloadLink.href = window.webkitURL.createObjectURL(textFileAsBlob);
    } else {
      // Firefox requires the link to be added to the DOM before it can be clicked.
      downloadLink.href = window.URL.createObjectURL(textFileAsBlob);
      downloadLink.onclick = destroySaveFileClickedElement;
      downloadLink.style.display = "none";
      document.body.appendChild(downloadLink);
    }
  
    downloadLink.click();
}

function destroySaveFileClickedElement(event) {
    // remove the link from the DOM
    document.body.removeChild(event.target);
}

function fexport() {
    uricont = "data:application/octet-stream," + "my conttestsa";
    newWind = window.open(uricont, 'savedoc');
}