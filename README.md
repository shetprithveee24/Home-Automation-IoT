# Home Automation System

A smart home automation system developed using **Arduino** and **Flutter**, designed to enhance lighting control and improve user convenience.

## Features
- Automatic light activation and deactivation based on room entry and exit.
- Mobile application control built with **Flutter**.
- Integration with **Arduino** for seamless hardware-software interaction.

## Technologies Used
- **Flutter**: This is for developing the mobile application interface.
- **Arduino**: For hardware implementation and communication.
- **Wi-Fi (ESP8266/ESP32)**: For wireless communication between devices.

## Getting Started

### Prerequisites
- **Arduino IDE** installed on your computer.
- **Flutter SDK** set up for mobile app development.
- ESP8266/ESP32 Wi-Fi module.

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/banu4prasad/home-automation.git
   ```

2. Open the **Arduino** folder and upload the code to your board using Arduino IDE.

3. Navigate to the **Flutter** folder, install dependencies, and run the app:
   ```bash
   cd FlutterApp
   flutter pub get
   flutter run
   ```

## Usage
- Connect the hardware to Wi-Fi using the provided configuration settings.
- Use the mobile app to control lights, customize colors, and monitor system status.

## Project Structure
- **/Arduino**: The Arduino source code for hardware control.
- **/FlutterApp**: Contains the Flutter project for the mobile application.

## Working

#### Sensor working
```mermaid
sequenceDiagram
    participant IR as IR Sensor
    participant ESP as ESP8266
    participant SERVO as Servo Motor
    participant PIR as PIR Sensor
    participant LED as LED Lights

    Note over IR,SERVO: IR Sensor Door Control
    IR->>ESP: Motion Detected
    ESP->>SERVO: Open Door (160°)
    SERVO-->>ESP: Door Opened
    ESP->>IR: Wait for Return
    IR->>ESP: Motion Detected Again
    ESP->>SERVO: Close Door (0°)
    SERVO-->>ESP: Door Closed

    Note over PIR,LED: PIR Sensor LED Control
    PIR->>ESP: Motion Detected
    ESP->>LED: Turn On White LEDs
    LED-->>ESP: LEDs On
    Note over PIR,LED: Wait 5 seconds
    PIR->>ESP: No Motion
    ESP->>LED: Turn Off White LEDs
    LED-->>ESP: LEDs Off
```

#### Sensor Data Flow
```mermaid
flowchart LR
    classDef hardware fill:#FF9999,stroke:#FF0000,color:#000000
    classDef sensor fill:#99FF99,stroke:#00FF00,color:#000000
    classDef motor fill:#9999FF,stroke:#0000FF,color:#000000
    classDef wifi fill:#FFFF99,stroke:#FFD700,color:#000000
    classDef cloud fill:#99FF99,stroke:#00FF00,color:#000000
    classDef storage fill:#9999FF,stroke:#0000FF,color:#000000
    classDef process fill:#FFFF99,stroke:#FFFF00,color:#000000

    subgraph Hardware["Hardware Layer"]
        ESP[ESP8266 WiFi Module]:::wifi
        IR[IR Sensor]:::sensor
        PIR[PIR Sensor]:::sensor
        SERVO[Servo Motor]:::motor
    end

    subgraph Cloud["Cloud Services"]
        API[Google Apps Script API]:::cloud
        WebApp[Web App Deployment]:::cloud
    end

    subgraph Storage["Data Storage & Analysis"]
        Sheets[Google Sheets]:::storage
        Graphs[Real-time Charts]:::process
    end

    IR -->|"Motion Detection"| ESP
    PIR -->|"Presence Detection"| ESP
    ESP -->|"Control Signal"| SERVO
    ESP -->|"Sensor Data"| API
    API --> WebApp
    WebApp --> Sheets
    Sheets --> Graphs

    %% Legend
    subgraph Legend["Component Types"]
        H[Hardware Components]:::hardware
        W[Wifi Module]:::wifi
        C[Cloud Services]:::cloud
        S[(Storage Systems)]:::storage
        P[Processing Elements]:::process
    end
```


## Future Enhancements
- Add voice control capabilities.
- Extend automation features to other appliances.
- Implement machine learning for predictive lighting control based on user behavior.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
- Inspiration from IoT smart home technologies.
- Support from the developer community.
