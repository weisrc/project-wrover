# Wireless Rover

Please see one of the following repositories for the latest codebase:
- https://github.com/weisrc/project-wrover
- https://github.com/schoolwei/project-wrover

This project is setup in a monorepo:
- `./esp` contains the code for the ESP32
- `./avr` contains the code for the ATmega8515
- `./web` contains the code for the web application
- `./pcb` contains the files for creating the PCB

## System Overview Diagram

Only the Web Application is not part of the Rover itself. All of the components are powered by a 5V power supply, except for the motors which are powered by a 9V power supply.

```mermaid
flowchart TD
    web[/Web Application/]

    cam[Camera]
    hall0[Left Wheel\n Hall Effect\n Sensor]
    hall1[Right Wheel\n Hall Effect\n Sensor]
    sonar0[Front Ultrasonic\n Sensor]
    sonar1[Left Ultrasonic\n Sensor]
    sonar2[Right Ultrasonic\n Sensor]
    amp[Amplifier]
    speaker[Speaker]
    
    lcd[LCD]

    esp[ESP32]
    avr[ATmega8515]

    subgraph 9V
    hbridge0[Left Wheel\n H-Bridge]
    hbridge1[Right Wheel\n H-Bridge]
    motor0[Left Wheel\n Motor]
    motor1[Right Wheel\n Motor]
    end

    web <-->|WebSocket or Serial| esp
    cam -->|OV2640| esp
    hall0 -->|IO| esp
    hall1 -->|IO| esp

    sonar0 <-->|IOs| avr
    sonar1 <-->|IOs| avr
    sonar2 <-->|IOs| avr
    
    avr -->|PWM| hbridge0
    avr -->|PWM| hbridge1
    hbridge0 -->|PWM| motor0
    hbridge1 -->|PWM| motor1

    avr -->|Waveform| amp
    amp -->|Waveform| speaker
    avr ==>|IOs| lcd
    
    esp <-->|ACK Serial| avr
```

## Web Application

Built using Next.js with TypeScript and Shadcn UI Component Library (stored in `web/components/ui`, not in `node_modules`), the web application is the user interface for the rover. It allows the user to:

- Control the rover
    - Calculate the relative position of the rover from the starting point using the wheel hall effect sensors
    - WASD controls
- View the camera feed
    - Change the camera resolution
    - Change the camera FPS
- Visualize the distances from the ultrasonic sensors
    - Draw the sample points
- Initial WiFi Setup
    - Send the WiFi credentials to the ESP32 via Serial
- Communciate with the rover via WebSocket and Serial

This will be hosted on a seperate server (Raspberry Pi) as the ESP32 will not be connected to the network at first.

### Initial WiFi Setup

To connect the ESP32 to the network, the user will need to connect the ESP32 with a client PC that has access to the hosted website using the Serial/USB cable, usually provided with the purchase of a development ESP32 board.

Right now, only Microsoft Edge, Google Chrome and Opera support the Web Serial API, which is required to connect to the ESP32 using the Serial/USB cable. Please refer to Can I Use (1) for more information.

1. https://caniuse.com/web-serial

### Calculating the Relative Position

Please see `web/lib/dual-odometer.ts` for the implementation of the Dual Odometer.

The calculation is also done on the Web Appllication as it allows the user to easily calibrate and fine-tune the odometer's paramters as all the data is stored in memory. Therefore, by adjusting the parameters, the user will see the sample points change on the canvas. The ESP32 will also calculate the current relative position, but it doesn't store the hall data history.

## ESP32

This module is implemented in C++ using the Arduino framework and the PlatformIO toolchain.

The ESP32 is the bridge between the Web Application and the ATmega8515. It's functionalities are:
- Stream the camera feed
- Calculate the relative position of the rover from the starting point using the wheel hall effect sensors
- Broadcasting the locomotion data: hall effect sensors and ultrasonic sensors
- Communicate with the ATmega8515 via Acknowledged Serial
- Communicate with the Web Application via WebSocket
- Smooth motor control

