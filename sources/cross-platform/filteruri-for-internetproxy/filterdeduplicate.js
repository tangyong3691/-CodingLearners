


function testlog(v) {
    console.log(v);
}

function filterAndDeduplicateLinks() {
    // 获取输入和输出的textarea元素
     const inputTextarea = document.getElementById('myTextarea');
    const outputTextarea = document.getElementById('outputTextarea');
    
    // 获取文本内容并按行分割
    const lines = inputTextarea.value.split('\n');
    
    // 用于存储已经出现的@...?模式文本
    const seenPatterns = new Set();
    // 用于存储不重复的结果
    const uniqueResults = [];
    
    // 首先筛选vass://或fass://开头的行
    const filteredLinks = lines.filter(line => {
		console.log(line);
        const trimmedLine = line.trim();
		if (trimmedLine.startsWith('vless://') || trimmedLine.startsWith('trojan://')) {
        return ( 
		(trimmedLine.includes('%e7%be%8e%e5%9b%bd') || trimmedLine.includes('%E7%BE%8E%E5%9B%BD') || trimmedLine.includes('#US_')|| trimmedLine.includes('_US_') ) && trimmedLine.includes(':443?'));
		} 
		else if (trimmedLine.startsWith('vmess://'))
		{
		   testlog(trimmedLine);
		   const vmessdestr = Base64.decode(trimmedLine.slice(8));
		   if (!vmessdestr) return false;
           const dejson = JSON.parse(vmessdestr);
		   const regexipport = /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?):(?:[1-9][0-9]{0,4})$/;
		   const regexip = /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
           if (	dejson.hasOwnProperty('add') && dejson.hasOwnProperty('ps')	&& dejson.hasOwnProperty('port') ) {
		       if ( (dejson.ps.includes('US_') || dejson.ps.includes('美国') )&& dejson.port == 443 && regexip.test(dejson.add)) return true;
		   }
		   return false;
		   
		}
		else {
		  return false;
		}
    });
    
    // 进一步处理筛选出的行
    filteredLinks.forEach(link => {
        // 提取@...?模式的文本
		if(link.startsWith('vless://') || link.startsWith('trojan://')) {
        const match = link.match(/@([^?]+)\?/);
        if (match) {
            const pattern = match[1]; // 获取@和?之间的文本
			
            const regexipport = /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?):(?:[1-9][0-9]{0,4})$/;
            // 如果这个模式之前没有出现过，就添加到结果中
			
            if ( regexipport.test(pattern) && !seenPatterns.has(pattern)) {
                seenPatterns.add(pattern);
                uniqueResults.push(link);
            }
        } else {
            // 如果链接中没有@...?模式，直接添加到结果中
            //uniqueResults.push(link);
        }
		}
		else {
		   const dejson = JSON.parse(Base64.decode(link.slice(8)));
		   const lkurl= dejson.add + dejson.port;
		   if (  !seenPatterns.has(lkurl)) {
                seenPatterns.add(lkurl);
                uniqueResults.push(link);
            }
		   
		
		}
    });
    
    // 将结果写入输出textarea
    outputTextarea.value = uniqueResults.join('\n');
}