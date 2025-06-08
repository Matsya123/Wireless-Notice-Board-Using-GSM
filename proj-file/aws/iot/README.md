# AWS IoT Rule – Train Announcement Trigger

This rule processes incoming MQTT messages from the ESP32 device and triggers actions based on the train ID and associated audio command.

---

## 🎯 Rule Purpose

To monitor the MQTT topic `'esp32/flight/audio'` for valid train ID messages and pass relevant data (e.g., `trainId`, `mp3Id`) to other AWS services (e.g., Lambda, SNS, DynamoDB).

---

## 📄 SQL Rule Statement

```sql
SELECT trainId, mp3Id
FROM 'esp32/flight/audio'
WHERE trainId IS NOT NULL