```mermaid
flowchart TD
    async[Async Serial]
    main[Main Program]
    avr[AVR Serial]
    serial[Web Serial]
    ws[Web Server]
    cam[Camera]
    locomotion[Locomotion]
    smooth[Smooth Motor Control]
    promise[Promise]
    queue[Message Queue]

    serial -->|sent to| main
    ws -->|adds| queue
    queue -->|handled by| main

    main -->|uses| avr
    main -->|update in loop| cam
    main -->|update in loop| locomotion
    locomotion -->|calls| smooth
    smooth -->|uses| avr
    cam -->|sends frame to| ws
    avr -->|uses| async
    main -->|update in loop| async
    async -->|resolves or creates| promise
    promise -->|may use recursively| async
    promise -->|may use| ws

```

### Acknowledged Serial

This is implemented using `SoftwareSerial` (1). Class `AsyncSerial` is an async (non-blocking, can't handle concurrency) utility for `SoftwareSerial`. This implementation relies on the `Promise` class which is inspired by JavaScript Promises (2). 

Async is required as waiting for the acknowledgement from the ATmega8515 is blocking otherwise, therefore, the Camera will not be able to stream smoothly. This will not be needed if a state machine is implemented. However, Promises in C++ is more interesting to implement. More or less, the state is captured in the Then closure. The Promise class itself doesn't memory manage itself. Therefore, it is wrapped in a `std::shared_ptr`.

Result class is inspired from Rust's Result (3). It is used to handle errors in the Promise instead of having to use exceptions.

There is parity checking only. If there is an error, ignore the frame.

1. https://github.com/plerup/espsoftwareserial
2. https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise.
3. https://doc.rust-lang.org/std/result/

#### Transmitting and Receiving Data
```mermaid
sequenceDiagram
    autonumber
    participant esp as ESP32
    participant avr as ATmega8515

    loop Until acknowledged
        esp->>avr: Data
        avr->>esp: Acknowledgement
    end
```

### Smooth Motor Control

To prevent drawing too much current from the ESP32, the rate at which the duty cycle is changed is limited. This is also done on the AVR side for extra safety.

The smooth motor value is sent to the ATmega8515 via the Acknowledged Serial. This value is also used to know the direction of the motor.
- If `l` is sent, the left motor hall effect sensor triggered and the motor is moving forward
- If `r`, then right with forward
- If `L`, then left with reverse
- If `R`, then right with reverse

The above values are stored in a string and sent to the Web Application via WebSocket in the `locomotion.hall` string. For more information, please see the ESP32 and Web Communication section.

### Hall Effect Sensors

The hall effect sensor readings are implemented using logical change interrupts. Debouncing is implemented to reduce the number of false positives. A series of alternating magnets are placed on the wheel to trigger the hall effect sensor.

### ESP32 and Web Communication

Communication is either done via WebSocket or Serial. This is done using the `Channel` virtual base class extended by `WSChannel` or `SerialChannel`.

Upon receiving a message form the WebApplication via WebSocket. The buffer is added to the singleton `MessageQueue` instance. The `MessageQueue` instance is then processed in the main loop. This is to prevent blocking the WebSocket thread at unexpected times. E.g. when the camera is streaming or when the `SoftwareSerial` is writing (messes up the timing).

Please see `RequestEvents` and `ResponseEvents` Type Records in `web/lib/types.ts` for more information.

- Requests
    - Connect is used to connect to a network. This is only used in Serial communication mode during the initial setup.
    - Scan is used to scan for available networks.
    - Disconnect is used to disconnect from the network.
    - SSID is used to get the SSID of the connected network.
    - IP: is used to get the IP address of the ESP32.
    - RSSI: is used to get the signal strength of the connected network.
    - Status: is used to get the WiFi status of the ESP32.
    - Begin: is used to start the WebSocket Server.
    - Set Camera FPS: is used to set the camera FPS.
    - Motor is used to control the motors.
    - Locomotion is used to get the locomotion data.
    - Capture is used to capture a frame from the camera.
- Responses
    - Scan returns the available networks.
    - IP returns the IP address of the ESP32.
    - RSSI returns the signal strength of the connected network.
    - Status returns the WiFi status of the ESP32.
    - Locomotion returns the locomotion data.
    - Socket Ready is used to indicate that the WebSocket connection is ready.
    - Binary Data (not JSON) is used to send the camera feed.

Status messages are also sent unsolicitedly to the Web Application when the WiFi status changes.

