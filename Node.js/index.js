// preparing module Crypto to get sha256 value from signatures
const crypto = require("crypto");
const hash = crypto.createHash("sha256");

// preparing server using express
var express = require("express")
  , http = require('http')
  , app = express()
  , server = http.createServer(app)
  , bodyParser = require('body-parser')

// setting values for port and ip address for server
const port = 9100;
const ip_address = process.argv[2];

// setting server to read json from request
app.use(bodyParser.json());

// running server to listen on port and ip address
server.listen(port, ip_address, function(){
  console.log("node.js server is listening");
});

// setting route and action for post request
app.post('/', function(request, response){

  console.log("post request received");

  // getting json array with key = data
  var array = request.body["data"];
  var number_of_items = 0;
  var signature = "";

  // concatenating all values from key = signature
  // counting number of all items in json array
  for(var i in array) {
    var s = array[i];
    signature += s["signature"];
    number_of_items++;
  }

  // calculating sha256 signature for concatenade string
  signature = hash.update(signature).digest("hex");

  // preparing json response 
  var jsonResponse = 
  { 
    "metric" : {
      "signature" : signature,
      "number-of-items" : number_of_items.toString()
    } 
  };

  // sending response from server to client
  response.send(jsonResponse);
})