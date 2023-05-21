import cv2
import numpy as np

def get_color_mask(img):
    yellow_lower_range = np.array([17, 138, 26])
    yellow_upper_range = np.array([75, 242, 255])

    blue_lower_range = np.array([43, 138, 26])
    blue_upper_range = np.array([141, 242, 255])

    height, width, _ = img.shape
    img[0:int(height/2), :] = 0
    img[int(height/1.28):, :] = 0

    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    def get_range(lower, upper):
        tmp = cv2.inRange(img_hsv, lower, upper)
        ret = cv2.GaussianBlur(tmp, (11, 11), 0)
        return ret

    yellow_mask = get_range(yellow_lower_range, yellow_upper_range)
    blue_mask = get_range(blue_lower_range, blue_upper_range)

    return cone_contours(blue_mask), cone_contours(yellow_mask)


def cone_contours(mask):
    threshold = 100

    canny_output = cv2.Canny(mask, threshold, threshold * 2)

    contours, _ = cv2.findContours(canny_output, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    boundRect = [None]*len(contours)

    for i, contour in enumerate(contours):
        poly = cv2.approxPolyDP(contour, 3, True)
        boundRect[i] = cv2.boundingRect(poly)

    return boundRect
