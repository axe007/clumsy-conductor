import cv2

def draw_bounding_boxes(img, contours, color=(0, 255, 0)):
    for contour in contours:
        x, y, w, h = cv2.boundingRect(contour)
        cv2.rectangle(img, (x, y), (x+w, y+h), color, 2)
    return img

