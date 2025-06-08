# Wireless Notice Board with Speaker using GSM, Arduino & AWS ☁️📢

This project combines embedded hardware and cloud software to build a **remote-controllable digital notice board** that receives messages via SMS or WhatsApp, displays them on an LCD, and announces them using voice output through a speaker.

---

## 🛠 Hardware Used
- ESP32 or Arduino UNO
- SIM800L GSM Module
- 16x2 LCD Display
- DFPlayer Mini (Audio Module)
- Speaker (3W/5W)
- Power Supply (5V Regulated)
- Antenna, Breadboard, Jumper Wires

---

## ☁️ Cloud Services Used
- **AWS Lex**: Voice/Text chatbot to capture user commands
- **AWS Lambda**: Backend logic to handle input and trigger actions
- **Amazon DynamoDB**: Stores message history and status
- **Amazon SNS (via Twilio)**: Sends/receives WhatsApp messages
- **AWS IoT Core**: MQTT-based device communication (optional extension)

---

## 🔁 System Workflow
1. User sends a message via SMS or WhatsApp.
2. The GSM module receives the SMS → ESP32/Arduino displays it on LCD.
3. DFPlayer Mini plays the message using a pre-recorded audio file.
4. WhatsApp-based users can also send dynamic text to be converted into announcements.
5. All interactions can be logged in DynamoDB.

---

## 🧪 Example Commands
- SMS: `"Notice: Test at 10AM in Lab-1"`
- WhatsApp: `"Display meeting at 3PM"`
- Lex Voice: `"Send notice to display"` → `"Show welcome message"`


---

## 📂 Project Structure
| Folder         | Purpose                                    |
|----------------|--------------------------------------------|
| `arduino-code/` | Embedded firmware (.ino)                  |
| `aws/`          | Lex, Lambda, SNS, IoT rules               |
| `media/`        | Diagrams, prototype photos                |
| `docs/`         | Final report (PDF)                        |

---

## 🧠 Future Scope
- Multilingual support
- Text-to-speech conversion using AWS Polly
- Message scheduling via DynamoDB TTL
- Enhanced web dashboard for control

---


## 📜 License
MIT
