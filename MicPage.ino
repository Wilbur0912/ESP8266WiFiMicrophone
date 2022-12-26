/*
 * WiFi Microphone program For ESP8266
 * Web Pages File
 * 
 * Author: M. Mahdi K. Kanan
 * WiCardTech Engineering Group
 * 
 * https://wicard.net
 * https://store.wicard.net
 * 
 * https://wicard.ir
 * https://shop.wicard.net
*/

void streamPage() //stream page: http://192.168.4.1/s
{
  unsigned int i,j,k,l = 0, uiSampleSize;

  digitalWrite(2, HIGH); //rev 1.3
  
  //client_stop = true;
  
  //rev1.2
  if(ucWMBitrate == 0)
  {
    /*Sample Rate - 8000*/
    buff[0x18] = 0x40;
    buff[0x19] = 0x1F;
  }
  else if(ucWMBitrate == 1)
  {
    /*Sample Rate - 9000*/
    buff[0x18] = 0x28;
    buff[0x19] = 0x23;
  }
  else //if(ucWMBitrate == 2)
  {
    /*Sample Rate - 10000*/
    buff[0x18] = 0x10;
    buff[0x19] = 0x27;
  }
  
  k = buff[0x19];
  k *= 256;
  k += buff[0x18];
  uiSampleSize = k * 2;
  if(ucWMPacketLen == 0)
    uiSampleSize *= (60 * 5);
  else if(ucWMPacketLen == 1)
    uiSampleSize *= (10);
  else if(ucWMPacketLen == 2)
    uiSampleSize *= (60 * 30);
  else //if(ucWMPacketLen == 3)
    uiSampleSize *= (60 * 60);

  //Server.sendHeader("Access-Control-Allow-Origin", "origin-list");
  //Server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  Server.setContentLength(uiSampleSize + 44);
  Server.send(200, "audio/wav", "");
  
  /*header*/
  /*id*/
  buff[0] = 0x52;
  buff[1] = 0x49;
  buff[2] = 0x46;
  buff[3] = 0x46;
  /*len-8*/
  buff[4] = (uiSampleSize+36)&0xFF; /*LSB*/
  buff[5] = ((uiSampleSize+36)/0x100)&0xff;
  buff[6] = ((uiSampleSize+36)/0x10000)&0xff;
  buff[7] = ((uiSampleSize+36)/0x1000000)&0xff;
  /*WAVE*/
  buff[8] = 0x57; /*LSB*/
  buff[9] = 0x41;
  buff[0xa] = 0x56;
  buff[0xb] = 0x45;
  /*Sign*/
  buff[0xc] = 0x66; /*LSB*/
  buff[0xd] = 0x6D;
  buff[0xe] = 0x74;
  buff[0xf] = 0x20;
  /*--*/
  buff[0x10] = 0x10;//12 /*LSB*/
  buff[0x11] = 0x00;
  buff[0x12] = 0x00;
  buff[0x13] = 0x00;
  /*PCM*/
  buff[0x14] = 0x01; /*LSB*/
  buff[0x15] = 0x00;
  /*CH - 1*/
  buff[0x16] = 0x01;
  buff[0x17] = 0x00;

  /* already set */
  
  /*-*/
  buff[0x1a] = 0x00;
  buff[0x1b] = 0x00;
  /* */
  buff[0x1c] = 0x40;
  buff[0x1d] = 0x1F;
  buff[0x1e] = 0x00;
  buff[0x1f] = 0x00;
  /* sample byte - 2*/
  buff[0x20] = 0x02;
  //buff[0x20] = 0x01;
  buff[0x21] = 0x00;
  /* bit per sample - 16*/
  buff[0x22] = 0x10;
  //buff[0x22] = 0x08;
  buff[0x23] = 0x00;
  /* data */
  buff[0x24] = 0x64;
  buff[0x25] = 0x61;
  buff[0x26] = 0x74;
  buff[0x27] = 0x61;

  /* len */
  buff[0x28] = uiSampleSize&0xff;
  buff[0x29] = (uiSampleSize/0x100)&0xff;
  buff[0x2a] = (uiSampleSize/0x10000)&0xff;
  buff[0x2b] = (uiSampleSize/0x1000000)&0xff;
  
  Server.sendContent_P((const char *)buff, 44);
  
  //rev 1.2
  if(ucWMBitrate == 0)
    ulTimer2 = micros() - AUDIO_80KBPS_TIMING;
  else if(ucWMBitrate == 1)
    ulTimer2 = micros() - AUDIO_90KBPS_TIMING;
  else //if(ucWMBitrate == 2)
    ulTimer2 = micros() - AUDIO_100KBPS_TIMING;

  for(k = 0; k < uiSampleSize; k++)
  {
    //rev 1.3
    for(i = 0; i < 2000;)
    {
      yield();

      ulTimer1 = micros();
      
      if((ulTimer1 - ulTimer2) > 1000)
        l = 0;
      else
      {
        if(l == 12000)
          break;

        l++;
        
        if(ucWMBitrate == 0)
        {
          if((ulTimer1 - ulTimer2) < AUDIO_80KBPS_TIMING)
            delayMicroseconds(AUDIO_80KBPS_TIMING - (ulTimer1 - ulTimer2));
        }
        else if(ucWMBitrate == 1)
        {
            if((ulTimer1 - ulTimer2) < AUDIO_90KBPS_TIMING)
              delayMicroseconds(AUDIO_90KBPS_TIMING - (ulTimer1 - ulTimer2));
        }
        else //if(ucWMBitrate == 2)
        {
            if((ulTimer1 - ulTimer2) < AUDIO_100KBPS_TIMING)
              delayMicroseconds(AUDIO_100KBPS_TIMING - (ulTimer1 - ulTimer2));
        }
      }
  
      ulTimer2 = ulTimer1;

      j = analogRead(A0);
  
      if(j > 1023)
        j = 1023;
  
      j *= 32;
      
      //rev 1.2
      if(ucWMAmp == 1)
      {
        if(j >= 16384)
          j = 32767;
        else
          j *= 2;
      }
      else if(ucWMAmp == 2)
      {
        if(j >= 8192)
          j = 32767;
        else
          j *= 4;
      }
      
      buff[i] = j & 0xFF;
      buff[i+1] = (j >> 8) & 0xFF;
        
      i +=2;

      k++;

      if(k >= uiSampleSize)
        break;
    }

    Server.sendContent_P((const char *)buff, i);

    if(l == 12000)
      break;
  }

  Server.client().stop();
}

