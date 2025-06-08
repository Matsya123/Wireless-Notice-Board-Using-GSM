# AWS Lambda Function – TrainStatusAnnouncementHandler

This Lambda function is used in a smart railway station announcement system. It receives a **train number** via Amazon Lex, fetches the corresponding audio file ID from DynamoDB, and triggers an ESP32 over MQTT to play the announcement using a speaker module (e.g., DFPlayer Mini).

---

## 🔁 Trigger Source

- **Amazon Lex Bot**
  - Intent: `TrainStatusIntent`
  - Slot: `trainNumber`
  - Example Utterance: “What is the status of train 12627?”

---

## 🧠 What It Does

1. Extracts the `trainNumber` slot from the Lex intent.
2. Queries **DynamoDB** table `TrainRespose` to fetch `mp3Id` associated with that train number.
3. Publishes the `mp3Id` as MQTT payload to topic `esp32/flight/audio`.
4. ESP32 receives the ID and plays the corresponding audio file through speaker.
5. Returns a confirmation back to Lex.

---

## 📚 AWS Services Used

- **AWS Lambda**: Backend logic
- **Amazon Lex**: Natural language input
- **DynamoDB**: Stores train numbers and associated `mp3Id`s
- **AWS IoT Core**: Publishes audio commands to ESP32 over MQTT
- **(Optional)** AWS SNS: Can be used for fallback alerts (included but unused here)

---

## 📤 DynamoDB Schema

**Table Name**: `TrainRespose`

| Field     | Type   | Description                        |
|-----------|--------|------------------------------------|
| trainId   | String | Primary key (train number)         |
| mp3Id     | String | Audio file ID stored on SD card    |

**Example Item:**
```json
{
  "trainId": "12627",
  "mp3Id": "123"
}
