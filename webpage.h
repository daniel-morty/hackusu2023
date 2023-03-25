const char indexHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Vehicle Control</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      background-color: #f0f0f0;
    }

    h1 {
      color: #333;
      font-size: 24px;
      margin-bottom: 10px;
    }

    .joystick-container {
      width: 200px;
      height: 200px;
      background-color: #ccc;
      border: 2px solid #333;
      border-radius: 50%;
      position: relative;
      overflow: hidden;
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    }

    .joystick {
      width: 50px;
      height: 50px;
      background-color: #333;
      border-radius: 50%;
      position: absolute;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%);
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.3);
    }
    button {
      background-color: #333;
      color: white;
      border: none;
      border-radius: 5px;
      padding: 10px 20px;
      font-size: 16px;
      cursor: pointer;
      margin-top: 20px;
      box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    }
	button:hover {
	  background-color: #444;
	}
  </style>
</head>
<body>
  <button id="autopilotButton">Toggle Autopilot</button>
  <h1>ESP32 Vehicle Control</h1>
  <div id="joystickContainer" class="joystick-container">
    <div id="joystick" class="joystick"></div>
  </div>
  <script>
    const joystick = document.getElementById("joystick");
    const container = document.getElementById("joystickContainer");
    const ws = new WebSocket("ws://" + location.hostname + "/ws");

    container.addEventListener("touchstart", handleTouchStart, false);
    container.addEventListener("touchmove", handleTouchMove, false);
    container.addEventListener("touchend", handleTouchEnd, false);

    function handleTouchStart(event) {
      event.preventDefault();
      moveJoystick(event.touches[0].clientX, event.touches[0].clientY);
    }

    function handleTouchMove(event) {
      event.preventDefault();
      moveJoystick(event.touches[0].clientX, event.touches[0].clientY);
    }

    function handleTouchEnd(event) {
      event.preventDefault();
      resetJoystick();
    }

    function moveJoystick(x, y) {
      const rect = container.getBoundingClientRect();
      const centerX = rect.x + rect.width / 2;
      const centerY = rect.y + rect.height / 2;

      const dx = x - centerX;
      const dy = y - centerY;

      const distance = Math.sqrt(dx * dx + dy * dy);
      const angle = Math.atan2(dy, dx);

      const radius = container.clientWidth / 2 - joystick.clientWidth / 2;

      const newX = Math.min(Math.max(dx, -radius), radius);
      const newY = Math.min(Math.max(dy, -radius), radius);

      joystick.style.transform = `translate(${newX}px, ${newY}px)`;

      sendJoystickData(Math.round(newX * 1000 / radius), Math.round(newY * 1000 / radius));
    }

    function resetJoystick() {
    joystick.style.transform = "translate(-50%, -50%)";
    sendJoystickData(0, 0);
    }

    function sendJoystickData(x, y) {
    let buffer = new ArrayBuffer(4);
    let view = new DataView(buffer);

    view.setInt16(0, x);
    view.setInt16(2, y);

    ws.send(buffer);
    }

	const autopilotButton = document.getElementById("autopilotButton");
	let autopilotEnabled = false;

	autopilotButton.addEventListener("click", function () {
	  autopilotEnabled = !autopilotEnabled;
	  ws.send("AUTOPILOT:" + (autopilotEnabled ? "1" : "0"));
	  autopilotButton.textContent = autopilotEnabled ? "Disable Autopilot" : "Enable Autopilot";
	});
  </script>
</body>
</html>
)rawliteral";
