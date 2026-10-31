
# 📽️ ESP8266 Epson Projector Web Remote

Control your Epson projector from your phone using an ESP8266 and an IR LED. No physical remote or app required!

## 🛒 What You Need
*   **ESP8266 Board** (NodeMCU or Wemos D1 Mini)
*   **IR LED** (940nm)
*   **Resistor** (100Ω or 220Ω)
*   Jumper wires

## 🔌 Wiring
1.  **Resistor:** Connect one end to the **D2** pin on the ESP8266.
2.  **IR LED:** Connect the **long leg** to the other end of the resistor.
3.  **Ground:** Connect the **short leg** of the IR LED directly to **GND**.

## 💻 Software Setup
1.  Install the **Arduino IDE**.
2.  Go to **Sketch > Include Library > Manage Libraries**.
3.  Search for **`IRremoteESP8266`** (by Cranky Old Git) and click **Install**.

## 🚀 How to Upload
1.  Open the `.ino` file provided in this repository.
2.  Plug in your ESP8266.
3.  Select Board: **Tools > Board > NodeMCU 1.0**.
4.  Select your Port: **Tools > Port > COMx**.
5.  Click **Upload**.

## 📱 How to Use
1.  On your phone, connect to the Wi-Fi network **`Epson_Remote`** (Password: `12345678`).
    *   *(Note: Your phone will say "No Internet." Ignore it and stay connected).*
2.  Open your phone's web browser and go to: **`192.168.4.1`**
3.  Point the IR LED at the projector and tap the buttons on your screen!

## 💡 Features
*   **Auto Power-Off:** The POWER button automatically sends the signal twice with a 1-second pause to confirm the projector's "Power Off?" prompt.
*   **Custom Hex Box:** Need a button that isn't there? Type the hex code in the Custom box at the bottom of the web page to test it.
