# 🤚 Hand Gesture Tracking with MediaPipe + DroidCam

Track hand skeletons in real-time using your computer webcam or smartphone camera with the power of **MediaPipe** and **OpenCV**.

---

## 📑 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Installation](#-installation)
- [Setup DroidCam (Optional)](#-setup-droidcam-optional)
- [Usage](#-usage)
- [Command Line Arguments](#-command-line-arguments)
- [Demo](#-demo)
- [Troubleshooting](#-troubleshooting)
- [Credits](#-credits)

---

## 🔥 Overview

This project detects and tracks hand gestures by identifying key landmarks on your hand in real-time.  
It works with a regular webcam or a smartphone camera using the **DroidCam** app.

Perfect for beginners learning **computer vision**, **gesture control**, and **MediaPipe**!

---

## ✨ Features

- Real-time hand landmark detection
- Works with built-in webcam or smartphone camera
- Frame rate (FPS) display
- Easy to extend for gesture-based applications
- Lightweight and efficient

---

## 🛠 Installation

1. **Install Python 3.7+**  
   Download: [https://www.python.org/downloads/](https://www.python.org/downloads/)

2. **Clone this Repository**

```bash
git clone https://github.com/Arisudan/Hand Gesture Using Python.git
cd Hand Gesture Using Python/Basic Hand Gesture using Python
```

Or simply download the project ZIP and extract it.

3. **Install Dependencies**

```bash
pip install opencv-python mediapipe numpy
```

---

## 📱 Setup DroidCam (Optional)

To use your **smartphone as a camera**:

1. Install the **DroidCam** app on your phone.
2. Install the **DroidCam Client** on your PC: [Download Here](https://www.dev47apps.com/)
3. Connect your phone and PC via **Wi-Fi** or **USB**.
4. Your phone will appear as a camera device (usually device `1`).

---

## 🚀 Usage

**Run with default webcam (device 0):**

```bash
python HandGesturebasic.py
```

**Run with DroidCam (device 1 or another number):**

```bash
python HandGesturebasic.py --device 1
```

**Customize camera resolution:**

```bash
python HandGesturebasic.py --device 1 --width 1280 --height 720
```

**Exit the Program:** Press the **ESC key**.

---

## ⚙️ Command Line Arguments

| Argument | Description | Default |
|:---|:---|:---|
| `--device` | Camera device index (0 = default webcam, 1 = DroidCam) | 0 |
| `--width` | Width of video frame | 960 |
| `--height` | Height of video frame | 540 |
| `--use_static_image_mode` | Better detection for still images (slower) | False |
| `--min_detection_confidence` | Minimum confidence for hand detection | 0.7 |
| `--min_tracking_confidence` | Minimum confidence for hand tracking | 0.5 |

---

## 🎥 Demo

*(Insert demo GIF or image here)*

Example:

```markdown
![Hand Tracking Demo](link_to_your_demo_image.gif)
```

---

## Troubleshooting

| Problem | Solution |
|:---|:---|
| Camera not opening | Make sure the correct `--device` index is selected. Ensure no other apps are using the camera. |
| No hand detected | Good lighting is essential. Show your hand clearly within the frame. |
| Low FPS | Reduce frame size (`--width 640 --height 480`) or close heavy applications. |
| DroidCam not working | Ensure both mobile and PC DroidCam apps are connected properly. |

---

## Credits

- [MediaPipe](https://mediapipe.dev/) - Hand tracking solution
- [OpenCV](https://opencv.org/) - Video processing library
- [DroidCam](https://www.dev47apps.com/) - Mobile-to-PC webcam solution

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

### Bonus tip:
- If you add a short GIF or image showing your hand detection (even a 3-second clip), your GitHub project will look **10x better**.
- You can record your screen using free tools like **OBS Studio** or simply capture a screenshot.

---
