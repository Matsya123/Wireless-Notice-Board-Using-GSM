# DynamoDB Table – TrainResponse

This table maps train IDs to corresponding audio announcements and descriptive text. It is used in a smart announcement system where voice messages are triggered based on a user query (via Amazon Lex).

---

## 🗃️ Table Name
**TrainResponse**

---

## 🔑 Primary Key
- `trainId` (String) — the unique identifier for each train/message.

---

## 📄 Attributes

| Attribute   | Type   | Description                                      |
|-------------|--------|--------------------------------------------------|
| trainId     | String | Unique ID (can be train number or command ID)   |
| mp3Id       | String | ID of the audio file to be played (e.g., `001`) |
| response    | String | Optional text response or status message        |

---

## 📦 Sample Items

```json
{
  "trainId": "123",
  "mp3Id": "001",
  "response": "Let me check the status of train 789 from platform 2..."
}
