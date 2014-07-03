function init() {
  var bodyElem = $("body");
  var keysPressed = [];
  while(!gapi.hangout.isApiReady()) {};
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
function generateSecureKey() {
	gapi.hangout.layout.displayNotice("Generating a secure key for your channel, please move your mouse at random for a few seconds.", false);
	var seeds = new Array();
	int lastX = 0;
	int lastY = 0;
	$("body").append("<div id='randGen'></div>");
	$("#randGen").mousemove(function(evt) {
		if(Math.round(Math.random()*100) <= 30)
			seeds.push(Math.abs(lastX - evt.X) * Math.abs(lastY - evt.Y));
		if(seeds.length >= 5) {
			$("#randGen").unbind();
			gapi.hangout.layout.dismissNotice();
			gapi.hangout.layout.displayNotice("Generation finished.", true);	
			console.log(seeds.join(","));
		}
	});
}
function appendData(key, value) {
	gapi.hangout.setValue(key, gapi.hangout.getValue(key)+value);
}