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
    }
  }, 50, 50);
}
gapi.hangout.onApiReady.add(function(eventObj) {
  init();
});
