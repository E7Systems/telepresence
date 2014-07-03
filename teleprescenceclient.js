var secured = false;
var cryptokey = [];
function init() {
  var bodyElem = $("body");
  var keysPressed = [];
  while(!gapi.hangout.isApiReady()) {};
  generateSecureKey();
  gapi.hangout.data.setValue("keys", JSON.stringify(keysPressed));
  bodyElem.keydown(function(evt) {
    if(keysPressed.indexOf(evt.which) == -1) {
      keysPressed.push(evt.which);
      gapi.hangout.data.setValue("keys", JSON.stringify(keysPressed));
    }
  });
  bodyElem.keyup(function(evt) {
    var index = keysPressed.indexOf(evt.which);
    if(index != -1) {
      keysPressed.splice(index, 1);
      gapi.hangout.data.setValue("keys", JSON.stringify(keysPressed));
    }
  });
  var lastData = gapi.hangout.data.getValue("keys");
  setInterval(function() {
    if(gapi.hangout.data.getValue("keys") != lastData) {
      lastData = gapi.hangout.data.getValue("keys");
      console.log("New Keys: " + lastData);
      //$.get("localhost:8080")
    }
  }, 50, 50);
}
gapi.hangout.onApiReady.add(function(eventObj) {
  init();
});

function up() {
  keysPressed.push(38);
}
function down() {
  keysPressed.push(40);
}
function left() {
  keysPressed.push(37);
}
function right() {
  keysPressed.push(39);
}
function setData(key, value) {
	gapi.hangout.setValue(key, value);
}
function setSecuredData(key, data, cipher) {
	setData(key, CryptoJS.AES.encrypt(data, cipher.join()));
}
function appendSecuredData(key, data, cipher) {
	setData(key, CryptoJS.AES.encrypt(getSecuredData(key) + data, cipher.join()));
}
function getData(key) {
	return gapi.hangout.getValue(key);
}
function getSecuredData(key, cipher) {
	return CryptoJS.AES.decrypt(getData(key), cipher);
}
function generateSecureKey() {
	gapi.hangout.layout.displayNotice("Generating a secure key for your channel, please move your mouse at random for a few seconds.", true);
	var keybits = new Array();
	var lastX = 0;
	var lastY = 0;
	$("body").mousemove(function(evt) {
		if(Math.round(Math.random()*100) <= 30) {
			var multVal = Math.abs(lastX - evt.pageX) * Math.abs(lastY - evt.pageY);
			keybits.push(multVal);
			console.log(multVal);
		}
		if(keybits.length >= 40) {
			$("body").unbind("mousemove");
			gapi.hangout.layout.dismissNotice();
			gapi.hangout.layout.displayNotice("Generation finished.", false);	
			console.log(keybits.join(","));
		}
	});
	cryptokey = keybits
	sendCryptoKey(cryptokey.join());
	return keybits;
}
function sendCryptoKey(key) {
	if(getData("cryptkey").strip() === "") {
		setData("cryptkey", key);
	} else {
		getCryptoKey("cryptkey");
	}
	console.log(getCryptoKey("cryptkey"));
}
function getCryptoKey() {
	return getData("cryptkey");
}
function appendData(key, value) {
	gapi.hangout.setValue(key, gapi.hangout.getValue(key)+value);
}