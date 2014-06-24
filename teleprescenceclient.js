var bodyElem = $("body");
var keysPressed = {};
bodyElem.keydown(function(evt) {
  keysPressed.push(evt.which);
  if(keys.indexOf(evt.which) == -1)
    gapi.hangout.data.setValue("keys", keysPressed);
});
bodyElem.keyup(function(evt) {
  var index = keysPressed.indexOf(evt.which);
  if(index != -1) {
    keysPressed.splice(index, 1);
    gapi.hangout.data.setValue("keys", keysPressed);
  }
});
var lastData = gapi.hangout.data.getValue("keys");
while(true) {
  if(gapi.hangout.data.getValue("keys") != lastData) {
    alert(gapi.hangout.data.getValue("keys"));
    lastData = gapi.hangout.data.getValue("keys");
    alert(lastData);
  }
}
