#include "Structure.h"

void commandReceived(char* topic, byte* payload, unsigned int length) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& command = jsonBuffer.parseObject((char*) payload);
  command.printTo(Serial);
  // StructureDevice::commandCallback(command);
}

StructureDevice::StructureDevice(char* id) {
  this->id = id;
  stateTopic = String(STRUCTURE_TOPIC_PREFIX);
  stateTopic.concat(id);
  stateTopic.concat(STRUCTURE_TOPIC_STATE);
  commandTopic = String(STRUCTURE_TOPIC_PREFIX);
  commandTopic.concat(id);
  commandTopic.concat(STRUCTURE_TOPIC_COMMAND);
}

char* StructureDevice::getId() {
  return id;
}

void StructureDevice::onCommand(CommandCallback callback) {
  // commandCallback = callback;
}

void StructureDevice::connect(Client& client, const char* key, const char* secret) {
  mqttClient.setClient(client);
  mqttClient.setServer(STRUCTURE_BROKER, STRUCTURE_PORT);
  mqttClient.connect(id, key, secret);
  mqttClient.setCallback(commandReceived);
  int topicLen = commandTopic.length() + 1;
  char topicBuf[topicLen];
  commandTopic.toCharArray(topicBuf, topicLen);
  mqttClient.subscribe(topicBuf);
}

void StructureDevice::disconnect() {
  mqttClient.disconnect();
}

boolean StructureDevice::connected() {
  return mqttClient.connected();
}

boolean StructureDevice::loop() {
  return mqttClient.loop();
}

void StructureDevice::sendState(JsonObject& state) {
  int topicLen = stateTopic.length() + 1;
  char topicBuf[topicLen];
  stateTopic.toCharArray(topicBuf, topicLen);
  int stateLen = state.measureLength() + 1;
  char stateBuf[stateLen];
  state.printTo(stateBuf, stateLen);
  state.printTo(Serial);
  Serial.println(topicBuf);
  mqttClient.publish(topicBuf, stateBuf);
}