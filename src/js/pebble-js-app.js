var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude + "&APPID=484bb4779c7a97c89c3f582dd2ac6292";

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp - 273.15);
      console.log("Temperature is " + temperature);

      // Conditions
      var conditions = json.weather[0].icon;      
      console.log("Conditions are " + conditions);
      
      // Conditions
      var condition_id = json.weather[0].id;      
      console.log("Condition id is " + condition_id);
      
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_TEMPERATURE": temperature,
        "KEY_CONDITIONS": conditions,
        "KEY_ID": condition_id
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );
    }      
  );
  
  //Get forecast
  url = "http://api.openweathermap.org/data/2.5/forecast?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude + "&APPID=484bb4779c7a97c89c3f582dd2ac6292";;
  
    xhrRequest(url, 'GET', 
    function(responseText) {
      var json = JSON.parse(responseText);
      var str1 = JSON.stringify(json.list[1].rain, null, 2).replace("3h","threeh");
      var json1 = JSON.parse(str1);
      var str2 = JSON.stringify(json.list[2].rain, null, 2).replace("3h","threeh");
      var json2 = JSON.parse(str2);
      var str3 = JSON.stringify(json.list[3].rain, null, 2).replace("3h","threeh");
      var json3 = JSON.parse(str3);
      
      var precip = Number(json1.threeh); //rain for next 3h
      var precip2 = Number(json2.threeh); //rain for next 3-6h
      var precip3 = Number(json3.threeh); //rain for next 6-9h
      console.log("Precipitation for 9h " + precip);
      
      var preturn = 0;
      if (!isNaN(precip)){
        preturn = parseInt(precip*100);
      }
      if (!isNaN(precip2)){
        preturn = preturn + parseInt(precip2*100);
      }
      if (!isNaN(precip3)){
        preturn = preturn + parseInt(precip3*100);
      }
      
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_PRECIP": preturn,
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Forecast info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending forecast info to Pebble!");
        }
      );
    }      
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getWeather();
  }                     
);
