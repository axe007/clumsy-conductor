import math

def steering_angle(contours_blue, contours_yellow):
    if not contours_blue and not contours_yellow:
        return 0.0

    x_blue, y_blue = get_min_point(contours_blue)
    x_yellow, y_yellow = get_min_point(contours_yellow)

    # Determine which contours are on the left and right based on x values
    if x_blue is not None and x_yellow is not None:
        if x_blue < x_yellow:
            x_left, y_left = x_blue, y_blue
            x_right, y_right = x_yellow, y_yellow
        else:
            x_left, y_left = x_yellow, y_yellow
            x_right, y_right = x_blue, y_blue
    elif x_blue is not None:  # Only blue contours are present
        x_left, y_left = x_blue, y_blue
        x_right, y_right = x_blue, y_blue
    elif x_yellow is not None:  # Only yellow contours are present
        x_left, y_left = x_yellow, y_yellow
        x_right, y_right = x_yellow, y_yellow
    else:  # Neither blue nor yellow contours are present
        return 0.0

    x_diff = calculate_x_diff(x_left, x_right)

    left_angle = 0.0
    right_angle = 0.0

    if y_right is not None and y_left is not None:
        right_angle = math.atan2(y_right, max(x_diff, 1.0))
        left_angle = math.atan2(y_left, max(x_diff, 1.0))
    elif y_left is not None:
        left_angle = math.atan2(y_left, max(x_diff, 1.0))
        right_angle = left_angle * 1.15
    elif y_right is not None:
        right_angle = math.atan2(y_right, max(x_diff, 1.0))
        left_angle = right_angle * 1.15

    res = left_angle - right_angle
    return res

def get_min_point(contours):
    if not contours:
        return None, None
    min_point_index = min(range(len(contours)), key=lambda index: contours[index][1])
    return contours[min_point_index]

def calculate_x_diff(x_left, x_right):
    if x_left is not None and x_right is not None:
        return (x_right - x_left) / 2.0
    elif x_left is not None:
        return 720 - x_left
    elif x_right is not None:
        return x_right
    else:
        return 0.0