void calPage() //calibration
{
  unsigned int j,k;
  
  Server.setContentLength(800);

  Server.send(200, "image/jpeg", "");

  for(k = 0; k < 800; k++)
  {
    //wdt_reset();
    yield();
    j = analogRead(A0);

    if(j > 1023)
      j = 1023;

    j /= 4;
    
    buff[k] = j;
  }
  Server.sendContent_P((const char *)buff, 800);
  Server.client().stop();
}

void rootPage() //root page: http://192.168.4.1/
{
  //rev 1.3
  if(Server.arg("c") != NULL)
    calPage();
  else if(Server.arg("aps") != NULL || Server.arg("sts") != NULL || Server.arg("s") != NULL || Server.arg("b") != NULL)
    configPage_AC();
  else
  {
    //secret link bug fixed-rev 1.2
    //appearance changed-rev 1.3
    PGM_P content = PSTR("<!DOCTYPE html><html><head><title>WiCard WiFiMicrophone-ESP8266</title><meta name='viewport' content='width=device-width'><meta http-equiv='Content-Type' content='text/html; charset=UTF-8'/> <link rel='icon' href='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAC6ElEQVRYhe3WQYiVVRQH8N9MQ0hURA2RjxIzCHMxi260krDIoEULI0qwMLBwUWaKECEi0cIWRZFJBEYoKBHVJoIWUTC0qOhAzUIhYqgpnIhpkHrIIMPQ4t7XfPO9941vXoGb+cMH7517z7n/e+7/nHtZxWXG0EodUkpXYgNuwtXF3MbvmIyIi/8rgZTSCO7DQ7gHmzDSMH0eZzCOT/BFRMwPRCClNIq9eEre7SA4h+M4GhEzfRFIKa3BAbxgMcX/FW0cwav1I1pCIKV0J05h4zKBvsMEfsVfxX4tbsEY7sZVDf5nsCMivu8ikFJ6Em9jTc3pIj7GCflMlxVZEen92ImHdetlDrsj4uS/BFJKz+P12sQFnMThiJiqLQLXlw9mMRsRdTLr8RIex3At/t6IeHOoqPxvS3f+M3ZGxHglWAvb8SDuwnW1gOfl4/kUH0TEuYrvFjmD62qZuGao7OaXyuA4tkXEbHHeUHaxXXP51TGP9+XsTZY4o/hILmWYwq2dIxjDvmI8EhFzKaVh7MfLunXRLy7gEN6IiIVSYS9iPV6LiImefaBMPCWLqI4FWc0T+KPYbpQrYJPus4YP8UREzNUHmlL6Xo/FZ/EW3q2LskJ8HXbhOUs18ohcTTvqPk0Z+NOiwsnnuaepm/XwH8UxPFrdQETcUJ/blIHTeFZO90G8EhGd8tsi1/hmi8Kdwldy5sYjYial9Bh+kDU0XGJ2oSkD5I52PiJ+LLZWWeCBBtIdfIZdnTJMKW2Uj+Prep9oJNCD0O34Eq1+5suX0L0d8suhl2J74ViPxWfwefnq2mgVn0uiXwLVy6WNp7E2IrZGxFasLbZ2ZV5fN2m/nW0f3pE73DMR8W11sDw6jqeUJuQLbaT4XBIrfpJ1kFK6GXvK36MR8dsgcQYiUK7cs/LbECZxx0rfg/SvgTrGKosrv8cGCTQogZ8sFVy72FaMKwZxmp6enmu1Wt/gNrkL7o6Is4PEWsUqLjv+AeQl8BLyi3sTAAAAAElFTkSuQmCC' type='image/x-png'/><style type='text/css'>body{background-color:#8F8;color:#777;font-family: sans-serif;}.inner{background-color:#BFF; min-height: 400px;box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);}.scope{text-align: center;margin: auto; padding: 5px; display:none;padding-top: 50px;}.h{display:none;}#set{text-align: center;margin: auto; padding: 5px;padding-top: 0; display:none;}#player{text-align: center;margin: auto; padding: 5px;padding-top: 50px;}.row{margin-left: auto;margin-right: auto;margin-bottom: 20px;}.row:after{content: '';display: table;clear: both;}@media only screen and (max-width: 792px){.container{max-width: 390px;width: 100%;margin: auto; overflow:auto; height:100%;}#loading{width:390px;}}@media only screen and (min-width: 793px){.container{width: 700px !important;margin: 0 auto;}#loading{width:700px;}}.del>a{color:red; text-decoration:none;}nav{color:#fff; background-color:#fff; width:100%; height:70px; line-height:70px;}nav a{color:#fff;}nav .nav-wrapper{position:relative; height:100%;}nav .brand-logo{position:absolute; color:#fff; display:inline-block; font-size:2.1rem; padding:0; white-space:nowrap}.side-nav{position:fixed; width:300px; left:0; top:0; margin:0; height:100%; height:calc(100% + 60px); height:-moz-calc(100%); padding-bottom:60px; background-color:#fff; z-index:999; overflow-y:auto; will-change:transform; -webkit-backface-visibility:hidden; backface-visibility:hidden;}.side-nav li{float:none; line-height:48px;}.side-nav a{color:rgba(0,0,0,0.87); display:block; font-size:14px; font-weight:500; height:48px; line-height:48px; padding:0 32px;}.side-nav a:hover{background-color:rgba(0,0,0,0.05);}#mbar{margin:12px 18px;float: right;}#mbar>div{width:35px;height:5px;background:#333;margin:6px 0;transition:0.4s;}#mbar.c .b1{-webkit-transform:rotate(-45deg) translate(-9px, 6px);transform:rotate(-45deg) translate(-9px, 6px);}#mbar.c .b2{opacity: 0;}#mbar.c .b3{-webkit-transform:rotate(45deg) translate(-8px, -8px);transform:rotate(45deg) translate(-8px, -8px);}.side-nav a{font-size:inherit;}#inner_side{left:-350px;}input[type=button],input[type=submit]{background: #25a8e1;color: #FFF;cursor: pointer;font-size: 15px;border: none;padding: 10px; width:100px; margin: 5px;}input[type=button]:focus,,input[type=submit]:focus{background: #1974BB;border:none;}h3{text-align: left;}h2{margin:auto; padding-top: 10px; padding-bottom: 10px;}.switch{position: relative;display: inline-block;width: 60px;height: 34px; padding-top:10px;}.switch input{opacity: 0;width: 0;height: 0;margin-top: 8px;}.sl{position: absolute;cursor: pointer;top: 0;left: 0;right: 0;bottom: 10px;border-radius: 34px;background-color: #ccc;-webkit-transition: .4s;transition: .4s;}.sl:before{position: absolute;content: '';height: 26px;width: 26px;left: 4px;bottom: 4px;border-radius: 50%;background-color: white;-webkit-transition: .4s;transition: .4s;}input:checked + .sl{background-color: #25a8e1;}input:checked + .sl:before{-webkit-transform: translateX(26px);-ms-transform: translateX(26px);transform: translateX(26px);}.sp{display: inline-block; height: 34px;}fieldset{background: #bfc;margin: auto;width: 98%;padding: 5px;}input[type='text']{width: 93%;margin: 5px 0 10px;padding: 10px;border: 1px solid #ccc;background: #FFF;}input[type='text']:focus{outline: 0;border: 1px solid #25a8e1;}input[type='submit']{width: 100%;margin: 5px 0 10px;padding: 10px;}.ap{border: 1px solid #ccc;padding: 5px;background: #9ff;margin: 0 0 10px;}.rssi{padding: 3px;background: #9dd;margin-left: 10px;}#loading{display:none;position:absolute; height:100%; background-color:#5556; z-index: 20;}#prgs{position: absolute;width: 100%;height: 5px;}</style></head><body><div class='container' id='con'><div id='loading'> <progress id='prgs'></progress> </div><nav><div class=nav-wrapper><a class='brand-logo center' href='https://store.wicard.net'><img style='padding:5px; height:50px; max-width:100%' src='data:image/png;base64,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'/></a><a href='javascript:;' onclick='menu();' id='mbar'><div class='b1'></div><div class='b2'></div><div class='b3'></div></a><ul id='inner_side' class='side-nav'><li><a href='#' onclick='player();'>Player</a></li><li><a href='#' onclick='scope();'>Scope</a></li><li><a href='#' onclick='set();'>Settings</a></li><li><a href='https://store.wicard.net'>WiCardTech</a></li></ul></div></nav><div class='inner'> <div id='set'><div class='row'> <section class='white_balance'><form method='get' action='' id='f2'> <fieldset><h2>Modem Configuration</h2> ST MAC: <span id='stmac'></span><br/> <span id='cj'></span><br/> IP: <span id='cji'></span><br/> <h3>Available APs</h3> <div id='av'></div>SSID:<br/><input id='sts' type='text' name='sts'/>PASSWORD:<br/><input type='text' name='stp'/><input type='submit' value='Save'/> </fieldset></form><form method='get' action='' id='f1'> <fieldset><h2>Device HotSpot Configuration</h2> AP MAC: <span id='apmac'></span><br/>SSID:<br/><input type='text' name='aps' id='aps'/>PASSWORD:<br/><input type='text' name='app' id='app'/>192.168.x.x/[SECURE-LINK]:<br/><input type='text' name='sli' id='sli'/> <br/><label class='switch'><input type='checkbox' name='hap' id='hap'/><span class='sl'></span></label> Hidden HotSpot <br/><label class='switch'><input type='checkbox' name='dap' id='dap'/><span class='sl'></span></label> Temporary HotSpot<input type='submit' value='Save'/> </fieldset></form><form method='get' action='' id='f3'> <fieldset> <h2>Microphone Configuration</h2> <br/> Audio Stream Bitrate <select size='1' name='b' id='bitrate'> <option value='0'>80  Kbps</option> <option value='1'>90  Kbps</option> <option value='2'>100 Kbps</option></select> <br/> <br/> Audio Stream Packet Length <select size='1' name='p' id='plen'> <option value='0'>5 Min</option> <option value='1'>10 Min</option> <option value='2'>30 Min</option> <option value='3'>60 Min</option></select> <br/> <br/> Audio Stream Software Amplitude <select size='1' name='a' id='amp'> <option value='0'>x1</option> <option value='1'>x2</option> <option value='2'>x4</option></select> <input type='submit' value='Save'/> </fieldset></form> </section> </div></div><div id='player'> <audio controls='controls' id='audioplayer'>Your browser does not support the audio tag.</audio> </div><div class='scope' id='scope'> <canvas id='C' width='1000' height='256' style='border:1px solid #d3d3d3;width:100%;background-color:white;'>Your browser does not support the HTML5 canvas tag. </canvas> </div></div></div><script>function id(e){return document.getElementById(e);}var menuanimate=0,menupos=0, blob, sc=0, init=0, intval,c=id('C'),ctx=c.getContext('2d');function player(){clearInterval(intval);id('set').style.display='none';id('scope').style.display='none';id('player').style.display='block'; id('mbar').click(); id('loading').style.display='block'; id('audioplayer').src='';id('audioplayer').removeAttribute('src'); sc=0; intval=setInterval(set_player,3000);}function scope(){clearInterval(intval);id('set').style.display='none';id('player').style.display='none';id('scope').style.display='block'; id('mbar').click(); id('loading').style.display='block'; id('audioplayer').src='';id('audioplayer').removeAttribute('src'); sc=0; intval=setInterval(set_scope,3000);}function set(){clearInterval(intval);id('scope').style.display='none';id('player').style.display='none';id('set').style.display='block'; id('mbar').click(); id('loading').style.display='block'; id('audioplayer').src='';id('audioplayer').removeAttribute('src'); sc=0; intval=setInterval(set_handler,3000);}function set_handler(){clearInterval(intval); var ur=window.location.pathname,i; if(ur !='/')ur +='/'; if(window.XMLHttpRequest) xhr=new XMLHttpRequest(); else{xhr=new ActiveXObject('Microsoft.XMLHTTP');}ur +='?s=1';xhr.open('GET', ur, true);xhr.onload=function (o){var r=xhr.response;var x=0, y=r.indexOf('*'),z;z=r.substr(x,y-x);x=y+1;if(z[0]=='1')id('hap').checked=true;else{id('hap').checked=false;}if(z[1]=='1')id('dap').checked=true;else{id('dap').checked=false;}id('bitrate').value=z[2];id('plen').value=z[3];id('amp').value=z[4];z=z.substr(5);id('cj').innerHTML='Connected To: '+z;y=r.indexOf('*', x);z=r.substr(x,y-x);x=y+1;id('cji').innerHTML=z;y=r.indexOf('*', x);z=r.substr(x,y-x);x=y+1;id('sts').value=z;y=r.indexOf('*', x);z=r.substr(x,y-x);x=y+1;id('aps').value=z;y=r.indexOf('*', x);z=r.substr(x,y-x);x=y+1;id('app').value=z;y=r.indexOf('*', x);z=r.substr(x,y-x);x=y+1;id('sli').value=z;y=r.indexOf('*', x);z=r.substr(x,y-x);x=y+1;id('stmac').innerHTML=z;y=r.indexOf('*', x);z=r.substr(x,y-x);x=y+1;id('apmac').innerHTML=z;z=r.substr(x);id('av').innerHTML=z; id('loading').style.display='none';};xhr.send();}function set_player(){clearInterval(intval); id('loading').style.display='none'; var u=window.location.pathname; if(u !='/')u +='/';u +='s'; id('audioplayer').src=u;} set_player(); function set_scope(){clearInterval(intval); id('loading').style.display='none';sc=1; intval=setInterval(loadFrame,200);}function loadFrame(){var i; clearInterval(intval); if(init==0){init=1;for(i=0; i < 94; i++){ctx.beginPath();ctx.moveTo(i, 128);ctx.lineTo(i+1, 128);ctx.stroke();}for(i=905; i < 999; i++){ctx.beginPath();ctx.moveTo(i, 128);ctx.lineTo(i+1, 128);ctx.stroke();}}if (window.XMLHttpRequest) xhr=new XMLHttpRequest(); else{xhr=new ActiveXObject('Microsoft.XMLHTTP');}xhr.open('GET', '?c=0', true); xhr.responseType='arraybuffer'; xhr.onreadystatechange=function(){if (xhr.readyState==4 && xhr.status==200){var a=new Uint8Array(xhr.response); if(a.length==800){ctx.clearRect(99,0,802,256);for(i=0; i < 799; i++){ctx.beginPath();ctx.moveTo(i+100, (255-a[i]));ctx.lineTo(i+101, (255-a[i+1]));ctx.stroke();}if(sc==1) intval=setInterval(loadFrame,200);}else{alert('error! please refresh...');}}}; xhr.send();}function hidemenu(){if(menuanimate==-1){menupos -=25;id('inner_side').style.left=menupos + 'px';if(id('inner_side').style.left=='-350px') menuanimate=0; else{setTimeout(hidemenu,20);}}}function showmenu(){if(menuanimate==1){menupos +=25;id('inner_side').style.left=menupos + 'px';if(id('inner_side').style.left=='0px') menuanimate=0; else{setTimeout(showmenu,20);}}}function menu(){if (id('inner_side').style.left=='0px'){menuanimate=-1; menupos=0; setTimeout(hidemenu,20); id('mbar').classList.remove('c');}else{menuanimate=1; menupos=-350; setTimeout(showmenu,20); id('mbar').classList.add('c');}}function ap(n){id('sts').value=n;}</script></body></html>");
    Server.send(200, "text/html", content);
  }
}
