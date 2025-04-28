#!/usr/bin/env python
# -*- coding: utf-8 -*-

import copy
import argparse
import time
import cv2 as cv
import numpy as np
import mediapipe as mp


def get_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("--device", type=int, default=0)
    parser.add_argument("--width", type=int, default=960)
    parser.add_argument("--height", type=int, default=540)

    parser.add_argument('--use_static_image_mode', action='store_true')
    parser.add_argument("--min_detection_confidence", type=float, default=0.7)
    parser.add_argument("--min_tracking_confidence", type=float, default=0.5)

    return parser.parse_args()


class CvFpsCalc:
    def __init__(self, buffer_len=10):
        self.start_tick = time.time()
        self.buffer_len = buffer_len
        self.tick_deque = []

    def get(self):
        current_tick = time.time()
        self.tick_deque.append(current_tick)
        if len(self.tick_deque) > self.buffer_len:
            self.tick_deque.pop(0)
        fps = 0
        if len(self.tick_deque) >= 2:
            fps = len(self.tick_deque) / (self.tick_deque[-1] - self.tick_deque[0])
        return int(fps)


def main():
    args = get_args()

    cap_device = args.device
    cap_width = args.width
    cap_height = args.height
    use_static_image_mode = args.use_static_image_mode
    min_detection_confidence = args.min_detection_confidence
    min_tracking_confidence = args.min_tracking_confidence

    cap = cv.VideoCapture(cap_device)
    cap.set(cv.CAP_PROP_FRAME_WIDTH, cap_width)
    cap.set(cv.CAP_PROP_FRAME_HEIGHT, cap_height)

    if not cap.isOpened():
        print("Error: Could not open camera.")
        exit()

    mp_hands = mp.solutions.hands
    hands = mp_hands.Hands(
        static_image_mode=use_static_image_mode,
        max_num_hands=2,  # Track two hands
        min_detection_confidence=min_detection_confidence,
        min_tracking_confidence=min_tracking_confidence,
    )

    mp_drawing = mp.solutions.drawing_utils
    mp_drawing_styles = mp.solutions.drawing_styles

    cvFpsCalc = CvFpsCalc(buffer_len=10)

    while True:
        fps = cvFpsCalc.get()

        key = cv.waitKey(10)
        if key == 27:  # ESC to exit
            break

        ret, image = cap.read()
        if not ret:
            print("Error: Can't receive frame (stream end?). Exiting ...")
            break

        image = cv.flip(image, 1)
        debug_image = copy.deepcopy(image)

        # Processing
        image = cv.cvtColor(image, cv.COLOR_BGR2RGB)
        image.flags.writeable = False
        results = hands.process(image)
        image.flags.writeable = True

        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    debug_image,
                    hand_landmarks,
                    mp_hands.HAND_CONNECTIONS,
                    mp_drawing_styles.get_default_hand_landmarks_style(),
                    mp_drawing_styles.get_default_hand_connections_style(),
                )

        # Show FPS
        cv.putText(debug_image, f"FPS: {fps}", (10, 30),
                   cv.FONT_HERSHEY_SIMPLEX, 1.0, (0, 255, 0), 2, cv.LINE_AA)

        cv.imshow('Hand Skeleton Tracking', debug_image)

    cap.release()
    cv.destroyAllWindows()


if __name__ == '__main__':
    main()
