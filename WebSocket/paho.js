// Create a client instance
client = new Paho.MQTT.Client("192.168.137.1", Number(9001), "ClienteWeb-123456789");

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({onSuccess:onConnect});


// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client.subscribe("outTopic");
  message = new Paho.MQTT.Message(0);
  message.destinationName = "inTopic";
  client.send(message);
}

function markerPlacement(){
  
if (message) {
  var marker = L.marker([19.532585, -96.911195]).addTo(mymap);
        marker.bindPopup(message).openPopup();
}else{
  marker.bindPopup("No se pudo recuperar la temperatura").openPopup();
}
  

}


// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
  console.log("onMessageArrived:"+message.payloadString);
var js= JSON.parse(message.payloadString);
console.log(js)
  if (message) {
    var marker = L.marker([js.lat,js.lon]).addTo(mymap);
          marker.bindPopup("<b>Temperatura:</b>"+js.temp).openPopup();
  }else{
    marker.bindPopup().openPopup();
  }


}