
#define JSON_RX_SIZE 300

void parseJson() {
  if (Serial.available() > 0) {
    //Serial.println("Serial Available");
    char command[JSON_RX_SIZE];
    // create JSON document
    StaticJsonDocument<JSON_RX_SIZE> jsonRXdoc;
    // Receive Command
    Serial.readBytesUntil(10, command, JSON_RX_SIZE);  // 10 = "\n"

    for (int i = 0; i < JSON_RX_SIZE; i++) {  // trim command down to remove redundent characters
      if (command[i] == '}') {
        command[i + 1] = '\0';
        break;
      }
    }


    Serial.print(F("\n{\"rxed\": \""));
    Serial.print(command);
    Serial.println(F("\"}"));


    //Serial.read();  // clear any additional data left in the buffer

    // NEW FUNCTION, try deserializing directly from stream
    // deserializeJson(jsonRXdoc, Serial);  // less overhead but harder to debug
    [[maybe_unused]] DeserializationError error = deserializeJson(jsonRXdoc, command);  // more overhead but can print message before processing (good for debugging)


    Serial.print("{\"deserialization\":\"");
    Serial.print(error.c_str());
    Serial.println(F("\"}"));


    JsonObject root = jsonRXdoc.as<JsonObject>();  // this was previously doc.to<JsonObject>(); DID NOT WORK! does now with "as"
                                                   // Now to parse the JSON message
                                                   // First get number of elements in the jsonStates enum numValues = NUM_VALUES;
                                                   // Then loop through all keys & see if root contains key

    bool set_keyword_used = false;  // Make the assumption that set keyword has not been used,
                                    // NOTE THIS LINE CHANGED 27/11/25 to try and fix compiler warning about unititialised var
    //  const char *keyString;  // If the root contains "set" -> preload value (which contains key) into valueString and compare this along with root.key in loop below (2 birds, 1 stone)
    const char *keyString = { "\n" };
    if (root["payload"]["air"].containsKey("speed")) {               //|| strcmp(keyString, jsonStateMap[i].cmd) == 0) {  // Match is found, i holds the correct ENUM reference for the state
      float airSpeed = root["payload"]["air"]["speed"].as<float>();  // this should contain the correct enum for the datatype
      Serial.print("airSpeed: ");
      Serial.println(airSpeed);
    }
  }
}